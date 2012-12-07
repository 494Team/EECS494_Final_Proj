#include "Redboy.h"
#include "zenilib.h"

using namespace Flame;

Redboy::Redboy(const Zeni::Point2f &location_)
  : Boss(REDBOY_HEALTH, REDBOY_SPEED, REDBOY_RADIUS, REDBOY_ATTACK_GAP,
         REDBOY_FIRE_CHARGE_COOLDOWN, 
         REDBOY_RING_OF_FIRE_COOLDOWN, 
         REDBOY_FIRE_SPIKES_COOLDOWN, 
         location_),
  damage(REDBOY_DAMAGE),
  attack_radius(REDBOY_ATTACK_RADIUS)
{}

void Redboy::attack() {
  float dist = (get_location() - target->get_location()).magnitude()
    - get_body().get_radius() - target->get_body().get_radius();
  if (dist < REDBOY_MIN_DIST) {
    //TODO: perform attack
    Monster::attack();
  } else {
    status = IDLE;
  }
}

// fire charge
void Redboy::skill1() {
  int total_p_num = Model_state::get_instance()->get_player_num();
  int player_num = randomer.rand_lt(total_p_num);
  if (total_p_num > 1) {
      while (players[player_num] == target) {
        player_num = randomer.rand_lt(total_p_num);
      }
  }
  fire_charge_tar_loc = players[player_num]->get_location();
  set_orientation(fire_charge_tar_loc - get_location());
  set_speed(REDBOY_FIRE_CHARGE_SPEED);
  fire_charge_render_list.clear();
  fire_charge_last_shadow_time = 0.0f;
  fire_charge_main_body_stop = false;
  fire_charge_speed = get_current_speed();
  fire_charge_ori = get_current_orientation();
  fire_charge_start_loc = get_location();
}

// ring of fire : 8 fire balls
void Redboy::skill2() {
  int num_fire_balls = 8;
  float theta = Zeni::Global::pi / 4.0f;
  for (int i = 0; i < num_fire_balls; ++i) {
    Zeni::Vector3f ori = Zeni::Vector3f(0.0f, -1.0f, 0.0f);
    Zeni::Quaternion rotate = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, -1.0f), theta * i);
    ori = rotate * ori;
    Model_state::get_instance()->add_spell(new Ring_of_fire(get_location(), Zeni::Point2f(ori.i, ori.j)));
  }
}

// fire spikes
void Redboy::skill3() {
  int total_p_num = Model_state::get_instance()->get_player_num();
  int player_num = randomer.rand_lt(total_p_num);
  Model_state::get_instance()->add_spell(new Hell_spikes(players[player_num]->get_location()));
}

