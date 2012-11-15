#ifndef AGENT_H
#define AGENT_H

#include "Moving_object.h"
#include "Sim_object.h"

namespace Flame {

  class Agent: private Moving_object, public Sim_object {
  public:
    Agent(
      const float &health_ = 0.0f, 
      const float &speed_ = 0.0f, 
      const Zeni::Point2f &location_ = Zeni::Point2f())
    : health(health_), alive(true) {
      Moving_object(speed_, location_);
    }

    Zeni::Point2f get_location() const {return Moving_object::get_current_location();}
    bool is_alive() const {return alive;}
    
    // getting hit cause health going down
    void get_hit(const float &damage) {
      health -= damage;
      if (health < 0) {
        alive = false;
        health = 0.0f;
      }
    }

  private:
    float health;
    bool alive;
  };

}

#endif