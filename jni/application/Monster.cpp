#include "Monster.h"

using namespace std;
using namespace Flame;

Monster::Monster(
  const float &health_,
  const float &speed_,
  const float &radius_,
  const float &attack_gap_,
  const Zeni::Point2f &location_)
: Agent(health_, speed_, radius_, location_),
  players((*Model_state::get_instance()->get_player_list_ptr())),
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

void Monster::update_rel_loc() {
  float scale = Model_state::get_instance()->get_scale();
  rel_loc = (get_location() - Model_state::get_instance()->get_center_location()) * scale + RENDER_CENTER;
}

void Monster::make_move(float time) {
  Zeni::Vector2f ori = get_current_orientation();
  Zeni::Point2f backup_loc = get_location();
  Zeni::Point2f attemp_loc;
  // move x
  attemp_loc = get_location() + time * get_current_speed() * ori.get_i();
  set_position(attemp_loc);
  update_body();
  if (!Model_state::get_instance()->can_move(&get_body())) {
    set_position(backup_loc);
    update_body();
  }
  // move y
  backup_loc = get_location();
  attemp_loc = get_location() + time * get_current_speed() * ori.get_j();
  set_position(attemp_loc);
  update_body();
  if (!Model_state::get_instance()->can_move(&get_body())) {
    set_position(backup_loc);
    update_body();
  }
}