void Redboy::update(float time) {
  Boss::update(time);
  if (target == 0) {
    return;
  }

  set_moving(false);
  switch (status) {
    case ATTACK:
      if (get_current_time() - last_skill_starts_time > attack_gap) {
        is_attacking = false;
        status = IDLE;
      }
      //std::cout<<"ATTACK\n";
      break;
    case SKILL1: {
      // fire charge is special.
      Zeni::Point2f redboy_backup_loc = get_location();
      if (get_current_time() - last_skill_starts_time > ATTACK_DURATION) {
        if (get_current_time() - fire_charge_last_shadow_time > 0.1f && fire_charge_render_list.size() < 4) {
          fire_charge_render_list.push_back(fire_charge_start_loc);
          fire_charge_last_shadow_time = get_current_time();
        }
        if (!fire_charge_main_body_stop) {
          // main body not yet stop
          if ((get_location() - fire_charge_tar_loc).magnitude() < get_body().get_radius()) {
            // main body stop
            fire_charge_main_body_stop = true;
          } else {
            set_moving(true);
            make_move(time, true);
            for (int i = 0; i < (int)players.size(); ++i) {
              if (players[i]->get_body().intersects(get_body())) {
                // make effects to player
                std::vector<attack_effect> effects;
                effects.push_back(HITBACK);
                players[i]->get_hit(REDBOY_FIRE_CHARGE_DAMAGE, effects);
                Zeni::Vector2f hitback_ori = players[i]->get_location() - get_location();
                hitback_ori.normalize();
                players[i]->set_orientation(hitback_ori);
                players[i]->set_speed(get_current_speed() * 2.0f);
                // move player so that there is no intersection
                Zeni::Point2f player_backup_loc = players[i]->get_location();
                players[i]->set_position(get_location() + hitback_ori * (get_body().get_radius() + players[i]->get_body().get_radius() + EPSILON));
                // check if player's move is safe
                if (!Model_state::get_instance()->can_player_move(players[i]->get_body())) {
                  // not possible to move any further. go back and stop this skill
                  players[i]->set_position(player_backup_loc);
                  set_position(redboy_backup_loc);
                  fire_charge_main_body_stop = true;
                  break;
                }
              }
            }
          }
          for (int i = 0; i < (int)fire_charge_render_list.size(); ++i) {
            fire_charge_render_list[i] += (get_location() - redboy_backup_loc);
          }
        } else {
          // main body already stopped
          bool still_moving = false;
          // move shadow
          for (int i = 0; i < (int)fire_charge_render_list.size(); ++i) {
            if ((fire_charge_render_list[i] - get_location()).magnitude() >= get_body().get_radius()) {
              still_moving = true;
              fire_charge_render_list[i] += (fire_charge_ori * time * fire_charge_speed);
            }
          }
          if (!still_moving) {
            // no shadow moving, stop skill
            recover_speed();
            set_orientation(target->get_location());
            status = IDLE;
          }
        }
      }
      
     /* std::cout << "pos: " << get_location().x << ' ' << get_location().y <<std::endl;
      std::cout << "tar_pos: " << fire_charge_tar_loc.x << ' ' << fire_charge_tar_loc.y <<std::endl;*/
      //std::cout << "SKILL1\n";
      break;
    }
    case SKILL2:
      if (get_current_time() - last_skill_starts_time > attack_gap) {
        status = IDLE;
      }
      //std::cout << "SKILL2\n";
      break;
    case SKILL3:
      if (get_current_time() - last_skill_starts_time > attack_gap) {
        status = IDLE;
      }
      //std::cout << "SKILL3\n";
      break;
    case IDLE:
      float dist = (get_location() - target->get_location()).magnitude()
        - get_body().get_radius() - target->get_body().get_radius();
      set_orientation(target->get_location() - get_location());
      if (dist > REDBOY_MIN_DIST) {
        set_moving(true);
        make_move(time);
      }
      decide_attack_type();
      // if haven't attacked target for a long time (8 secs for now). clear hatred 
      if (get_current_time() - get_prev_attack_time() > 8.0f) {
        clear_hatred(target);
      }
      //std::cout << "IDLE\n";
  }
}

void Redboy::render() {
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
      if (get_current_time() - last_skill_starts_time > ATTACK_DURATION) {
        Zeni::Vector2f backup_loc = get_location();
        for (int i = (int)fire_charge_render_list.size() - 1; i >= 0; --i) {
          set_position(fire_charge_render_list[i]);
          get_render_params(get_body().get_radius() * 1.8f, ul, lr, radians_ccw);
          Zeni::Color color_filter((4 - i) * 0.25f, 1.0f, 1.0f, 1.0f);
          if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
            Zeni::render_image("redboy_right1", ul, lr, false, color_filter);
          } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
            Zeni::render_image("redboy_front1", ul, lr, false, color_filter);
          } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
            Zeni::render_image("redboy_left1", ul, lr, false, color_filter);
          } else {
            Zeni::render_image("redboy_back1", ul, lr, false, color_filter);
          }
        }
        set_position(backup_loc);

      } else {
        Zeni::render_image("redboy_casting0", ul, lr, 0, 1.0f, rel_loc);
      }
      break;
    case SKILL2:
      Zeni::render_image("redboy_casting1", ul, lr, 0, 1.0f, rel_loc);
      break;
    case SKILL3:
      Zeni::render_image("redboy_casting2", ul, lr, 0, 1.0f, rel_loc);
      break;
    case ATTACK:
      Zeni::render_image(
        "sword_attack", 
        ul + scale * Zeni::Vector2f(get_body().get_radius(), 0.0f), 
        lr + scale * Zeni::Vector2f(get_body().get_radius(), 0.0f), 
        -radians_ccw, 
        1.0f, 
        rel_loc
      );
      render_suffix = "_attack";
    case IDLE: {

      if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
        Zeni::render_image("redboy_right" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
      } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
        Zeni::render_image("redboy_front" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
      } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
        Zeni::render_image("redboy_left" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
      } else {
        Zeni::render_image("redboy_back" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
      }
      break;
    }
  }
  Agent::render();
}