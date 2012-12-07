#ifndef AGENT_H
#define AGENT_H

#include "Moving_object.h"
#include "Sim_object.h"
#include "Collision.h"
#include "Model_state.h"
#include <map>

namespace Flame {

  class Agent: public Moving_object, public Sim_object {
  public:
    Agent(
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f())
    : Moving_object(speed_, location_),
      body(Zeni::Point3f(location_.x, location_.y, 0.f),
           Zeni::Point3f(location_.x, location_.y, kCollision_object_height)
           , radius_),
      initial_health(health_),
      health(health_),
      alive(true),
      radius(radius_),
      hitback(false),
      freeze(false),
      get_wukong_charge(false),
      armor(1.0f)
    {}

    Zeni::Point2f get_location()const {return Moving_object::get_current_location();}
    float get_current_health() const {return health;}
    float get_initial_health() const {return initial_health;}
    const Zeni::Collision::Capsule& get_body() const {return body;}
    bool is_alive() const {return alive;}
    bool is_hitback() const {return hitback;}
    bool is_freeze() const {return freeze;}
    bool is_get_wukong_charge() const {return get_wukong_charge;}
    void relieve_from_wukong_charge() {get_wukong_charge = false;}
    float get_armor() const {return armor;}

    void set_armor(float armor_) { armor = armor_; }

    void update(float time);

    void update_body();

    void render();

    // decrease health (if damage is negative, then increase)
    void dec_health(const float &damage);

    virtual void get_hit(const float &damage, const std::vector<attack_effect> &effects, Player* attacker=NULL, Zeni::Vector2f coming_ori = Zeni::Vector2f());

    float get_radius() {
      return radius;
    }
    void set_radius(const float radius_) {
      radius = radius_;
    }

    void set_position(Zeni::Point2f pos_) {
      Moving_object::set_position(pos_);
      update_body();
    }

  protected:
    Zeni::Vector2f ori_before_hitback;  
  private:
    std::map<attack_effect, float> effect_timers;
    Zeni::Collision::Capsule body;
    float armor; // armor is between 0.0 and 1.0
    float initial_health;
    float health;
    bool alive;
    float radius;

    // effects
    bool hitback;
    bool freeze;
    bool get_wukong_charge;
  };

}

#endif