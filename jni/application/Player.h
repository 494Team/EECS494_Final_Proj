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
#include "Monster.h"

using namespace Zeni;

namespace Flame {
  #define PLAYER_ATTACK_INTERVAL 0.5f
  //temp
  const float kSpell1_CD = 1.0f;
  const float kSpell2_CD = 1.0f;
  const float kSpell3_CD = 1.0f;

  const float kPlayer_init_speed = 200.0f;

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
  const float kCharge_CD = 2.0f;
  const float kCharge_last = 0.5f;
  const float kCharge_speed = 200.0f;
  const float kCharge_attack_damage = 200.0f;
  const float kCharge_attack_last = 0.3f;
  const float kCharge_attack_CD = 0.3f;
  const int kCharge_attack_max = 3;
  const float kBerserk_CD = 2.0f;
  const float kBerserk_last = 1.0f;
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
        
    bool is_charging() {
      return (ptype == WUKONG && spell2_active);
    }
    
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
      float percent = passed_time > spell2_CD ? 1.0f : passed_time/spell2_CD;
      return percent;
    }
    float get_CD3_percent() {
      float current_time = game_time->seconds();
      float passed_time = current_time - last_spell3;
      float percent = passed_time > spell3_CD ? 1.0f : passed_time/spell3_CD;
      return percent;
    }
    Zeni::Point2f get_rel_loc() {
      return rel_loc;
    }
    
    void cudgel_fury_begin();
    void cudgel_fury_end();

    int get_skill_point() {
      return skill_point;
    }
    void set_skill_point(int point) {
      skill_point = point;
    }
    int attack;
    int defense;
    int strength;
    int speed;

    bool is_fire_magic_arrow() {
      return fire_magic_arrow;
    }
    void switch_magic_arrow() {
      fire_magic_arrow = !fire_magic_arrow;
    }

  private:
    int skill_point;

    Chronometer<Time>* game_time;
    std::vector<Monster *> * monster_list_ptr;
    void static_move(float time);

    bool render_player;

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

    void charge();
    void charge_update(float time);
    void charge_end();
    float last_charge_attack;
    bool charge_attacking;
    bool is_charge_attacking() { return charge_attacking;}

    //float backup_size;
    void berserk();
    void berserk_end();
    //SHASENG
    bool fire_magic_arrow;
    //BAJIE
    //1
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
