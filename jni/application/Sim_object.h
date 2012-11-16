#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include <Zeni.h>
#include <string>
#include "Control.h"

namespace Flame {

  class Sim_object {
  public:

    // get the location of this Sim_object
    virtual Zeni::Point2f get_location() = 0;

    // update this Sim_object with time passed passing in, and potentially a control
    virtual void update(float time = 0.0f) = 0;

    virtual void render() = 0;

  };

}

#endif