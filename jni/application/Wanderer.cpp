#include "Wanderer.h"
#include "zenilib.h"

using namespace Flame;

Wanderer::Wanderer(
  const Zeni::Point2f &location_)
: Monster(WANDERER_HEALTH, WANDERER_SPEED, WANDERER_RADIUS, WANDERER_ATTACK_GAP, location_),
  damage(WANDERER_DAMAGE),
  attack_radius(WANDERER_ATTACK_RADIUS),
  is_attacking(false)
{
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

void Wanderer::attack() {
  //Collision_sector attack_sector(get_location(), get_current_orientation(), attack_radius);
  set_prev_attack_time(get_current_time());
  is_attacking = true;

  // TODO: add attack sector to spell vector
  //Model_state::get_instance()->get_spell_list_ptr()->push_back(
}

void Wanderer::update(float time) {
  update_current_time(time);
  update_rel_loc();

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
      if (get_current_time() - get_prev_attack_time() > RENDER_ATTACK_TIME) {
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
  Zeni::Point2f ul = rel_loc 
    - Zeni::Vector2f(get_body().get_radius(), get_body().get_radius()) * scale;
  Zeni::Point2f lr = ul + Zeni::Vector2f(get_body().get_radius() * 2.0f, get_body().get_radius() * 2.0f) * scale;
  float radians_ccw = get_current_orientation().angle_between(Zeni::Vector2f(1.0f, 0.0f));
  if (get_current_orientation().y < 0.0f) {
    radians_ccw = Zeni::Global::pi * 2.0f - radians_ccw;
  }


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
}