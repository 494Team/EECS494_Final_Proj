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

  const float kSpeed_player = 200.0f;

  const float kAttack_show_time = 0.2f;

  #define PLAYER_SPELL1_CD 1.0f
  #define PLAYER_SPELL2_CD 1.0f
  #define PLAYER_SPELL3_CD 1.0f
  #define HURT_SHOWING_INTERVAL 0.2f
  const float kMove_speed = 500.0f;
  const float kRun_render_gap = 0.2f;
  const float kPlayer_attack_range = 40.0f;
  const float kPlayer_attack_strengh = 100.0f;

  const float kInit_buff = 1.0f;

  // spells
  //SANZANG
  const float kConfusing_CD = 2.0f;
  const float kHealing_CD = 2.0f;
  const float kAll_healing_CD = 2.0f;
  //WUKONG
  const float kCudge_fury_CD = 2.0f;
  const float kCharge_CD = 2.0f;
  const float kBerserk_last = 1.0f;
  const float kBerserk_CD = 2.0f;
  const float kBerserk_enlarge = 1.5f;
  //SHASENG
  const float kStrafe_CD = 2.0f;
  const float kTrap_CD = 2.0f;
  const float kMagicarrow_CD = 2.0f;
  //BAJIE
  const float kShield_last = 0.5f;
  const float kShield_CD = 1.0f;
  const float kShield_effect = 0.5f;

  const float kTaunt_CD = 3.0f;

  const float kBloodsuck_last = 3.0f;
  const float kBloodsuck_CD = 6.0f;
  const float kBloodsuck_effect = 0.5f;

  class Player: public Agent {
  public:
    Player(
      Chronometer<Time>* game_t,
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f(),
      const kPlayer_type player_type_ = WUKONG);

    virtual void update(float time = 0.f);
    void render();

    //control
    Control ctrl;
    kPlayer_type ptype;
    void fire(kKey_type type);
    void try_normal_attack();
    void try_spell1();
    void try_spell2();
    void try_spell3();
    
    kPlayer_type get_player_type() {
      return ptype;
    }
    float get_CD1_percent() {
      float current_time = game_time->seconds();
      float passed_time = current_time - last_spell1;
      float percent = passed_time > spell1_CD ? 1.0f : passed_time/spell1_CD;
      return percent;
    }
    float get_CD2_percent() {
      float current_time = game_time->seconds();
      float passed_time = current_time - last_spell2;
      float percent = passed_time > spell1_CD ? 1.0f : passed_time/spell2_CD;
      return percent;
    }
    float get_CD3_percent() {
      float current_time = game_time->seconds();
      float passed_time = current_time - last_spell3;
      float percent = passed_time > spell1_CD ? 1.0f : passed_time/spell3_CD;
      return percent;
    }

  private:
    Chronometer<Time>* game_time;

    void hitback_move(float time);

    float orient_vec_to_radians(Vector2f vec) {
      float radians = atan2(vec.i, vec.j);
      return radians;
    }

    //attack
    bool normal_attack;
    //Zeni::Time_HQ last_htime;
    float last_htime;
    float attack_buff; //range: [1, +INF]; initial:1.0f

    //spell status
    float spell1_CD;
    float spell2_CD;
    float spell3_CD;

    bool spell1_active;
    bool spell2_active;
    bool spell3_active;
    
    //timer_begin
    float last_spell1;
    float last_spell2;
    float last_spell3;
    //timer_end

    //SANZANG
    //WUKONG
    //float backup_size;
    void berserk();
    void berserk_end();
    bool berserked;
    //SHASENG
    //BAJIE
    //1
    float backup_armor;
    void shield();
    //2
    void taunt();
    //3
    void bloodsuck();


    //running renderer
    bool running_status;
    float render_clock;
  
    //vector<Agent*>* Monsters;
    int attack_strength;
    double attack_range;

    //relative location;
    Zeni::Point2f rel_loc;
    float size;
  };
}

#endif
