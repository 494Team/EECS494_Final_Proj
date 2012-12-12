#include <sstream>
#include "Bullking.h"
#include "zenilib.h"

using namespace Flame;

Bullking::Bullking(const Zeni::Point2f &location_)
  : Boss(BULLKING_HEALTH, BULLKING_SPEED, BULLKING_RADIUS, BULLKING_ATTACK_GAP,
             BULLKING_HELLDOOR_COOLDOWN,
             BULLKING_MARCH_COOLDOWN,
             BULLKING_EXPLOSION_COOLDOWN,
             location_),
  damage(BULLKING_DAMAGE),
  attack_radius(BULLKING_ATTACK_RADIUS)
{
  play_sound("dark_laugh");
  room_ul = Zeni::Point2f(0.0f, 0.0f);
  room_lr = Zeni::Point2f(600.0f, 600.0f);
}

void Bullking::attack() {
  float dist = (get_location() - target->get_location()).magnitude()
    - get_body().get_radius() - target->get_body().get_radius();
  if (dist < BULLKING_MIN_DIST) {
    Monster::attack();
    Zeni::Point2f attack_location = get_location() + get_current_orientation() * 0.5f * get_radius();
    Attack_spell *attack_spell = new Attack_spell(attack_location, get_current_orientation(), attack_radius, damage);
    Model_state::get_instance()->add_spell(attack_spell);
  } else {
    status = IDLE;
  }
}

// helldoor
void Bullking::skill1() {
  bool is_collision = true;
  Helldoor *new_helldoor;
  while (is_collision) {
    Zeni::Point2f helldoor_pos(
      randomer.frand_lte() * (room_lr.x - room_ul.x - 100.0f) + 50.0f, 
      randomer.frand_lte() * (room_lr.y - room_ul.y - 100.0f) + 50.0f
    );
    new_helldoor = new Helldoor(helldoor_pos);
    if (Model_state::get_instance()->can_monster_move(new_helldoor->get_body())) {
      is_collision = false;
    }
  }
  Model_state::get_instance()->add_monster(new_helldoor);
}

// march
void Bullking::skill2() {
  set_invincible(true);
  march_last_new_time = get_current_time();
}

// explosion
void Bullking::skill3() {
  set_invincible(true);
  explosion_last_new_time = get_current_time();
  explosion_last_render_time = get_current_time();
  explosion_last_render_suffix = 0;
}

void Bullking::update(float time) {
  Boss::update(time);
  if (target == 0) {
    is_attacking = false;
    status = IDLE;
    return;
  }
  if (is_get_wukong_charge()) {
    is_attacking = false;
    set_moving(false);
    return;
  }

  set_moving(false);
  switch (status) {
    case ATTACK:
      if (get_current_time() - last_skill_starts_time > ATTACK_DURATION) {
        is_attacking = false;
        status = IDLE;
      }
      break;
    case SKILL1:
      // hell door
      if (get_current_time() - last_skill_starts_time > ATTACK_DURATION) {
        status = IDLE;
      }
      break;
    case SKILL2:
      // march
      if (get_current_time() - last_skill_starts_time > BULLKING_MARCH_DURATION) {
        status = IDLE;
        set_invincible(false);
      } else {
        if (get_current_time() - march_last_new_time > BULLKING_MARCH_NEW_GAP) {
          march_last_new_time = get_current_time();
          // new mini bullking
          int edge = randomer.rand_lt(4);
          // temporary. change!
          float mini_bullking_radius = 5.0f;
          float edge_length = room_lr.x - room_ul.x - mini_bullking_radius * 2.0f;
          Zeni::Point2f new_mini_bullking_pos;
          Zeni::Vector2f new_mini_bullking_ori;
          switch (edge) {
            case 0:
              // up
              new_mini_bullking_pos = Zeni::Point2f(
                room_ul.x + mini_bullking_radius + edge_length * randomer.frand_lte(),
                room_ul.y
              );
              new_mini_bullking_ori = Zeni::Vector2f(0.0f, 1.0f);
              break;
            case 1:
              // right
              new_mini_bullking_pos = Zeni::Point2f(
                room_lr.x,
                room_ul.y + mini_bullking_radius + edge_length * randomer.frand_lte()
              );
              new_mini_bullking_ori = Zeni::Vector2f(-1.0f, 0.0f);
              break;
            case 2:
              // down
              new_mini_bullking_pos = Zeni::Point2f(
                room_ul.x + mini_bullking_radius + edge_length * randomer.frand_lte(),
                room_lr.y
              );
              new_mini_bullking_ori = Zeni::Vector2f(0.0f, -1.0f);
              break;
            case 3:
              // left
              new_mini_bullking_pos = Zeni::Point2f(
                room_ul.x,
                room_ul.y + mini_bullking_radius + edge_length * randomer.frand_lte()
              );
              new_mini_bullking_ori = Zeni::Vector2f(1.0f, 0.0f);
              break;
          }
          Model_state::get_instance()->add_spell(new Dajun(new_mini_bullking_pos, new_mini_bullking_ori));
        }
      }
      break;
    case SKILL3:
      // explosion
      if (get_current_time() - last_skill_starts_time > BULLKING_EXPLOSION_DURATION) {
        status = IDLE;
        set_invincible(false);
      } else {
        if (get_current_time() - explosion_last_new_time > BULLKING_EXPLOSION_NEW_GAP) {
          explosion_last_new_time = get_current_time();
          int player_num = randomer.rand_lt(players.size());
          Model_state::get_instance()->add_spell(new Explosion(players[player_num]->get_location()));
        }
      }
      break;
    case IDLE:
      float dist = (get_location() - target->get_location()).magnitude()
        - get_body().get_radius() - target->get_body().get_radius();
      set_orientation(target->get_location() - get_location());
      if (dist > BULLKING_MIN_DIST) {
        set_moving(true);
        make_move(time);
      }
      decide_attack_type();
      // if haven't attacked target for a long time (8 secs for now). clear hatred 
      if (get_current_time() - get_prev_attack_time() > 8.0f) {
        clear_hatred(target);
      }
      break;
  }
}

