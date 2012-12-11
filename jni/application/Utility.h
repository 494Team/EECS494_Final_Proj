//
//  Utility.h
//  game
//
//  Created by Kyle Kong on 11/16/12.
//
//

#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <set>
#include <Zeni.h>

namespace Flame{
  const int kTemp_revival_max = 5;
  const float kScale_threshold_high = 3.0f;
  const float kScale_threshold_low = 1.0f;

  extern const float kCollision_object_height;

  extern const float kAxis_constrain;

  // render center 
  extern const Zeni::Point2f RENDER_CENTER;

  // float infinity
  extern const float INFINITY;
  // float epsilon
  extern const float EPSILON;

  // the initial hatred for the closest player when a monster see one
  extern const float INITIAL_HATRED;

  // render time for a normal attack
  extern const float ATTACK_DURATION;

  // Wanderer constants
  const float WANDERER_RADIUS = 16.0f;
  const float WANDERER_SPEED = 50.0f;
  const float WANDERER_DAMAGE = 100.0f;
  const float WANDERER_HEALTH = 2500.0f;
  const float WANDERER_ATTACK_GAP = 1.0f;
  const float WANDERER_ATTACK_RADIUS = 20.0f;
  const float WANDERER_MIN_DIST = 5.0f;
  const float WANDERER_VIEW_RANGE = 800.0f;

  // Whisper constants
  const float WHISPER_RADIUS = 16.0f;
  const float WHISPER_SPEED = 40.0f;
  const float WHISPER_DAMAGE = 150.0f;
  const float WHISPER_HEALTH = 1500.0f;
  const float WHISPER_ATTACK_GAP = 1.5f;
  const float WHISPER_SPELL_RADIUS = 16.0f;
  const float WHISPER_MIN_DIST = 100.0f;
  const float WHISPER_MAX_DIST = 500.0f;
  const float WHISPER_VIEW_RANGE = 800.0f;

  // Boss general constants
  const float BOSS_VIEW_RANGE = 2000.0f;

  // Redboy constants
  const float REDBOY_RADIUS = 20.0f;
  const float REDBOY_SPEED = 50.0f;
  const float REDBOY_DAMAGE = 300.0f;
  const float REDBOY_HEALTH = 50000.0f;
  const float REDBOY_ATTACK_GAP = 1.5f;
  const float REDBOY_ATTACK_RADIUS = 32.0f;
  const float REDBOY_MIN_DIST = 10.0f;
  const float REDBOY_FIRE_CHARGE_COOLDOWN = 10.0f;
  const float REDBOY_RING_OF_FIRE_COOLDOWN = 15.0f;
  const float REDBOY_FIRE_SPIKES_COOLDOWN = 20.0f;
  const float REDBOY_FIRE_CHARGE_DAMAGE = 500.0f;
  const float REDBOY_FIRE_CHARGE_SPEED = 200.0f;

  // Bullking constants
  const float BULLKING_RADIUS = 30.0f;
  const float BULLKING_SPEED = 100.0f;
  const float BULLKING_DAMAGE = 500.0f;
  const float BULLKING_HEALTH = 10000.0f;
  const float BULLKING_ATTACK_GAP = 1.5f;
  const float BULLKING_ATTACK_RADIUS = 35.0f;
  const float BULLKING_MIN_DIST = 10.0f;
  const float BULLKING_HELLDOOR_COOLDOWN = 15.0f;
  const float BULLKING_MARCH_COOLDOWN = 20.0f;
  const float BULLKING_EXPLOSION_COOLDOWN = 25.0f;
  const float BULLKING_HELLDOOR_HEALTH = 500.0f; //2500.0f
  const float BULLKING_HELLDOOR_RADIUS = 20.0f;
  const float BULLKING_HELLDOOR_NEW_GAP = 2.0f;
  const float BULLKING_MARCH_DURATION = 10.0f;
  const float BULLKING_MARCH_NEW_GAP = 0.2f;
  const float BULLKING_EXPLOSION_DURATION = 12.0f;
  const float BULLKING_EXPLOSION_NEW_GAP = 0.6f;
  const Zeni::Vector2f BULLKING_EXPLOSION_INITIAL_SIZE = Zeni::Vector2f(10.0f, 10.0f);
  const Zeni::Vector2f BULLKING_EXPLOSION_RESIZE_SPEED = Zeni::Vector2f(70.0f, 70.0f);
  const Zeni::Vector2f BULLKING_EXPLOSION_MAX_SIZE = Zeni::Vector2f(100.0f, 100.0f);
  const float BULLKING_EXPLOSION_LIFETIME = 4.0f;
  const float BULLKING_EXPLOSION_DAMAGE_RENDER_TIME = 0.2f;
  const float BULLKING_EXPLOSION_DAMAGE = 800.0f;

