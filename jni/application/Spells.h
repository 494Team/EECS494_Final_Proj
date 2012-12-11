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

  const Zeni::Vector2f kGet_hit_size = Zeni::Vector2f(48.f, 48.f);
  const float kGet_hit_life_time = 0.25f;

  const float kDisintegrate_length = 384.f;
  const float kDisintegrate_radius = 8.f;

  const Zeni::Vector2f kDing_size = Zeni::Vector2f(100.f, 100.f);
  const float kDing_life_time = 3.f;
  const float kDing_damage = 5.f;

  const float kHealing_size = 16.f;
  const float kHealing_speed = 50.f;
  const float kHealing_life_time = 3.f;
  const float kHealing_healing_amount = -50.f;

  const float kTaunt_radius = 112.f;
  const float kTaunt_hatred = 1000.f;
  const float kTaunt_life_time = 1.f;

  const Zeni::Vector2f kArrow_size = Zeni::Vector2f(4.f, 16.f);
  const float kArrow_speed = 200.f;
  const float kArrow_life_time = 5.f;
  const float kArrow_damage = 50.f;

  const Zeni::Vector2f kMagic_arrow_size = Zeni::Vector2f(8.f, 32.f);
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

  const Zeni::Vector2f kRing_of_fire_size = Zeni::Vector2f(8.f, 16.f);
  const float kRing_of_fire_speed = 50.0f;
  const float kRing_of_fire_life_time = 5.0f;
  const float kRing_of_fire_damage = 50.0f;

  const Zeni::Vector2f kHell_spikes_size = Zeni::Vector2f(125.f, 125.f);
  const float kHell_spikes_pre_time = 1.f;
  const float kHell_spikes_life_time = 5.f;
  const float kHell_spikes_damage = 50.f;

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

  class Get_hit : public Resizable_spell {
  public:
    Get_hit(const Zeni::Point2f& location_ = Zeni::Point2f()) :
      Resizable_spell(location_, kGet_hit_size, Zeni::Vector2f(), kGet_hit_life_time),
      m_location(location_)
      {}
    virtual Zeni::Point2f get_location() const
      {return m_location + Zeni::Vector2f(0.f, 5.f);}
    virtual void render()
      {Resizable_spell::render("get_hit");}
  private:
    Zeni::Point2f m_location;
  };

  // Tripitaka
  class Disintegrate : public Spell {
  public:
    Disintegrate(Player * player_ptr_ = nullptr, float damage_ = 0.f);
    virtual Zeni::Point2f get_location() const;
    virtual void update_body();
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
    Zeni::Collision::Capsule body;
    float length;
    float render_time;
    float damage;
  };

  class Ding : public Resizable_spell {
  public:
    Ding(const Zeni::Point2f& location_ = Zeni::Point2f(), Player * player_ptr_ = nullptr, float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
    float render_time;
    float damage;
  };

  class Healing_spell : public Moving_spell_circle {
  public:
    Healing_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                  const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                  float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    float damage;
  };

  // Monkey King
  class Cudgel_fury : public Moving_spell_circle {
  public:
    Cudgel_fury(const Zeni::Point2f& location_ = Zeni::Point2f(),
                const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                const float size_ = 50.0f,
                Player * player_ptr_ = nullptr,
                Zeni::Chronometer<Zeni::Time>* game_time = nullptr,
                float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
    std::vector<Monster *> * monster_list_ptr;
    int render_flag;
    Zeni::Chronometer<Zeni::Time>* game_time;
    float last_render_time;
    float damage;
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
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                 Player * player_ptr_ = nullptr,
                 float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
    float damage;
  };

  class Magic_arrow_ice : public Moving_spell_rectangle {
  public:
    Magic_arrow_ice(const Zeni::Point2f& location_ = Zeni::Point2f(),
                    const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                    Player * player_ptr_ = nullptr,
                    float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
    float damage;
  };

  class Magic_arrow_fire : public Moving_spell_rectangle {
  public:
    Magic_arrow_fire(const Zeni::Point2f& locatioin_ = Zeni::Point2f(),
                      const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                      Player * player_ptr_ = nullptr,
                      float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Player * player_ptr;
    float damage;
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
    float timer;
    Player * player_ptr;
  };

  class Strafe : public Spell{
  public:
    Strafe(const Zeni::Point2f& location_ = Zeni::Point2f(),
           const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
           Player * player_ptr_ = nullptr,
           float damage_ = 0.f);
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
    Trap(const Zeni::Point2f& location_ = Zeni::Point2f(), Player * player_ptr_ = nullptr, float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    std::vector<Sim_object *> target_list;
    Player * player_ptr;
    int remain_times;
    float timer;
    float damage;
  };

  class Trap_attack : public Moving_spell_circle {
  public:
    Trap_attack(const Zeni::Point2f& location_ = Zeni::Point2f(),
                const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                float speed_ = kTrap_attack_speed,
                Player * player_ptr_ = nullptr,
                float damage_ = 0.f);
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    Zeni::Vector2f orientation;
    Zeni::Vector2f counter_orientation;
    int is_left;
    float time_counter;
    float render_timer;
    Player * player_ptr;
    float damage;
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

  class Ring_of_fire : public Moving_spell_rectangle {
  public:
    Ring_of_fire(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f());
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    float render_time;
  };

  class Hell_spikes : public Resizable_spell {
  public:
    Hell_spikes(const Zeni::Point2f& location_ = Zeni::Point2f());
    virtual void update(float time = 0.f);
    virtual void render();
  private:
    float pre_time;
  };

  class Explosion : public Resizable_spell {
  public:
    Explosion(const Zeni::Point2f& location_);
    void update(float time = 0.0f);
    void render();
  private:
    bool already_cause_damage;
    float lifetime_after_damage_effect;
    float damage_effect_render_time;
  };
}

#endif