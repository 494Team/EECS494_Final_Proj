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
  extern const float RENDER_ATTACK_TIME;

  // Wanderer constants
  extern const float WANDERER_RADIUS;
  extern const float WANDERER_SPEED;
  extern const float WANDERER_DAMAGE;
  extern const float WANDERER_HEALTH;
  extern const float WANDERER_ATTACK_GAP;
  extern const float WANDERER_ATTACK_RADIUS;
  extern const float WANDERER_MIN_DIST;

  // Whisper constants
  extern const float WHISPER_RADIUS;
  extern const float WHISPER_SPEED;
  extern const float WHISPER_DAMAGE;
  extern const float WHISPER_HEALTH;
  extern const float WHISPER_ATTACK_GAP;
  extern const float WHISPER_SPELL_RADIUS;
  extern const float WHISPER_MIN_DIST;
  extern const float WHISPER_MAX_DIST;

  // attack effect time
  extern const float HITBACK_TIME;
  extern const float SLOWDOWN_TIME;
  extern const float FREEZE_TIME;

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

  enum attack_effect {
    HITBACK,
    SLOWDOWN,
    FREEZE
  };

  enum kKey_type {
    MENU=1,
    L1, L2, L3, L4,
    HORI1, HORI2, HORI3, HORI4,
    VERT1, VERT2, VERT3, VERT4,
    A1, A2, A3, A4,
    B1, B2, B3, B4,
    X1, X2, X3, X4,
    Y1, Y2, Y3, Y4,
  };

  enum kPlayer_type {
    SANZANG,
    WUKONG,
    SHASENG,
    BAJIE
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
}

#endif