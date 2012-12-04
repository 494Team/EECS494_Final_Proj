#include "Boss.h"

using namespace Flame;

Boss::Boss(
  float health_, float speed_, float radius_, float attack_gap_,
  float skill_1_cooldown_, float skill_2_cooldown_, float skill_3_cooldown_,
  const Zeni::Point2f &location_
)
  : Monster(health_, speed_, radius_, attack_gap_, location_),
  i_skill_1_cooldown(skill_1_cooldown_), i_skill_2_cooldown(skill_2_cooldown_), i_skill_3_cooldown(skill_3_cooldown_),
  skill_1_cooldown(skill_1_cooldown_), skill_2_cooldown(skill_2_cooldown_), skill_3_cooldown(skill_3_cooldown_),
  status(IDLE),
  last_skill_ends_time(0.0f)
{}

bool Boss::can_use_skill() {
  if (get_current_time() - last_skill_ends_time > attack_gap) {
    return true;
  } else {
    return false;
  }
}

void Boss::update(float time) {
  Monster::update(time);
  skill_1_cooldown -= time;
  skill_2_cooldown -= time;
  skill_3_cooldown -= time;
  switch (status) {
    case ATTACK: 
    case SKILL1:
    case SKILL2:
    case SKILL3:
      last_skill_ends_time = get_current_time();
      break;
    case IDLE:
      break;
  }
}

void Boss::decide_attack_type() {

  if (can_use_skill()) {
    if (skill_1_cooldown <= 0.0f) {
      status = SKILL1;
      use_skill1();
    } else if (skill_2_cooldown <= 0.0f) {
      status = SKILL2;
      use_skill2();
    } else if (get_current_health() / get_initial_health() < 0.5f && skill_3_cooldown <= 0.0f) {
      status = SKILL3;
      use_skill3();
    } else {
      status = ATTACK;
      use_attack();
    }
  } else {
    status = IDLE;
  }
}

void Boss::use_skill1() {
  skill_1_cooldown = Boss::get_random_cooldown(i_skill_1_cooldown);
  last_skill_starts_time = get_current_time();
  skill1();
}

void Boss::use_skill2() {
  skill_2_cooldown = Boss::get_random_cooldown(i_skill_2_cooldown);
  last_skill_starts_time = get_current_time();
  skill2();
}

void Boss::use_skill3() {
  skill_3_cooldown = Boss::get_random_cooldown(i_skill_3_cooldown);
  last_skill_starts_time = get_current_time();
  skill3();
}

void Boss::use_attack() {
  Monster::attack();
  last_skill_starts_time = get_current_time();
  attack();
}

float Boss::get_random_cooldown(float cooldown) {
  Zeni::Random loc_rand;
  return (cooldown + loc_rand.frand_lte() * 0.5f * cooldown);
}

void Boss::get_hit(const float &damage, const std::vector<attack_effect> &effects, Player* attacker, Zeni::Vector2f coming_ori) {
  dec_health(damage);
  increase_hatred(damage, attacker);
}