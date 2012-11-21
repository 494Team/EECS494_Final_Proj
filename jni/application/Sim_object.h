#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include <Zeni.h>
#include <string>

namespace Flame {

  class Sim_object {
  public:
    virtual ~Sim_object() {}

    // get the location of this Sim_object
    virtual Zeni::Point2f get_location()const = 0;

    // update this Sim_object with time passed passing in, and potentially a control
	  virtual void update(float) = 0;

    virtual void render() = 0;

  };

}

#endif