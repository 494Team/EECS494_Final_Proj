#ifndef SPELLS_H
#define SPELLS_H

#include "Spell.h"
#include "Player.h"

namespace Flame {

  const float kHealing_size = 16.f;
  const float kHealing_speed = 50.f;
  const float kHealing_life_time = 3.f;
  const float kHealing_healing_amount = -50.f;

  const float kTaunt_radius = 112.f;
  const float kTaunt_hatred = 1000.f;
  const float kTaunt_life_time = 0.15f;

  const Zeni::Vector2f kArrow_size = Zeni::Vector2f(4.f, 16.f);
  const float kArrow_speed = 200.f;
  const float kArrow_life_time = 5.f;
  const float kArrow_damage = 50.f;

  const Zeni::Vector2f kTrap_size = Zeni::Vector2f(16.f, 16.f);
  const float kTrap_life_time = 30.f;
  const float kTrap_damage = 50.f;

  const float kFireball_size = 16.0f;
  const float kFireball_speed = 50.0f;
  const float kFireball_life_time = 3.0f;
  const float kFireball_damage = 50.0f;

  const Zeni::Vector2f kFire_spike_size = Zeni::Vector2f(50.f, 50.f);
  const float kFire_spike_life_time = 5.f;
  const float kFire_spike_damage = 50.f;

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

  // Tripitaka
  class Healing_spell : public Moving_spell_circle {
  public:
    Healing_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                  const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  // Pigsy
  class Taunt : public Resizable_spell {
  public:
    Taunt(const Zeni::Point2f& location_ = Zeni::Point2f(), Player * player_ptr_ = nullptr);
    virtual void render();
  private:
    Player * player_ptr;
  };

  // Friar Sand
  class Arrow_attack : public Moving_spell_rectangle {
  public:
    Arrow_attack(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  class Strafe : public Spell{
  public:
    Strafe(const Zeni::Point2f& location_ = Zeni::Point2f(),
           const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual Zeni::Point2f get_location() const
      {return location;}
    virtual void update_body() {}
    virtual void update(float time = 0.f);
    virtual void render();

  private:
    Zeni::Point2f location;
    Arrow_attack arrow0;
    Arrow_attack arrow1;
    Arrow_attack arrow2;
    Arrow_attack arrow3;
    Arrow_attack arrow4;
  };

  class Trap : public Resizable_spell {
  public:
    Trap(const Zeni::Point2f& location_ = Zeni::Point2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  // Boss1
  class Fire_ball : public Moving_spell_circle {
  public:
    Fire_ball(const Zeni::Point2f& location_ = Zeni::Point2f(),
              const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  class Fire_spikes : public Resizable_spell {
  public:
    Fire_spikes(const Zeni::Point2f& location_ = Zeni::Point2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

}

#endif