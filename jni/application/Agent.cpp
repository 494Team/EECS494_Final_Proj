#include <Agent.h>

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
  if (health < 0) {
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


void Agent::get_hit(const float &damage, const std::vector<attack_effect> &effects = std::vector<attack_effect>()) {
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

