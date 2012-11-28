#ifndef SPELLS_H
#define SPELLS_H

#include "Spell.h"
#include "Player.h"

namespace Flame {

  const float kHealing_size = 16.f;
  const float kHealing_speed = 50.f;
  const float kHealing_life_time = 3.f;
  const float kHealing_healing_amount = -50.f;
  const Zeni::Vector2f kArrow_size = Zeni::Vector2f(4.f, 16.f);
  const float kArrow_speed = 200.f;
  const float kArrow_life_time = 5.f;
  const float kArrow_damage = 50.f;

  class Attack_spell : public Spell {
  public:
    Attack_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                 float radius_ = 0.f,
                 float attack_strength_ = 0.f,
                 bool is_player_ = false,
                 bool heal_self_ = false,
                 Player * player_ptr_ = nullptr);
    virtual Zeni::Point2f get_location() const {return Zeni::Point2f();}
    virtual void update_body() {}
    virtual void update(float time = 0.f); 
    virtual void render() {};
  private:
    Collision_sector body;
    Player * player_ptr;
    float attack_strength;
    bool is_player;
    bool heal_self;
  };

  class Healing_spell : public Moving_spell_circle {
  public:
    Healing_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                  const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  class Arrow_attack : public Moving_spell_rectangle {
  public:
    Arrow_attack(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

}

#endif