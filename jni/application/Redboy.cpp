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

  } else {
    status = IDLE;
  }
}

// fire charge
void Redboy::skill1() {
  int player_num = randomer.rand_lt(4);
  while (players[player_num] == target) {
    player_num = randomer.rand_lt(4);
  }
  fire_charge_tar_loc = players[player_num]->get_location();
  set_orientation(fire_charge_tar_loc - get_location());
  set_speed(get_current_speed() * 2.0f);
}

// ring of fire : 8 fire balls
void Redboy::skill2() {
  int num_fire_balls = 8;
  float theta = Zeni::Global::pi / 4.0f;
  for (int i = 0; i < num_fire_balls; ++i) {
    Zeni::Vector3f ori = Zeni::Vector3f(0.0f, -1.0f, 0.0f);
    Zeni::Quaternion rotate = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, -1.0f), theta * i);
    ori = rotate * ori;
    Model_state::get_instance()->add_spell(new Fire_ball(get_location(), Zeni::Point2f(ori.i, ori.j)));
  }
}

// fire spikes
void Redboy::skill3() {
  int player_num = randomer.rand_lt(4);
  Model_state::get_instance()->add_spell(new Fire_spikes(players[player_num]->get_location()));
}

void Redboy::update(float time) {
  Boss::update(time);
  target = highest_hatred();

  switch (status) {
    case ATTACK:
      if (get_current_time() - last_skill_starts_time > attack_gap) {
        is_attacking = false;
        status = IDLE;
      }
      //std::cout<<"ATTACK\n";
      break;
    case SKILL1:
      // fire charge is special.
      if (get_current_time() - last_skill_starts_time > ATTACK_DURATION) {
        if ((get_location() - fire_charge_tar_loc).magnitude() < get_body().get_radius()) {
          // if ends
          recover_speed();
          set_orientation(target->get_location());
          status = IDLE;
        } else {
          make_move(time);
          for (int i = 0; i < (int)players.size(); ++i) {
            if (players[i]->get_body().intersects(get_body())) {
              std::vector<attack_effect> effects;
              effects.push_back(HITBACK);
              players[i]->get_hit(REDBOY_FIRE_CHARGE_DAMAGE, effects);
              players[i]->set_orientation(players[i]->get_location() - get_location());
              players[i]->set_speed(get_current_speed() * 1.5f);
            }
          }
        }
      }
      
     /* std::cout << "pos: " << get_location().x << ' ' << get_location().y <<std::endl;
      std::cout << "tar_pos: " << fire_charge_tar_loc.x << ' ' << fire_charge_tar_loc.y <<std::endl;*/
      //std::cout << "SKILL1\n";
      break;
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
        make_move(time);
      }
      decide_attack_type();
      //std::cout << "IDLE\n";
  }
}

void Redboy::render() {
  float scale = Model_state::get_instance()->get_scale();
  Zeni::Point2f ul, lr;
  float radians_ccw;
  get_render_params(ul, lr, radians_ccw);

      //std::cout << "ori: " << get_current_orientation().x << ' ' << get_current_orientation().y << std::endl;
  switch (status) {
    case SKILL1:
      if (get_current_time() - last_skill_starts_time > ATTACK_DURATION) {
        if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
          Zeni::render_image("redboy_right", ul, lr, 0, 1.0f, rel_loc);
        } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
          Zeni::render_image("redboy_front", ul, lr, 0, 1.0f, rel_loc);
        } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
          Zeni::render_image("redboy_left", ul, lr, 0, 1.0f, rel_loc);
        } else {
          Zeni::render_image("redboy_back", ul, lr, 0, 1.0f, rel_loc);
        }

      } else {
        Zeni::render_image("redboy_casting", ul, lr, 0, 1.0f, rel_loc);
      }
      break;
    case SKILL2:
      Zeni::render_image("redboy_casting", ul, lr, 0, 1.0f, rel_loc);
      break;
    case SKILL3:
      Zeni::render_image("redboy_casting", ul, lr, 0, 1.0f, rel_loc);
      break;
    case ATTACK:
      Zeni::render_image("sword_attack", ul, lr, 0.0f, 1.0f, rel_loc);
    case IDLE: {

      if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
        Zeni::render_image("redboy_right", ul, lr, 0, 1.0f, rel_loc);
      } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
        Zeni::render_image("redboy_front", ul, lr, 0, 1.0f, rel_loc);
      } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
        Zeni::render_image("redboy_left", ul, lr, 0, 1.0f, rel_loc);
      } else {
        Zeni::render_image("redboy_back", ul, lr, 0, 1.0f, rel_loc);
      }
      break;
    }
  }
  Agent::render();
}