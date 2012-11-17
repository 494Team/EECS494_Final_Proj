#ifndef COLLISION_H
#define COLLISION_H

#include <Zeni.h>

namespace Flame {

  // forward declarations
  class Collision_circle;
  class Collision_rectangle;

  // an abstract base class for all collision classes.
  // cannot be instantiated
  class Collision_object {
  public:
    // by default, enable collision
    Collision_object() :
      collidable(true)
      {}

    // reset collidable to be true to enable collision
    void recover_body()
      {collidable = true;}

    // set collidable to be false so that no collision can be dectected
    void destroy_body()
      {collidable = false;}

    // Accessor
    bool is_collidable() const
      {return collidable;}

    // check if two objects collide
    virtual bool collide(const Collision_circle&) const = 0;
    virtual bool collide(const Collision_rectangle&) const = 0;

  private:
    bool collidable;
  };

  // circle-shaped collision object
  class Collision_circle : public Collision_object {
  public:
    Collision_circle(const Zeni::Point2f& origin_ = Zeni::Point2f(0.f, 0.f),
                     float radius_ = 0.f);

    // check if two objects collide
    virtual bool collide(const Collision_circle& other) const;
    virtual bool collide(const Collision_rectangle& other) const;

    // make origin and size accessible to Collision_rectangle
    friend Collision_rectangle;

  private:
    Zeni::Point2f origin;
    float radius;
  };


  // rectangle-shaped collision object
  class Collision_rectangle : public Collision_object {
  public:
    // corner is the upper left corner,
    // size is a Vector2f pointing to the lower right
    Collision_rectangle(const Zeni::Point2f& corner_ = Zeni::Point2f(0.f, 0.f),
                        const Zeni::Vector2f& size_ = Zeni::Vector2f(0.f, 0.f));

    // check if two objects collide
    virtual bool collide(const Collision_circle& other) const;
    virtual bool collide(const Collision_rectangle& other) const;

    // make corner and size accessible to Collision_circle
    friend class Collision_circle;

  private:
    Zeni::Point2f corner;
    Zeni::Vector2f size;
  };

}

#endif
