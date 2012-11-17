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

#endif