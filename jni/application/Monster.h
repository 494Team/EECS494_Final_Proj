#ifndef MONSTER_H
#define MONSTER_H

#include <vector>
#include <map>
#include "Player.h"
#include "Agent.h"

namespace Flame {
  class Monster : public Agent {
  public:
    Monster::Monster(
      const std::vector<Flame::Player *> &players_,
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const float &attack_gap_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f());

    // increase hatred for a specific player
    void increase_hatred(const float &hate_, Player* player);

    // get hit from player, cause hatred increase
    void get_hit(const float &damage, Player* source);

    // find player with the highest hatred
    Player * highest_hatred();

    // is previous attack finished?
    bool can_attack();

    float get_current_time() {return current_time;}
    float get_prev_attack_time() {return prev_attack_time;}

    // increase current_time by time. MUST be called at every perform_logic()
    void update_current_time(float time) {current_time += time;}

    void set_prev_attack_time(float prev_attack_time_) {prev_attack_time = prev_attack_time_;}
      
  protected:
    Player* target;
    std::vector<Player *> players;
    std::map<Player *, float> hatred;
  private:
    float current_time;
    float prev_attack_time;
    float attack_gap;
  };
}

#endif