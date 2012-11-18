#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include <Zeni.h>

namespace Flame {
  
  class Moving_object {
  public:
    Moving_object(
      const float &speed_ = 0.0f, 
      const Zeni::Point2f &location_ = Zeni::Point2f())
    : speed(speed_), location(location_), is_moving(false), orientation(Zeni::Vector2f())
    {}

    bool is_currently_moving() const {return is_moving;}
    float get_current_speed() const {return speed;}
    Zeni::Point2f get_current_location() const {return location;}
    Zeni::Point2f get_current_orientation() const {return orientation;}

    // update orientation according to destination and location
    void set_orientation(const Zeni::Vector2f &orientation_) { 
      orientation = orientation_;  
      orientation.normalize();
    }

    // update speed
    void set_speed(const float &speed_) { speed = speed_; }

    // update is_moving
    void set_moving(const bool &is_moving_) { is_moving = is_moving_; }

    // update the location of this Moving_object
    void update_location(const float &time) { location = location + is_moving * orientation * time; }
    
    // alternative way to update the location of this Moving_object
    // being used in Player class
    void set_position(Zeni::Point2f pos) {location = pos;}

  private:
    bool is_moving;
    float speed;
    Zeni::Point2f location;

    // orientation should be unit vector
    Zeni::Vector2f orientation;
  };
}

#endif