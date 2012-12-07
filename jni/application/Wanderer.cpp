#include "Wanderer.h"
#include "zenilib.h"

using namespace Flame;

Wanderer::Wanderer(
  const Zeni::Point2f &location_)
: Monster(WANDERER_HEALTH, WANDERER_SPEED, WANDERER_RADIUS, WANDERER_ATTACK_GAP, WANDERER_VIEW_RANGE, location_),
  damage(WANDERER_DAMAGE),
  attack_radius(WANDERER_ATTACK_RADIUS),
  decision_time(1.0f)
{}

void Wanderer::attack() {
  //Collision_sector attack_sector(get_location(), get_current_orientation(), attack_radius);
  Monster::attack();

  // TODO: add attack sector to spell vector
  //Model_state::get_instance()->get_spell_list_ptr()->push_back(
}

void Wanderer::update(float time) {
  Monster::update(time);
  if (is_get_wukong_charge() || is_freeze()) {
    is_attacking = false;
    return;
  }
  if (is_hitback()) {
    set_moving(true);
    make_move(time);
    return;
  }

  // move
  target = highest_hatred();
  if (target == NULL) {
    is_attacking = false;
    decision_time += time;
    if (decision_time > 0.5f) {
      decision_time = 0.0f;
    } else {
      make_move(time);
      return;
    }
    if (!is_currently_moving()) {
      set_moving(true);
      set_orientation(Zeni::Vector2f(rand_inst.frand_lte() * 2.0f - 1.0f, rand_inst.frand_lte() * 2.0f - 1.0f));
    } else {
      set_moving(false);
    }
    return;
  }

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


  update_render_suffix();
  if (!is_currently_moving())
    render_suffix = "0";
  if (!is_hitback() && !is_freeze() && is_attacking) {
    Zeni::render_image("sword_attack",
      ul + scale * Zeni::Vector2f(2.0f * get_body().get_radius(), 0.0f),
      lr + scale * Zeni::Vector2f(2.0f * get_body().get_radius(), 0.0f),
      -radians_ccw,
      1.0f,
      rel_loc
    );
    render_suffix = "_attack";
  }
  if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
    Zeni::render_image("wanderer_right" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
  } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
    Zeni::render_image("wanderer_front" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
  } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
    Zeni::render_image("wanderer_left" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
  } else {
    Zeni::render_image("wanderer_back" + render_suffix, ul, lr, 0, 1.0f, rel_loc);
  }

  Agent::render();
}