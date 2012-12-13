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
  class Helldoor;
  #define PLAYER_SPELL1_CD 1.0f
  #define PLAYER_SPELL2_CD 1.0f
  #define PLAYER_SPELL3_CD 1.0f
  #define HURT_SHOWING_INTERVAL 0.2f

  const float kPlayer_action_gap = 0.5f;
  const float kPlayer_init_speed = 250.0f;
  const float kRegen_CD = 0.01f;
  const float kInit_attack_buff = 1.0f;

  const float kRun_render_gap = 0.2f;

  /************** spells ******************/
  const float kAttack_show_time = 0.2f;
  const float kPlayer_attack_range = 25.0f;
  const float kPlayer_attack_strengh = 500.0f;
  //SANZANG
  const float kDisintegrate_CD = 0.0f;
  const float kDisintegrate_dam = 5.0f;
  //const float kDisintegrate_mp_initial_cost = 2.0f;
  const float kDisintegrate_mp_cost = 2.0f;
  const float kHealing_CD = 2.0f;
  const float kHealing_amount = 500.0f; // -1000.0f
  const float kHealing_mp_cost = 300.0f;
  const float kDing_CD = 8.0f;
  const float kDing_dam = 0.8f;
  const float kDing_mp_cost = 500.0f;
  //WUKONG
  const float kCudgel_fury_dam = 2.5f;
  const float kCudgel_fury_mp_cost = 200.0f;
  const float kCharge_CD = 5.0f;
  const float kCharge_last = 0.5f;
  const float kCharge_speed = 200.0f;
  const float kCharge_mp_cost = 500.0f;
  const float kCharge_attack_damage = 400.0f;
  const float kCharge_attack_last = 0.2f;
  const float kCharge_attack_CD = 0.15f;
  const int kCharge_attack_max = 3;
  const float kBerserk_CD = 15.0f;
  const float kBerserk_last = 10.0f;
  const float kBerserk_enlarge = 1.35f;
  const float kBerserk_attack_buff = 1.8f;
  const float kBerserk_mp_cost = 400.0f;
  //SHASENG
  const float kArrow_dam = kPlayer_attack_strengh * 1.2f;
  const float kStrafe_CD = 3.0f;
  const float kStrafe_dam = kArrow_dam;
  const float kStrafe_mp_cost = 200.0f;
  const float kTrap_CD = 8.0f;
  const float kTrap_dam = 200.0f;
  const float kTrap_mp_cost = 500.0f;
  const float kMagicarrow_CD = 3.0f;
  const float kMagicarrow_dam = kArrow_dam * 1.2f;
  const float kMagicarrow_mp_cost = 300.0f;
  //BAJIE
  const float kShield_last = 5.0f;
  const float kShield_CD = 8.0f;
  const float kShield_effect = 0.3f;
  const float kShield_mp_cost = 200.0f;

  const float kTaunt_CD = 3.0f;
  const float kTaunt_mp_cost = 200.0f;

  const float kBloodsuck_last = 5.0f;
  const float kBloodsuck_CD = 15.0f;
  const float kBloodsuck_effect = 0.3f;
  const float kBloodsuck_mp_cost = 300.0f;

  //player number buff
  const float kPlayer_number_attack_buff[4] = {1.1f, 0.9f, 0.75f, 0.65f};
  const float kPlayer_number_defense_buff[4] = {0.6f, 0.75f, 0.8f, 0.9f};

  class Player: public Agent {
  public:
    virtual void get_hit(const float &damage, const std::vector<attack_effect> &effects, Player* attacker=NULL, Zeni::Vector2f coming_ori = Zeni::Vector2f()) {
      Agent::get_hit(damage, effects, attacker, coming_ori);
      disable_status_when_being_forced_move();
    }
    Player(
      Chronometer<Time>* game_t,
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f(),
      const kPlayer_type player_type_ = WUKONG);
    ~Player() {
      if (is_disintegrate()) {
        disintegrate_end();
      }
      if (is_cudgel_fury()) {
        cudgel_fury_end();
      }
      if (is_shield()) {
        shield_end();
      }
    }
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
    bool controllable() {
      return ((!is_hitback()) && (!is_charging()) && is_alive());
    }
        
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
    void level_up() {
      skill_point += 3;
    }
    int attack;
    int defense;
    int hpmp_regen;
    int speed;

    bool is_fire_magic_arrow() {
      return fire_magic_arrow;
    }
    void switch_magic_arrow() {
      fire_magic_arrow = !fire_magic_arrow;
    }

    void set_attack_buff(float buff) {
      attack_buff = buff;
    }
    float get_hp() {
      return get_current_health();
    }
    float get_mp() {
      return mp;
    }

    void button_x_release() {
      if (is_disintegrate())
        disintegrate_end();

    }

    void set_controller(int controller_) {
      controller = controller_;
    }
    int get_controller() {
      return controller;
    }
    void set_hp_regen_rate(float hp_regen_rate_) {
      hp_regen_rate = hp_regen_rate_;
    }
    void set_mp_regen_rate(float mp_regen_rate_) {
      mp_regen_rate = mp_regen_rate_;
    }
    bool spell1_mp_enough() {return mp > spell1_mp;}
    bool spell2_mp_enough() {return mp > spell2_mp;}
    bool spell3_mp_enough() {return mp > spell3_mp;}
    bool is_normal_attack() {return normal_attack;}
    bool is_disintegrate() {return (ptype==SANZANG && spell1_active);}
    bool is_cudgel_fury() {return (ptype==WUKONG && spell1_active);}
    bool is_shield() {return (ptype==BAJIE && spell1_active);}
    bool is_charge() {return (ptype==WUKONG && spell2_active);}
    bool is_berserk() {return (ptype==WUKONG && spell3_active);}
    bool is_bloodsuck() {return (ptype== BAJIE && spell3_active);}
    void disintegrate_end_when_die() {disintegrate_end();}
    void hitback_end_action() {
      if (ctrl.l)
        ctrl.l = false;
    }
    void disable_status_when_being_forced_move() {
      if (is_disintegrate())
        disintegrate_end();
      if (is_cudgel_fury())
        cudgel_fury_end();
      if (is_charge())
        charge_end();
      if (ctrl.l)
        ctrl.l = false;
    }
    void end_action() {
      if (is_disintegrate())
        disintegrate_end();
      if (is_cudgel_fury())
        cudgel_fury_end();
      if (is_shield())
        shield_end();
      if (is_charge())
        charge_end();
      if (is_berserk())
        berserk_end();
      if (is_bloodsuck())
        bloodsuck_end();
      if (ctrl.l)
        ctrl.l = false;
    }
  private:
    Difficulty diff;
    int controller; //range: [0, 3]
    float mp;
    float hp_regen_rate;
    float mp_regen_rate;
    float last_regen;
    float attack_buff; //range: [1, +INF]; initial:1.0f
    int skill_point;

    bool is_moving() {
      return (!is_disintegrate() && !is_hitback() && !is_charging() && abs(ctrl.move_hori) + abs(ctrl.move_vert) > 0.3f);
    }
    bool cost_mp(const float cost) {
      bool result = true;
      float backup_mp = mp;
      mp -= cost;
      if (mp < 0.0f) {
        mp = backup_mp;
        result = false;
      }
      return result;
    }
    void hpmp_regenerate() {
      dec_health(-hp_regen_rate);
      mp += mp_regen_rate;
      if (mp > kMp_max) {
        mp = kMp_max;
      }
    }
    Chronometer<Time>* game_time;
    std::vector<Monster *> * monster_list_ptr;
    void static_move(float time, bool force_move = false);

    bool render_player;

    float orient_vec_to_radians(Vector2f vec) {
      float radians = atan2(vec.i, vec.j);
      return radians;
    }

    //attack
    bool normal_attack;
    //Zeni::Time_HQ last_htime;
    float last_htime;

    //spell status
    float spell1_CD;
    float spell2_CD;
    float spell3_CD;
    float spell1_mp;
    float spell2_mp;
    float spell3_mp;

    bool spell1_active;
    bool spell2_active;
    bool spell3_active;
    
    //timer_begin
    float last_spell1;
    float last_spell2;
    float last_spell3;
    //timer_end

    //status judgement
    bool doing_action() {
      bool result = is_normal_attack() ||
                    is_disintegrate() ||
                    is_cudgel_fury() ||
                    is_charge();
      return result;
    }

    //SANZANG
    void disintegrate_begin();
    void disintegrate_end();
    Spell* disintegrate_ptr;
    //WUKONG
    void charge();
    void charge_update(float time);
    void charge_end();
    float last_charge_attack;
    bool charge_attacking;
    std::vector<Monster *> charged_monsters;
    bool charge_no_hit_before;
    bool is_charge_attacking() { return charge_attacking;}
    //float backup_size;
    void berserk();
    void berserk_end();
    //SHASENG
    bool fire_magic_arrow;
    //BAJIE
    void shield();
    void shield_end();
    void bloodsuck();
    void bloodsuck_end();
    Zeni::Sound_Source *bloodsuck_sfx;
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
