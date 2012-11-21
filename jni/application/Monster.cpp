#include "Monster.h"

using namespace std;
using namespace Flame;

Monster::Monster(
  const std::vector<Flame::Player *> &players_,
  const float &health_,
  const float &speed_,
  const float &radius_,
  const float &attack_gap_,
  const Zeni::Point2f &location_)
: Agent(health_, speed_, radius_, location_),
  players(players_),
  current_time(0.0f),
  prev_attack_time(0.0f),
  attack_gap(attack_gap_)
{
  for (int i = 0; i < int(players.size()); ++i) {
    hatred[players[i]] = 0.0f;
  }
}

void Monster::increase_hatred(const float &hate_, Player* player) {
  hatred[player] += hate_;
}

void Monster::get_hit(const float &damage, Player* source) {
  increase_hatred(damage, source);
  dec_health(damage);
}

Player * Monster::highest_hatred() {
  float max_hatred = -1.0f;
  Player* target_player = NULL;
  for (int i = 0; i < int(players.size()); ++i) {
    if (hatred[players[i]] > max_hatred) {
      max_hatred = hatred[players[i]];
      target_player = players[i];
    }
  }
  assert(target_player != NULL);
  return target_player;
}

bool Monster::can_attack() {
  if (current_time - prev_attack_time > attack_gap) {
    return true;
  } else {
    return false;
  }
}