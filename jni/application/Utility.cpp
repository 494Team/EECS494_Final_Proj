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

  const float MIN_DIST_MONSTER_PLAYER = 5.0f;

  const float INFINITY = 1E+20f;

  const float INITIAL_HATRED = 100.0f;

  const float WANDERER_RADIUS = 10.0f;
  const float WANDERER_SPEED = 20.0f;
  const float WANDERER_DAMAGE = 100.0f;
  const float WANDERER_HEALTH = 500.0f;
  const float WANDERER_ATTACK_GAP = 0.5f;
  const float WANDERER_ATTACK_RADIUS = 18.0f;

  bool Less_than::operator() (const Sim_object * lhs, const Sim_object * rhs)
  {
    if (lhs->get_location().y < rhs->get_location().y)
      return true;
    else if (lhs->get_location().y == rhs->get_location().y)
      return lhs < rhs;
    return false;
  }

}
