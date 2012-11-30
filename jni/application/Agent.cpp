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
}

void Agent::update(float time) {
  for (std::map<attack_effect, float>::iterator it = effect_timers.begin(); it != effect_timers.end(); ++it) {
    it->second -= time;
    if (it->second < 0.0f) {
      it->second = 0.0f;
      switch (it->first) {
        case HITBACK:
          hitback = false;
          recover_speed();
          break;
        case SLOWDOWN:
          recover_speed();
          break;
        case FREEZE:
          freeze = false;
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
  Player* attacker) {

  dec_health(damage);
  for (int i = 0; i < (int) effects.size(); ++i) {
    switch (effects[i]) {
      case HITBACK:
        set_speed(-get_current_speed());
        hitback = true;
        effect_timers[effects[i]] = HITBACK_TIME;
        break;
      case SLOWDOWN:
        set_speed(get_current_speed() * 0.5f);
        effect_timers[effects[i]] = SLOWDOWN_TIME;
        break;
      case FREEZE:
        freeze = true;
        effect_timers[effects[i]] = FREEZE_TIME;
        break;
    }
  }
}

