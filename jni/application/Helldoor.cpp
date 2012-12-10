#include "Helldoor.h"
#include "zenilib.h"

using namespace Flame;

Helldoor::Helldoor(const Zeni::Point2f &location_)
  : Monster(BULLKING_HELLDOOR_HEALTH, 0.0f, BULLKING_HELLDOOR_RADIUS, 0.0f, 0.0f, location_),
  last_new_time(0.0f)
{}

void Helldoor::update(float time) {
  Monster::update(time);
  if (get_current_time() - last_new_time > BULLKING_HELLDOOR_NEW_GAP) {
    // generate new monster
    last_new_time = get_current_time();
    int i = rand_inst.rand_lt(3);
    if (i == 0) {
      Wanderer* new_wanderer = new Wanderer(get_location());
      Model_state::get_instance()->add_monster(new_wanderer);
    } else {
      Whisper* new_whisper = new Whisper(get_location());
      Model_state::get_instance()->add_monster(new_whisper);
    }
  }
}

void Helldoor::render() {
  Agent::render();
  Zeni::Point2f ul, lr;
  float radians_ccw;
  get_render_params(get_body().get_radius(), ul, lr, radians_ccw);
  Zeni::render_image("helldoor", ul, lr);
}