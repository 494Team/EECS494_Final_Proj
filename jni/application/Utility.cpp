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
  const float EPSILON = 0.0000001f;

  const float INITIAL_HATRED = 500.0f;

  const float ATTACK_DURATION = 0.5f;

  bool Less_than::operator() (const Sim_object * lhs, const Sim_object * rhs)
  {
    if (lhs->get_location().y < rhs->get_location().y)
      return true;
    else if (lhs->get_location().y == rhs->get_location().y)
      return lhs < rhs;
    return false;
  }

}
