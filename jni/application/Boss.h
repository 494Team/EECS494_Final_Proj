#ifndef BOSS_H
#define BOSS_H

#include "Monster.h"
#include "Utility.h"

namespace Flame {
  enum Boss_status {
    ATTACK,
    SKILL1,
    SKILL2,
    SKILL3,
    IDLE
  };

  class Boss : public Monster {
  public:
    Boss(
      float health_, float speed_, float radius_, float attack_gap_,
      float skill_1_cooldown_, float skill_2_cooldown_, float skill_3_cooldown_,
      const Zeni::Point2f &location_ = Zeni::Point2f()
    );

    bool can_use_skill();

    void update(float time);

    void decide_attack_type();

    void use_skill1();
    void use_skill2();
    void use_skill3();
    void use_attack();
    virtual void skill1() = 0;
    virtual void skill2() = 0;
    virtual void skill3() = 0;
    virtual void attack() = 0;

    void get_hit(const float &damage, const std::vector<attack_effect> &effects, Player* attacker);

    static float get_random_cooldown(float cooldown);


  protected:
    Boss_status status;
    float skill_1_cooldown;
    float skill_2_cooldown;
    float skill_3_cooldown;

    float i_skill_1_cooldown;
    float i_skill_2_cooldown;
    float i_skill_3_cooldown;
    
    float last_skill_starts_time;
    float last_skill_ends_time;
    Zeni::Random randomer;
  };

}

#endif BOSS_H