  // attack effect
  const float NORMAL_HITBACK_TIME = 0.1f;
  const float BOSS_CHARGE_HITBACK_TIME = 0.5f;
  const float SLOWDOWN_TIME = 2.0f;
  const float FREEZE_TIME = 2.0f;
  const float TAUNT_EFFECT_RENDER_TIME = 1.0f;
  const Zeni::Color SLOWDOWN_COLOR = Zeni::Color(1.0f, 0.3f, 0.6f, 1.0f);
  
  //spells written in/related to Player.cpp
  const float kCudgel_fury_CD = 5.0f;
  const float kCudgelfury_last = 3.0f;
  const float kCudgelfury_render_gap = 0.02f;
  const int kCudgelfury_render_max = 3;
  const float kCudgelfury_damage = 2.0f;

  //player abilities
  const int kShop_cursor_max = 7;
  const int kAttack_max = 5;
  const int kDefense_max = 5;
  const int kHp_regen_max = 5;
  const int kSpeed_max = 5;

  const float kAttack_maxbuff = 1.0f;
  const float kDefense_maxbuff = 0.5f;
  const float kHp_regen_maxbuff = 0.4f;
  const float kMp_regen_maxbuff = 0.8f;
  const float kSpeed_maxbuff = 150.0f;

  //others
  const float kVert_move_threshold = 0.95f;
  const int kPlayer_num_max = 4;
  const float kHpbar_width = 3.0f;
  const float kHighlight_move_CD = 0.2f;
  const float kShow_alldie_time = 3.0f;

  //player setting
  const float kHp_max = 1000.0f;
  const float kMp_max = 1000.0f;
  
  //choosing character page "Preparation State"
  const int kCursor_max = 3;
  const int kCursor_min = 0;
  const float kCursor_move_CD = 0.2f;
  const Zeni::Color kCannotmove_color(1.0f, 0.3f, 0.3f, 0.3f);

  // forward declaration
  class Sim_object;
  class Player;
  class Monster;
  class Spell;

  struct Less_than {
    bool operator() (const Sim_object * lhs, const Sim_object * rhs);
  };
  typedef std::set<Sim_object *, Less_than> Render_list_t;

  struct Error {
    Error(const std::string& msg_) :
      msg(msg_)
      {}

    std::string msg;
  };
  
  enum Dialog_state {
    NO_DIALOG,
    LVL1_1,
    LVL2_1,
    LVL3_1
  };

  enum attack_effect {
    HITBACK,
    SLOWDOWN,
    FREEZE,
    GET_WUKONG_CHARGE,
    TAUNT
  };

  enum kKey_type {
    MENU=1, BACK,
    L1, L2, L3, L4,
    HORI1, HORI2, HORI3, HORI4,
    VERT1, VERT2, VERT3, VERT4,
    A1, A2, A3, A4,
    B1, B2, B3, B4,
    X1, X2, X3, X4,
    Y1, Y2, Y3, Y4,
    CONFIRM1, CONFIRM2, CONFIRM3, CONFIRM4,
    JOIN1, JOIN2, JOIN3, JOIN4
  };

  enum kPlayer_type {
    SANZANG = 0,
    WUKONG,
    SHASENG,
    BAJIE,
    NONE
  };

  struct Control {
    float move_hori;
    float move_vert;
    bool l;
    Control()
    : move_hori(0.0f),
      move_vert(0.0f),
      l(false)
    {}
  };

  const int kMove_left = 1;
  const int kMove_right = 2;
  const int kMove_up = 3;
  const int kMove_down = 4;
}

#endif