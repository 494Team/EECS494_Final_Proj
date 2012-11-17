#include "Monster.h"

using namespace std;
using namespace Flame;

void Monster::increase_hatred(const float &hate_, Player* target) {
  hatred[target] += hate_;
}

void Monster::get_hit(const float &damage, Player* source) {
  increase_hatred(damage, source);
  dec_health(damage);
}

Player * Monster::highest_hatred() {
  float max_hatred = -1.0f;
  Player* target_player = NULL;
  for (int i = 0; i < players.size(); ++i) {
    if (hatred[players[i]] > max_hatred) {
      max_hatred = hatred[players[i]];
      target_player = players[i];
    }
  }
  assert(target_player != NULL);
  return target_player;
}

void Monster::target_at(Player* target) {
  Zeni::Point2f target_loc = target->get_location();
  Zeni::Point2f my_loc = get_location();
  Zeni::Vector2f orientation = target_loc - my_loc;
  set_orientation(orientation);
}