#ifndef SPELLS_H
#define SPELLS_H

#include "Spell.h"
#include "Player.h"
#include "Utility.h"
#include <vector>

namespace Flame {

  // forward declaration
  class Agent;
  class Player;

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

  const Zeni::Vector2f kMagic_arrow_size = Zeni::Vector2f(4.f, 16.f);
  const float kMagic_arrow_speed = 200.f;
  const float kMagic_arrow_life_time = 5.f;
  const float kMagic_arrow_damage = 50.f;
  const Zeni::Vector2f kMagic_arrow_effect_size = Zeni::Vector2f(50.f, 50.f);
  const float kMagic_arrow_effect_life_time = 5.f;
  const float kMagic_arrow_effect_damage = 1.f;

  const Zeni::Vector2f kTrap_size = Zeni::Vector2f(100.f, 100.f);
  const float kTrap_life_time = 30.f;
  const float kTrap_damage = 50.f;

  const Zeni::Vector2f kTrap_attack_size = Zeni::Vector2f(12.f, 12.f);
  const float kTrap_attack_speed = 50.f;
  const float kTrap_attack_life_time = 5.f;
  const float kTrap_attack_damage = 10.f;
  const float kTrap_attack_period = .1f;

  const float kFireball_size = 16.0f;
  const float kFireball_speed = 50.0f;
  const float kFireball_life_time = 3.0f;
  const float kFireball_damage = 50.0f;

  const Zeni::Vector2f kRing_of_fire_size = Zeni::Vector2f(16.0f, 16.f);
  const float kRing_of_fire_speed = 50.0f;
  const float kRing_of_fire_life_time = 5.0f;
  const float kRing_of_fire_damage = 50.0f;

  const Zeni::Vector2f kHell_spikes_size = Zeni::Vector2f(125.f, 125.f);
  const float kHell_spikes_pre_time = 1.f;
  const float kHell_spikes_life_time = 5.f;
  const float kHell_spikes_damage = 50.f;

  const float kCudgelfury_last = 3.0f;
  const float kCudgelfury_render_gap = 0.02f;
  const int kCudgelfury_render_max = 3;
  const float kCudgelfury_damage = 2.0f;

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
    Zeni::Vector2f orientation;
  };

  // Tripitaka
  class Healing_spell : public Moving_spell_circle {
  public:
    Healing_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                  const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  // Monkey King
  class Cudgel_fury : public Moving_spell_circle {
  public:
    Cudgel_fury(const Zeni::Point2f& location_ = Zeni::Point2f(),
                const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                const float size_ = 50.0f,
                Player * player_ptr_ = nullptr,
                Zeni::Chronometer<Zeni::Time>* game_time = nullptr);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
    std::vector<Monster *> * monster_list_ptr;
    int render_flag;
    Zeni::Chronometer<Zeni::Time>* game_time;
    float last_render_time;
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

  class Magic_arrow_ice : public Moving_spell_rectangle {
  public:
    Magic_arrow_ice(const Zeni::Point2f& location_ = Zeni::Point2f(),
                    const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                    Player * player_ptr_ = nullptr);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
  };

  class Magic_arrow_fire : public Moving_spell_rectangle {
  public:
    Magic_arrow_fire(const Zeni::Point2f& locatioin_ = Zeni::Point2f(),
                      const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                      Player * player_ptr_ = nullptr);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
  };

  class Magic_arrow_ice_effect : public Resizable_spell {
  public:
    Magic_arrow_ice_effect(const Zeni::Point2f& location_ = Zeni::Point2f(),
                           Player * player_ptr_ = nullptr);
    virtual void update(float) {}
    virtual void render() {};
  private:
    Player * player_ptr;
  };

  class Magic_arrow_fire_effect : public Resizable_spell {
  public:
    Magic_arrow_fire_effect(const Zeni::Point2f& location_ = Zeni::Point2f(),
                            Player * player_ptr_ = nullptr);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
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
    Trap(const Zeni::Point2f& location_ = Zeni::Point2f(), Player * player_ptr_ = nullptr);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    std::vector<Sim_object *> target_list;
    Player * player_ptr;
    int remain_times;
    float timer;
  };

  class Trap_attack : public Moving_spell_circle {
  public:
    Trap_attack(const Zeni::Point2f& location_ = Zeni::Point2f(),
                const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                float speed_ = kTrap_attack_speed,
                Player * player_ptr_ = nullptr);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Zeni::Vector2f orientation;
    Zeni::Vector2f counter_orientation;
    int is_left;
    float time_counter;
    Player * player_ptr;
  };

  class Track_attack : public Moving_spell_circle {
  public:
    Track_attack(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& size_ = Zeni::Vector2f(),
                 float speed_ = 0.f,
                 float damage_ = 0.f,
                 float life_time_ = 0.f,
                 std::vector<Flame::attack_effect> attack_effect_ = std::vector<Flame::attack_effect>(),
                 Agent * target_ptr_ = nullptr,
                 Player * player_ptr_ = nullptr);
    virtual void update(float time = 0.f);
    virtual void render();

  private:
    float damage;
    std::vector<attack_effect> attack_effect;
    Agent * target_ptr;
    Player * player_ptr;
  };

  // Boss1
  class Fire_ball : public Moving_spell_circle {
  public:
    Fire_ball(const Zeni::Point2f& location_ = Zeni::Point2f(),
              const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  class Ring_of_fire : public Moving_spell_circle {
  public:
    Ring_of_fire(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  };

  class Hell_spikes : public Resizable_spell {
  public:
    Hell_spikes(const Zeni::Point2f& location_ = Zeni::Point2f());
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    float pre_time;
  };

}

#endif