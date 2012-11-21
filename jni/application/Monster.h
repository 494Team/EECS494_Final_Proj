#ifndef MONSTER_H
#define MONSTER_H

#include <vector>
#include <map>
#include "Player.h"
#include "Agent.h"

namespace Flame {
  class Monster : public Agent {
  public:
    Monster(
      const std::vector<Flame::Player *> &players_,
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f())
    : Agent(health_, speed_, radius_, location_),
      players(players_)
    {
      for (int i = 0; i < int(players.size()); ++i) {
        hatred[players[i]] = 0.0f;
      }
    }

    // increase hatred for a specific player
    void increase_hatred(const float &hate_, Player* target);

    // get hit from player, cause hatred increase
    void get_hit(const float &damage, Player* source);

    // find player with the highest hatred
    Flame::Player * highest_hatred();

    // target at a player
    void target_at(Player* target);
      
  private:
    std::vector<Player *> players;
    std::map<Flame::Player *, float> hatred;
  };
}

#endif