#ifndef AGENT_H
#define AGENT_H

#include "Moving_object.h"
#include "Sim_object.h"
#include "Collision.h"

namespace Flame {

  class Agent: public Moving_object, public Sim_object {
  public:
    Agent(
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f())
    : health(health_), initial_health(health_), alive(true), 
      Moving_object(speed_, location_), body(location_, radius_)
    {}

    Zeni::Point2f get_location() const {return Moving_object::get_current_location();}
    Flame::Collision_circle get_body() const {return body;}
    bool is_alive() const {return alive;}
    
    // decrease health (if damage is negative, then increase)
    void dec_health(const float &damage) {
      health -= damage;
      if (health < 0) {
        alive = false;
        health = 0.0f;
      }
      if (health > initial_health) {
        health = initial_health;
      }
    }

  private:
    float initial_health;
    float health;
    bool alive;
    Flame::Collision_circle body;
  };

}

#endif