#include <zeni.h>
#include "Collision.h"
#include <cmath>

using namespace Zeni;

using Zeni::Point2f; using Zeni::Vector2f; using Zeni::Global::pi;

namespace Flame {

  Collision_circle::Collision_circle(const Point2f& origin_, float radius_) :
    origin(origin_),
    radius(radius_)
    {}

  bool Collision_circle::collide(const Collision_circle& other) const
  {
    if (!Collision_object::is_collidable() || !other.is_collidable())
      return false;
    return (other.origin - origin).magnitude() > other.radius + radius;
  }

  bool Collision_circle::collide(const Collision_rectangle& other) const
  {
    if (!Collision_object::is_collidable() || !other.is_collidable())
      return false;
    Point2f other_center = other.corner + other.size / 2;
    if (other.corner.x < origin.x && origin.x < other.corner.x + other.size.x)
      return abs(other_center.y - origin.y) < other.size.y / 2 + radius;
    if (other.corner.y < origin.y && origin.y < other.corner.y + other.size.y)
      return abs(other_center.x - origin.x) < other.size.x / 2 + radius;
    if (origin.x <= other.corner.x && origin.y <= other.corner.y)
      return (other.corner - origin).magnitude() < radius;
    if (origin.x <= other.corner.x && other.corner.y <= origin.y)
      return (other.corner + Point2f(0.f, other.size.y) - origin).magnitude() < radius;
    if (other.corner.x <= origin.x && origin.y <= other.corner.y)
      return (other.corner + Point2f(other.size.x, 0.f) - origin).magnitude() < radius;
    return (other.corner + other.size - origin).magnitude() < radius;
  }

  Collision_rectangle::Collision_rectangle(const Point2f& corner_, const Vector2f& size_) :
    corner(corner_),
    size(size_)
    {}

  bool Collision_rectangle::collide(const Collision_circle& other) const
    {return other.collide(*this);}

  bool Collision_rectangle::collide(const Collision_rectangle& other) const
  {
    if (!Collision_object::is_collidable() || !other.is_collidable())
      return false;
    if ((corner.x < other.corner.x && other.corner.x < corner.x + size.x) ||
        (other.corner.x < corner.x && corner.x < other.corner.x + other.size.x)) {
      if (corner.y > other.corner.y)
        return corner.y - other.corner.y < other.size.y;
      else
        return other.corner.y - corner.y < size.y;
    }
    if ((corner.y < other.corner.y && other.corner.y < corner.y + size.y) ||
        (other.corner.y < corner.y && corner.y < other.corner.y + other.size.y)) {
      if (corner.x > other.corner.x)
        return corner.x - other.corner.x < other.size.y;
      else
        return other.corner.x - corner.x < size.x;
    }
    return false;
  }

  Collision_sector::Collision_sector(const Point2f& origin_,
                                     const Vector2f& orientation_,
                                     float radius_) :
    origin(origin_),
    orientation(orientation_),
    radius(radius_)
    {}

  bool Collision_sector::collide(const Collision_circle& other) const
  {
    if (orientation.angle_between(other.origin - origin) < pi / 4)
      if ((other.origin - origin).magnitude() < other.radius + radius)
        return true;
    return false;
  }

}