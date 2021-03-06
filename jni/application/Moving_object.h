#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include <Zeni.h>

namespace Flame {
  
  class Moving_object {
  public:
    Moving_object(
      const float &speed_ = 0.0f, 
      const Zeni::Point2f &location_ = Zeni::Point2f())
    : is_moving(false), speed(speed_), initial_speed(speed_), location(location_), orientation(Zeni::Vector2f())
    {}

    bool is_currently_moving() const {return is_moving;}
    float get_current_speed() const {return speed;}
    Zeni::Point2f get_current_location() const {return location;}
    Zeni::Vector2f get_current_orientation() const {return orientation;}

    // update orientation according to destination and location
    void set_orientation(const Zeni::Vector2f &orientation_) { 
      orientation = orientation_;  
      orientation.normalize();
      if (orientation.magnitude() == 0.0f)
        orientation = Zeni::Vector2f(1.0f, 0.0f);
    }

    // update speed
    void set_speed(const float &speed_) { speed = speed_; }

    // recover to initial speed
    void recover_speed() { speed = initial_speed; }

    // update is_moving
    void set_moving(const bool &is_moving_) { is_moving = is_moving_; }

    // update the location of this Moving_object
    void update_location(const float &time) { location = location + is_moving * speed * orientation * time; }
    
    // alternative way to update the location of this Moving_object
    // being used in Player class
    virtual void set_position(Zeni::Point2f pos) {location = pos;}

  private:
    bool is_moving;
    float speed;
    float initial_speed;
    Zeni::Point2f location;

    // orientation should be unit vector
    Zeni::Vector2f orientation;
  };
}

#endif