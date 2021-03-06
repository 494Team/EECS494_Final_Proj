#include <Agent.h>
#include <zenilib.h>

using namespace Flame;

void Agent::update_body()
{
  body = Zeni::Collision::Capsule(
    Zeni::Point3f(get_location().x, get_location().y, 0.f),
    Zeni::Point3f(get_location().x, get_location().y, kCollision_object_height),
    radius
  );
}

void Agent::dec_health(const float &damage) {
  if (damage < 0.0f) {
    health -= damage;
  } else {
    health -= damage * armor;
  }
  if (health <= 0) {
    alive = false;
    health = 0.0f;
  }
  if (health > initial_health) {
    health = initial_health;
  }
  //for revive
  if (!alive && health > 0.0f) {
    alive = true;
  }
}

void Agent::update(float time) {
  for (std::map<attack_effect, float>::iterator it = effect_timers.begin(); it != effect_timers.end(); ++it) {
    if (it->second > 0.0f) {
      it->second -= time;
     // if (it->first == HITBACK) {
      //  std::cerr << "timer: " <<it->second << std::endl;
    //  }
    }
    if (it->second <= 0.0f) {
      it->second = 0.0f;
      switch (it->first) {
        case HITBACK:
          hitback = false;
          set_orientation(ori_before_hitback);
          recover_speed();
          break;
        case SLOWDOWN:
          slowdown = false;
          recover_speed();
          break;
        case FREEZE:
          freeze = false;
          break;
        case TAUNT:
          taunt = false;
          break;
      }
    }
  }
}

void Agent::render() {
  float scale = Model_state::get_instance()->get_scale();
  Zeni::Point2f rel_loc = (get_location() - Model_state::get_instance()->get_center_location()) * scale + Zeni::Point2f(400.0f, 300.0f);
  Zeni::Video &vr = Zeni::get_Video();
  Zeni::Colors &cr = Zeni::get_Colors();

  Zeni::Vertex2f_Color hpp0(rel_loc - Zeni::Point2f(radius * scale, radius * 1.6f * scale), cr["red"]);
  Zeni::Vertex2f_Color hpp1(rel_loc - Zeni::Point2f(radius * scale, radius * 1.6f * scale) + Zeni::Point2f(0.0f, kHpbar_width * scale), cr["red"]);
  Zeni::Vertex2f_Color hpp2(rel_loc - Zeni::Point2f(radius * scale, radius * 1.6f * scale) + Zeni::Point2f(2 * radius * health/initial_health * scale, kHpbar_width * scale), cr["red"]);
  Zeni::Vertex2f_Color hpp3(rel_loc - Zeni::Point2f(radius * scale, radius * 1.6f * scale) + Zeni::Point2f(2 * radius * health/initial_health * scale, 0.0f), cr["red"]);
  Zeni::Quadrilateral<Zeni::Vertex2f_Color> hppanel(hpp0, hpp1, hpp2, hpp3);
  vr.render(hppanel);
}

void Agent::get_hit(
  const float &damage,
  const std::vector<attack_effect> &effects = std::vector<attack_effect>(),
  Player* attacker,
  Zeni::Vector2f coming_ori) {

  dec_health(damage);
  for (int i = 0; i < (int) effects.size(); ++i) {
    switch (effects[i]) {
      case HITBACK:
        if (is_freeze() || is_get_wukong_charge()) {
          break;
        }
        if (!is_hitback()) {
          ori_before_hitback = get_current_orientation();
          set_speed(get_current_speed() * 2.0f);
        }
        set_orientation(coming_ori);
        hitback = true;
        if (attacker == 0) {
          effect_timers[effects[i]] = BOSS_CHARGE_HITBACK_TIME;
        } else {
          effect_timers[effects[i]] = NORMAL_HITBACK_TIME;
        }
        break;
      case SLOWDOWN:
        if (is_get_wukong_charge()) {
          break;
        }
        slowdown = true;
        set_speed(get_current_speed() * 0.5f);
        effect_timers[effects[i]] = SLOWDOWN_TIME;
        break;
      case FREEZE:
        freeze = true;
        effect_timers[effects[i]] = FREEZE_TIME;
        break;
      case GET_WUKONG_CHARGE:
        get_wukong_charge = true;
        break;
      case TAUNT:
        taunt = true;
        effect_timers[effects[i]] = TAUNT_EFFECT_RENDER_TIME;
        break;
    }
  }
}

