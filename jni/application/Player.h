#ifndef PLAYER_H
#define PLAYER_H

#include "Agent.h"
#include <vector>
#include <zenilib.h>
//#define NDEBUG
#include <cassert>
#include "Map.h"
#include "Model_state.h"
#include "Collision.h"
#include "Spells.h"

using namespace Zeni;

namespace Flame {
  #define PLAYER_ATTACK_INTERVAL 0.5f
  //temp
  const float kSpell1_CD = 1.0f;
  const float kSpell2_CD = 1.0f;
  const float kSpell3_CD = 1.0f;

  const float kAttack_show_time = 0.2f;
  #define PLAYER_SPELL1_CD 1.0f
  #define PLAYER_SPELL2_CD 1.0f
  #define PLAYER_SPELL3_CD 1.0f
  #define HURT_SHOWING_INTERVAL 0.2f
  const float kMove_speed = 500.0f;
  const float kRun_render_gap = 0.2f;
  const float kPlayer_attack_range = 40.0f;
  const float kPlayer_attack_strengh = 100.0f;

  // spells
  const float kShield_last = 0.5f;
  const float kShield_CD = 1.0f;
  const float kShield_effect = 0.5f;
  const float kBloodsuck_last = 0.5f;
  const float kBloodsuck_CD = 1.0f;
  const float kBloodsuck_effect = 0.5f;

  class Player: public Agent {
  public:
    Player(
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f());

    virtual void update(float time = 0.f);
    void render();

    //control
    Control ctrl;
    kPlayer_type ptype;
    void fire(kKey_type type);
    void try_spell1(const Zeni::Time_HQ current_time);
    void try_spell2(const Zeni::Time_HQ current_time);
    void try_spell3(const Zeni::Time_HQ current_time);

  private:
    float orient_vec_to_radians(Vector2f vec) {
      float radians = atan2(vec.i, vec.j);
      return radians;
    }

    //attack
    bool normal_attack;
    bool damaged; //whether the normal attack has created a damage
    Zeni::Time_HQ last_htime;

    //spell status
    float spell1_CD;
    float spell2_CD;
    float spell3_CD;
    Zeni::Time_HQ last_spell1;
    Zeni::Time_HQ last_spell2;
    Zeni::Time_HQ last_spell3;
    //SANZANG
    //WUKONG
    //SHASENG
    //BAJIE
    bool bloodsucking;
    void bloodsuck();
    bool shielding;
    void shield();

    //running renderer
    bool running_status;
    Zeni::Time_HQ render_clock;
  
  //vector<Agent*>* Monsters;
    int attack_strength;
    double attack_range;

  //relative location;
  Zeni::Point2f rel_loc;
  float size;
  };
}

#endif
