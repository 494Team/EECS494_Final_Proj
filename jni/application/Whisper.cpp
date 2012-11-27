#include "Whisper.h"
#include "zenilib.h"

using namespace Flame;

Whisper::Whisper(const Zeni::Point2f &location_) 
: Monster(WHISPER_HEALTH, WHISPER_SPEED, WHISPER_RADIUS, WHISPER_ATTACK_GAP, location_),
  damage(WHISPER_DAMAGE),
  spell_radius(WHISPER_SPELL_RADIUS),
  is_attacking(false) {

  float min_dist = INFINITY;
  int idx = 0;
  for (int i = 0; i < players.size(); ++i) {
    float dist = (players[i]->get_location() - get_location()).magnitude();
    if (dist < min_dist) {
      min_dist = dist;
      idx = i;
    }
  }
  hatred[players[idx]] += INITIAL_HATRED;
  set_moving(true);
}

void Whisper::attack() {
  set_prev_attack_time(get_current_time());
  is_attacking = true;
}

void Whisper::update(float time) {
  update_current_time(time);
  update_rel_loc();

  // move
  target = highest_hatred();
  // check if able to attack
  Zeni::Vector3f spell_path_para = Zeni::Point3f(target->get_location().x, target->get_location().y, 0.0f)
    - Zeni::Point3f(get_location().x, get_location().y, 0.0f);
  Zeni::Vector3f spell_path_normal = Zeni::Vector3f(0.0f, 0.0f, 1.0f) % spell_path_para;
  spell_path_normal.normalize();
  spell_path_normal *= 2.0f * spell_radius;
  Zeni::Point3f spell_path_point = Zeni::Point3f(get_location().x, get_location().y, 0.0f) - 0.5f * spell_path_normal;
  Zeni::Collision::Parallelepiped 
    spell_path(spell_path_point, spell_path_para, spell_path_normal, Zeni::Vector3f(0.0f, 0.0f, kCollision_object_height));
  if (spell_path.intersects(
}

void Whisper::render() {

}