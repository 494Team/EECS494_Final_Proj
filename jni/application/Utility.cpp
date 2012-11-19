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

  const float kAxis_constraint = 3000;

  bool Less_than::operator() (const Sim_object * lhs, const Sim_object * rhs)
  {
    if (lhs->get_location().y < rhs->get_location().y)
      return true;
    else if (lhs->get_location().y == rhs->get_location().y)
      return lhs < rhs;
    return false;
  }

}