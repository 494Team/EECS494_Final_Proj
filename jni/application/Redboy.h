#ifndef REDBOY_H
#define REDBOY_H

#include "Boss.h"

namespace Flame {
  class Redboy : public Boss {
  public:
    Redboy(const Zeni::Point2f &location_ = Zeni::Point2f());

    // normal attack
    void attack();

    // skill: fire charge
    void skill1();

    // skill: ring of fire
    void skill2();

    // skill: fire spikes
    void skill3();

    void update(float time = 0.0f);

    void render();

  private:
    Zeni::Point2f fire_charge_tar_loc;
    float damage;
    float attack_radius;
  };
}

#endif