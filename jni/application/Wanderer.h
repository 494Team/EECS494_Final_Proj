#ifndef WANDERER_H
#define WANDERER_H

#include "Monster.h"
#include "Utility.h"

namespace Flame {
  class Wanderer : public Monster {
  public:
    Wanderer(const Zeni::Point2f &location_ = Zeni::Point2f());

    // perform attack
    void attack();

    void update(float time = 0.0f);
    void render();

  private:
    float damage;
    float attack_radius;
  };
}

#endif