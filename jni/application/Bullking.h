#ifndef BULLKING_H
#define BULLKING_H

#include "Boss.h"
#include "Helldoor.h"

namespace Flame {
  class Bullking : public Boss {
  public:
    Bullking(const Zeni::Point2f &location_ = Zeni::Point2f());

    void attack();

    void skill1();

    void skill2();

    void skill3();

    void update(float time = 0.0f);

    void render();

  private:
    Zeni::Point2f room_ul, room_lr;
    float damage;
    float attack_radius;
    float march_last_new_time;
    float explosion_last_new_time;
  };
}

#endif