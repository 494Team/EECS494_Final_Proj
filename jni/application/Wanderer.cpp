#include "Wanderer.h"
#include "zenilib.h"

using namespace Flame;

Wanderer::Wanderer(
  const Zeni::Point2f &location_)
: Monster(WANDERER_HEALTH, WANDERER_SPEED, WANDERER_RADIUS, WANDERER_ATTACK_GAP, location_),
  damage(WANDERER_DAMAGE),
  attack_radius(WANDERER_ATTACK_RADIUS)
{}

void Wanderer::attack() {
  //Collision_sector attack_sector(get_location(), get_current_orientation(), attack_radius);
  Monster::attack();

  // TODO: add attack sector to spell vector
  //Model_state::get_instance()->get_spell_list_ptr()->push_back(
}

void Wanderer::update(float time) {
  Monster::update(time);
  if (is_freeze()) {
    return;
  }
  if (is_hitback()) {
    set_moving(true);
    make_move(time);
    return;
  }

  // move
  target = highest_hatred();
  set_orientation(target->get_location() - get_location());
  if (is_currently_moving()) {
    make_move(time);
  }

  // attack
  float dist = (get_location() - target->get_location()).magnitude()
    - get_body().get_radius() - target->get_body().get_radius();
  if (dist < WANDERER_MIN_DIST) {
    set_moving(false);
    if (can_attack()) {
      attack();
    } else {
      if (get_current_time() - get_prev_attack_time() > ATTACK_DURATION) {
        is_attacking = false;
      }
    }
  } else {
    set_moving(true);
    is_attacking = false;
  }
}

void Wanderer::render() {
  float scale = Model_state::get_instance()->get_scale();
  Zeni::Point2f ul, lr;
  float radians_ccw;
  get_render_params(get_body().get_radius(), ul, lr, radians_ccw);


  Zeni::render_image("enemy",
    ul,
    lr,
    -radians_ccw,
    1.0f,
    rel_loc
  );
  if (is_attacking) {
    Zeni::render_image("sword_attack",
      ul + scale * Zeni::Vector2f(2.0f * get_body().get_radius(), 0.0f),
      lr + scale * Zeni::Vector2f(2.0f * get_body().get_radius(), 0.0f),
      -radians_ccw,
      1.0f,
      rel_loc
    );
  }

  Agent::render();
}