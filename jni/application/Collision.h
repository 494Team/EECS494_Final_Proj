#ifndef COLLISION_H
#define COLLISION_H

#include <Zeni.h>
#include <string>
#include "Utility.h"

namespace Flame {

  // Collision_sector is only for attack dectection,
  // thus can only dectect collision with Capsule
  class Collision_sector {
  public:
    Collision_sector(const Zeni::Point2f& origin_ = Zeni::Point2f(),
                     const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                     float radius_ = 0.f);

    // check if intersects a Collision::Capsule
    bool intersect(const Zeni::Collision::Capsule& rhs) const;

  private:
    Zeni::Point2f origin;
    Zeni::Vector2f orientation;
    float radius;
  };
}

#endif
