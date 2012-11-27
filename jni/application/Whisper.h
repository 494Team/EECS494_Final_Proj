#ifndef whisper
#define whisper

#include "Monster.h"
#include "Utility.h"

namespace Flame {
  class Whisper : public Monster {
  public:
    Whisper(const Zeni::Point2f &location_ = Zeni::Point2f());

    // perform attack, i.e. cast spell
    void attack();

    void update(float time = 0.0f);

    void render();

  private:
    float damage;
    float spell_radius;
    bool is_attacking;
  };
}

#endif