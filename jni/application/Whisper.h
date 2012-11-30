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

    Zeni::Collision::Parallelepiped create_path(const Zeni::Point3f &point1, const Zeni::Point3f &point2, const float radius);
    bool is_path_clear(const Zeni::Collision::Parallelepiped &path_obj);

    float damage;
    float spell_radius;
    bool made_decision;
    float decision_time;
    Random rand_inst;
  };
}

#endif