void Bullking::render() {
  Agent::render();
  float scale = Model_state::get_instance()->get_scale();
  Zeni::Point2f ul, lr;
  float radians_ccw;
  // image is 1.8x real size
  get_render_params(get_body().get_radius() * 1.8f, ul, lr, radians_ccw);
  
  update_render_suffix();
  if (!is_currently_moving())
    render_suffix = "0";
  switch (status) {
    case SKILL1:
      Zeni::render_image("bullking_casting0", ul, lr);
      break;
    case SKILL2:
      Zeni::render_image("bullking_casting1", ul, lr);
      break;
    case SKILL3: {
      Zeni::String texture = "bullking_casting2_";
      if (get_current_time() - explosion_last_render_time > BULLKING_EXPLOSION_BODY_CHANGE_TIME) {
        explosion_last_render_time = get_current_time();
        explosion_last_render_suffix++;
        explosion_last_render_suffix %= 4;
      }
      std::stringstream ss;
      ss << explosion_last_render_suffix;
      texture = texture + ss.str().c_str();
      Zeni::render_image(texture, ul, lr);
      break;
    }
    case ATTACK:
      if (is_attacking) {
        Zeni::render_image(
          "sword_attack",
          ul + scale * Zeni::Vector2f(get_body().get_radius(), 0.0f), 
          lr + scale * Zeni::Vector2f(get_body().get_radius(), 0.0f), 
          -radians_ccw, 
          1.0f, 
          rel_loc
        );
        render_suffix = "_attack";
      }
    case IDLE:
      Zeni::Color color_filter;
      if (is_slowdown()) {
        color_filter = SLOWDOWN_COLOR;
      }
      if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
        Zeni::render_image("bullking_right" + render_suffix, ul, lr, false, color_filter);
      } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
        Zeni::render_image("bullking_front" + render_suffix, ul, lr, false, color_filter);
      } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
        Zeni::render_image("bullking_left" + render_suffix, ul, lr, false, color_filter);
      } else {
        Zeni::render_image("bullking_back" + render_suffix, ul, lr, false, color_filter);
      }
      break;
  }

  if (is_taunt()) {
    float taunt_render_radius = get_radius() * 0.6f;
    Zeni::Point2f taunt_pos = rel_loc + scale * (get_radius() + taunt_render_radius) * Zeni::Vector2f(0.0f, -1.0f);
    Zeni::Point2f taunt_ul = taunt_pos - scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::Point2f taunt_lr = taunt_pos + scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::render_image("taunt_effect", taunt_ul, taunt_lr);
  }

  if (is_get_wukong_charge()) {
    render_get_wukong_charge_effect();
  }
}