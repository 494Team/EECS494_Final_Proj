#include <zeni.h>
#include "Collision.h"
#include <cmath>

using namespace Zeni;
using Zeni::Point2f; using Zeni::Vector2f; using Zeni::Global::pi;

namespace Flame {

  Collision_sector::Collision_sector(const Point2f& origin_,
                                     const Vector2f& orientation_,
                                     float radius_) :
    origin(origin_),
    orientation(orientation_),
    radius(radius_)
    {}

  bool Collision_sector::intersect(const Collision::Capsule& rhs) const
  {
    Point2f rhs_origin = Point2f(rhs.get_end_point_a().x, rhs.get_end_point_b().y);
    float rhs_radius = rhs.get_radius();
    Vector2f rhs_origin_direction = rhs_origin - origin;
    Point2f p0 = rhs_origin + rhs_radius * Vector2f(rhs_origin_direction.y, -rhs_origin_direction.x).normalized();
    Point2f p1 = rhs_origin + rhs_radius * Vector2f(-rhs_origin_direction.y, rhs_origin_direction.x).normalized();
    if (orientation.angle_between(p0 - origin) < Global::pi / 4 || orientation.angle_between(p1 - origin) < Global::pi / 4)
      if (radius + rhs_radius > rhs_origin_direction.magnitude())
        return true;
    return false;
  }

}