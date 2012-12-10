#ifndef HELLDOOR_H
#define HELLDOOR_H

#include "Monster.h"
#include "Wanderer.h"
#include "Whisper.h"

namespace Flame {
  class Helldoor : public Monster {
  public:
    Helldoor(const Zeni::Point2f &location_ = Zeni::Point2f());

    void update(float time = 0.0f);
    void render();
  private:
    float last_new_time;
    Random rand_inst;
  };
}

#endif