#include "Monster.h"

using namespace std;
using namespace Flame;

Monster::Monster(
  const float &health_,
  const float &speed_,
  const float &radius_,
  const float &attack_gap_,
  const float &view_range_,
  const Zeni::Point2f &location_)
: Agent(health_, speed_, radius_, location_),
  players((*Model_state::get_instance()->get_player_list_ptr())),
  current_time(0.0f),
  attack_gap(attack_gap_),
  view_range(view_range_),
  is_attacking(false),
  invincible(false),
  last_render_change_time(0.0f),
  render_suffix("0")
{
  prev_attack_time = - 2.0f * attack_gap_;
  for (int i = 0; i < int(players.size()); ++i) {
    hatred[players[i]] = 0.0f;
  }

  set_orientation(Zeni::Vector2f(1.0f, 0.0f));
  float min_dist = INFINITY;
  int idx = -1;
  for (int i = 0; i < (int)players.size(); ++i) {
    float dist = (players[i]->get_location() - get_location()).magnitude();
    if (dist <= view_range && dist < min_dist) {
      min_dist = dist;
      idx = i;
    }
  }
  if (idx != -1) {
    hatred[players[idx]] += INITIAL_HATRED;
  }
  set_moving(true); 
}

void Monster::increase_hatred(const float &hate_, Player* player) {
  hatred[player] += hate_;
}

void Monster::clear_hatred(Player* player) {
  hatred[player] = 0;
}

Player * Monster::highest_hatred() {
  float max_hatred = 0.0f;
  Player* target_player = NULL;
  for (int i = 0; i < int(players.size()); ++i) {
    if (hatred[players[i]] > max_hatred) {
      max_hatred = hatred[players[i]];
      target_player = players[i];
    }
  }
  return target_player;
}

Player * Monster::nearest_player() {
  float min_dist = INFINITY;
  Player* nearest_p = NULL;
  for (int i = 0; i < int(players.size()); ++i) {
    if ((players[i]->get_location() - get_location()).magnitude() < min_dist) {
      min_dist = (players[i]->get_location() - get_location()).magnitude();
      nearest_p = players[i];
    }
  }
  return nearest_p;
}

void Monster::attack() {
  set_prev_attack_time(get_current_time());
  is_attacking = true;
}

bool Monster::can_attack() {
  if (current_time - prev_attack_time > attack_gap) {
    return true;
  } else {
    return false;
  }
}

void Monster::update_rel_loc() {
  float scale = Model_state::get_instance()->get_scale();
  rel_loc = (get_location() - Model_state::get_instance()->get_center_location()) * scale + RENDER_CENTER;
}

void Monster::make_move(float time, bool force_move) {
  if (!is_currently_moving())
    return;
  Zeni::Vector2f ori = get_current_orientation();
  Zeni::Point2f backup_loc = get_location();
  Zeni::Point2f attemp_loc;
  // move x
  attemp_loc = get_location() + time * get_current_speed() * ori.get_i();
  set_position(attemp_loc);
  update_body();
  if (force_move) {
    if (!Model_state::get_instance()->can_move(get_body())) {
      set_position(backup_loc);
      update_body();
    }
  } else {
    if (!Model_state::get_instance()->can_monster_move(get_body())) {
      set_position(backup_loc);
      update_body();
    }
  }
  // move y
  backup_loc = get_location();
  attemp_loc = get_location() + time * get_current_speed() * ori.get_j();
  set_position(attemp_loc);
  update_body();
  if (force_move) {
    if (!Model_state::get_instance()->can_move(get_body())) {
      set_position(backup_loc);
      update_body();
    }
  } else {
    if (!Model_state::get_instance()->can_monster_move(get_body())) {
      set_position(backup_loc);
      update_body();
    }
  }
}

float Monster::calc_angle_between(const Zeni::Vector2f &a, const Zeni::Vector2f &b) {
  float a_len = a.magnitude();
  float b_len = b.magnitude();
  float c_len = (b - a).magnitude();
  float cos_val = (a_len * a_len + b_len * b_len - c_len * c_len) / (2 * a_len * b_len);
  if (cos_val > 1.0f) {
    cos_val = 1.0f;
  } else if (cos_val < -1.0f) {
    cos_val = -1.0f;
  }
  return float(acos(cos_val));
}

void Monster::update(float time) {
  Agent::update(time);
  update_current_time(time);
  update_rel_loc();
  players = (*Model_state::get_instance()->get_player_list_ptr());
  for (int i = 0; i < (int)players.size(); ++i) {
    if ((players[i]->get_location() - get_location()).magnitude() > view_range) {
      clear_hatred(players[i]);
    }
  }
  if (highest_hatred() == NULL) {
    for (int i = 0; i < (int)players.size(); ++i) {
      if ((players[i]->get_location() - get_location()).magnitude() <= view_range) {
        increase_hatred(INITIAL_HATRED, players[i]);
      }
    }
  }
  last_target = target;
  target = highest_hatred();
  if (last_target != target) {
    increase_hatred(INITIAL_HATRED, target);
  }
}

void Monster::get_hit(const float &damage, const std::vector<attack_effect> &effects, Player* attacker, Zeni::Vector2f coming_ori) {
  Agent::get_hit(damage, effects, attacker, coming_ori);
  increase_hatred(damage, attacker);
}

void Monster::get_render_params(float render_radius, Zeni::Point2f &ul, Zeni::Point2f &lr, float &radians_ccw) {
  update_rel_loc();
  float scale = Model_state::get_instance()->get_scale();
  ul = rel_loc - Zeni::Vector2f(render_radius, render_radius) * scale;
  lr = ul + Zeni::Vector2f(render_radius * 2.0f, render_radius * 2.0f) * scale;
  if (is_hitback()) {
    radians_ccw = calc_angle_between(ori_before_hitback, Zeni::Vector2f(1.0f, 0.0f));
  } else {
    radians_ccw = calc_angle_between(get_current_orientation(), Zeni::Vector2f(1.0f, 0.0f));
  }
  if (is_hitback()) {
    if (ori_before_hitback. y < 0.0f) {
      radians_ccw = Zeni::Global::pi * 2.0f - radians_ccw;
    }
  } else {
    if (get_current_orientation().y < 0.0f) {
      radians_ccw = Zeni::Global::pi * 2.0f - radians_ccw;
    } 
  }
}

void Monster::update_render_suffix() {
  if (get_current_time() - last_render_change_time > 0.2f) {
    if (render_suffix == "0")
      render_suffix = "1";
    else
      render_suffix = "0";
    last_render_change_time = get_current_time();
  }
}