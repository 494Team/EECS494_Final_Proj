//
//  Utility.cpp
//  game
//
//  Created by Kyle Kong on 11/16/12.
//
//

#include "Utility.h"
#include "Sim_object.h"

using std::string;
using std::set;

namespace Flame {

  const float kCollision_object_height = 5.f;

  const float kAxis_constraint = 3000;

  const Zeni::Point2f RENDER_CENTER = Zeni::Point2f(400.0f, 300.0f);


  const float INFINITY = 1E+20f;
  const float EPSILON = 1E-10f;

  const float INITIAL_HATRED = 100.0f;

  const float ATTACK_DURATION = 0.5f;

  const float WANDERER_RADIUS = 16.0f;
  const float WANDERER_SPEED = 50.0f;
  const float WANDERER_DAMAGE = 100.0f;
  const float WANDERER_HEALTH = 500.0f;
  const float WANDERER_ATTACK_GAP = 1.0f;
  const float WANDERER_ATTACK_RADIUS = 16.0f;
  const float WANDERER_MIN_DIST = 20.0f;

  const float WHISPER_RADIUS = 16.0f;
  const float WHISPER_SPEED = 40.0f;
  const float WHISPER_DAMAGE = 150.0f;
  const float WHISPER_HEALTH = 300.0f;
  const float WHISPER_ATTACK_GAP = 1.5f;
  const float WHISPER_SPELL_RADIUS = 16.0f;
  const float WHISPER_MIN_DIST = 100.0f;
  const float WHISPER_MAX_DIST = 500.0f;

  bool Less_than::operator() (const Sim_object * lhs, const Sim_object * rhs)
  {
    if (lhs->get_location().y < rhs->get_location().y)
      return true;
    else if (lhs->get_location().y == rhs->get_location().y)
      return lhs < rhs;
    return false;
  }

}
