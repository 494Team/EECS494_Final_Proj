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

extern const float kAxis_constrain;

struct Error {
  Error(const std::string& msg_) :
    msg(msg_)
    {}

  std::string msg;
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
namespace Flame{
struct Control {
  float move_hori;
  float move_vert;
  bool l;
  
  Control()
  : move_hori(0.0f),
    move_vert(0.0f),
    l(false)
  {
  }
};
}

#endif