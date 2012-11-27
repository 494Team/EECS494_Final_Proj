#ifndef SPELLS_H
#define SPELLS_H

#include "Spell.h"
#include "Player.h"

namespace Flame {

  class Attack_spell : public Spell {
  public:
    Attack_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                 float radius_ = 0.f,
                 float attack_strength_ = 0.f,
                 bool is_player_ = false,
                 bool heal_self_ = false,
                 Player * player_ptr_ = nullptr);
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


}

#endif