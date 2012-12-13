#include "Whisper.h"
#include "zenilib.h"

using namespace Flame;

Whisper::Whisper(const Zeni::Point2f &location_) 
: Monster(WHISPER_HEALTH, WHISPER_SPEED, WHISPER_RADIUS, WHISPER_ATTACK_GAP, WHISPER_VIEW_RANGE, location_),
  damage(WHISPER_DAMAGE),
  spell_radius(WHISPER_SPELL_RADIUS),
  decision_time(1.0f)
{}

void Whisper::attack() {
  Monster::attack();
  Fire_ball* test_spell = new Fire_ball(get_location(), target->get_location() - get_location(), damage);
  Model_state::get_instance()->add_spell(test_spell);
}

void Whisper_green::attack() {
  Monster::attack();
  Fire_ball* test_spell = new Fire_ball_green(get_location(), target->get_location() - get_location(), WHISPER_GREEN_DAMAGE);
  Model_state::get_instance()->add_spell(test_spell);
}

void Whisper_violet::attack() {
  Monster::attack();
  Ice_ball* test_spell = new Ice_ball(get_location(), target->get_location() - get_location(), WHISPER_VIOLET_DAMAGE);
  Model_state::get_instance()->add_spell(test_spell);
}

Zeni::Collision::Parallelepiped Whisper::create_path(const Zeni::Point3f &point1, const Zeni::Point3f &point2, const float radius) {
  Zeni::Vector3f path_para = point2 - point1;
  Zeni::Vector3f path_normal = Zeni::Vector3f(0.0f, 0.0f, 1.0f) % path_para;
  path_normal.normalize();
  path_normal *= 2.0f * radius;
  Zeni::Point3f path_point = point1 - 0.5f * path_normal;
  return Zeni::Collision::Parallelepiped(
    path_point, path_para, path_normal, Zeni::Vector3f(0.0f, 0.0f, kCollision_object_height)
  );
}

bool Whisper::is_path_clear(const Zeni::Collision::Parallelepiped &path_obj) {
  bool path_clear = true;
  std::vector<Map *> * map_obj_list = Model_state::get_instance()->get_map_obj_list_ptr();
  for (std::vector<Map *>::iterator it = map_obj_list->begin(); it != map_obj_list->end(); ++it) {
    if (!(*it)->walk_thru() && (*it)->get_body().intersects(path_obj)) {
      path_clear = false;
      break;
    }
  }
  return path_clear;
}

void Whisper::update(float time) {
  Monster::update(time);
  if (is_get_wukong_charge() || is_freeze() || (is_slowdown() && effect_timers[SLOWDOWN] > 0.75f * SLOWDOWN_TIME)) {
    is_attacking = false;
    set_moving(false);
    return;
  }
  if (is_hitback()) {
    set_moving(true);
    make_move(time);
    return;
  }

  decision_time += time;
  if (decision_time > 0.5f) {
    decision_time = 0.0f;
  } else {
    make_move(time);
    return;
  }

  target = highest_hatred();
  if (target == NULL) {
    is_attacking = false;
    if (!is_currently_moving()) {
      set_moving(true);
      set_orientation(Zeni::Vector2f(rand_inst.frand_lte() * 2.0f - 1.0f, rand_inst.frand_lte() * 2.0f - 1.0f));
    } else {
      set_moving(false);
    }
    make_move(time);
    return;
  }

  if (can_attack()) {
    Zeni::Point3f my_loc = Zeni::Point3f(get_location().x, get_location().y, 0.0f);
    Zeni::Point3f target_loc = Zeni::Point3f(target->get_location().x, target->get_location().y, 0.0f);

    // check if able to attack
    Zeni::Collision::Parallelepiped spell_path = create_path(my_loc, target_loc, spell_radius);
    bool attack_path_clear = is_path_clear(spell_path);

    if (!attack_path_clear) {
      Zeni::Collision::Parallelepiped temp_spell_path = create_path(Zeni::Point3f(target_loc.x, my_loc.y, 0.0f), target_loc, spell_radius);
      Zeni::Collision::Parallelepiped temp_walk_path = create_path(my_loc, Zeni::Point3f(target_loc.x, my_loc.y, 0.0f), get_body().get_radius());
      if (is_path_clear(temp_spell_path) && is_path_clear(temp_walk_path)) {
        Zeni::Vector3f ori_3 = Zeni::Point3f(target_loc.x, my_loc.y, 0.0f) - my_loc;
        set_orientation(Zeni::Vector2f(ori_3.x, ori_3.y));
      } else {
        temp_spell_path = create_path(Zeni::Point3f(my_loc.x, target_loc.y, 0.0f), target_loc, spell_radius);
        temp_walk_path = create_path(my_loc, Zeni::Point3f(my_loc.x, target_loc.y, 0.0f), get_body().get_radius());
        if (is_path_clear(temp_spell_path) && is_path_clear(temp_walk_path)) {
          Zeni::Vector3f ori_3 = Zeni::Point3f(my_loc.x, target_loc.y, 0.0f) - my_loc;
          set_orientation(Zeni::Vector2f(ori_3.x, ori_3.y));
        } else {
          // make random move
          set_orientation(Zeni::Vector2f(rand_inst.frand_lte() * 2.0f - 1.0f, rand_inst.frand_lte() * 2.0f - 1.0f));
        }
      }
      set_moving(true);
      make_move(time);
    } else {
      Random ra;
      if(ra.rand() % 2 == 0){
        set_orientation(target->get_location() - get_location());
        set_moving(false);
        attack();
      }
    }
  } else {
    if (get_current_time() - get_prev_attack_time() > ATTACK_DURATION) {
        set_moving(true);
        is_attacking = false;
        // if can not yet do another attack, move around
        Player* nearest_p = nearest_player();
        Zeni::Point2f nearest_p_loc = nearest_p->get_location();
        if ((nearest_p_loc - get_location()).magnitude() < WHISPER_MIN_DIST) {
          // too close! move back
          set_orientation(get_location() - nearest_p_loc);
        } else if ((target->get_location() - get_location()).magnitude() > WHISPER_MAX_DIST) {
          // too far to target! go forward
          set_orientation(target->get_location() - get_location());
        } else {
          // move left/right
          Zeni::Vector2f tar_ori_2 = target->get_location() - get_location();
          Zeni::Vector3f tar_ori_3 = Zeni::Vector3f(tar_ori_2.x, tar_ori_2.y, 0.0f);
          Zeni::Vector3f left_ori_3 = Zeni::Vector3f(0.0f, 0.0f, 1.0f) % tar_ori_3;
          Zeni::Vector2f left_ori_2 = Zeni::Vector2f(left_ori_3.x, left_ori_3.y);
          Zeni::Vector2f ori = left_ori_2;
          if (rand_inst.frand_lte() < 0.5f) {
            ori = -ori;
          }
          set_orientation(ori);
        }
      make_move(time);
    }
    //do nothing
  }
}

void Whisper::render() {
  Agent::render();
  float scale = Model_state::get_instance()->get_scale();
  Zeni::Point2f ul, lr;
  float radians_ccw;
  get_render_params(get_body().get_radius(), ul, lr, radians_ccw);

  update_render_suffix();
  if (!is_currently_moving())
    render_suffix = "0";
  if (!is_hitback() && !is_freeze() && is_attacking) {
    render_suffix = "_attack";
  }
  Zeni::Color color_filter;
  if (is_slowdown()) {
    color_filter = SLOWDOWN_COLOR;
  }
  if (is_freeze()){
    m_set.start();
    Zeni::render_image("ding_trap", ul, lr, false, color_filter);
  }
  if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
    Zeni::render_image("whisper_right" + render_suffix, ul, lr, false, color_filter);
  } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
    Zeni::render_image("whisper_front" + render_suffix, ul, lr, false, color_filter);
  } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
    Zeni::render_image("whisper_left" + render_suffix, ul, lr, false, color_filter);
  } else {
    Zeni::render_image("whisper_back" + render_suffix, ul, lr, false, color_filter);
  }
  if (is_slowdown() && effect_timers[SLOWDOWN] > SLOWDOWN_TIME * 0.75f) {
    Zeni::render_image("slowdown_effect", ul, lr);
  }
  if(is_freeze()){
    if(m_set.seconds()<0.5f)
      Zeni::render_image("ding_glow0", ul, lr);
    else if (m_set.seconds()<1.f)
      Zeni::render_image("ding_glow1", ul, lr);
    else{
      m_set.reset();
      m_set.start();
      Zeni::render_image("ding_glow0", ul, lr);
    }
  }
  if (is_taunt()) {
    float taunt_render_radius = get_radius() * 0.6f;
    Zeni::Point2f taunt_pos = rel_loc + scale * (get_radius() + taunt_render_radius) * Zeni::Vector2f(0.0f, -1.0f);
    Zeni::Point2f taunt_ul = taunt_pos - scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::Point2f taunt_lr = taunt_pos + scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::render_image("taunt_effect", taunt_ul, taunt_lr);
  }

  if (is_get_wukong_charge()) {
    render_get_wukong_charge_effect();
  }
}

void Whisper_green::render() {
  Agent::render();
  float scale = Model_state::get_instance()->get_scale();
  Zeni::Point2f ul, lr;
  float radians_ccw;
  get_render_params(get_body().get_radius(), ul, lr, radians_ccw);

  update_render_suffix();
  if (!is_currently_moving())
    render_suffix = "0";
  if (!is_hitback() && !is_freeze() && is_attacking) {
    render_suffix = "_attack";
  }
  Zeni::Color color_filter;
  if (is_slowdown()) {
    color_filter = SLOWDOWN_COLOR;
  }
  if (is_freeze()){
    m_set.start();
    Zeni::render_image("ding_trap", ul, lr, false, color_filter);
  }
  if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
    Zeni::render_image("whisper_green_right" + render_suffix, ul, lr, false, color_filter);
  } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
    Zeni::render_image("whisper_green_front" + render_suffix, ul, lr, false, color_filter);
  } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
    Zeni::render_image("whisper_green_left" + render_suffix, ul, lr, false, color_filter);
  } else {
    Zeni::render_image("whisper_green_back" + render_suffix, ul, lr, false, color_filter);
  }
  if (is_slowdown() && effect_timers[SLOWDOWN] > SLOWDOWN_TIME * 0.75f) {
    Zeni::render_image("slowdown_effect", ul, lr);
  }
  if(is_freeze()){
    if(m_set.seconds()<0.5f)
      Zeni::render_image("ding_glow0", ul, lr);
    else if (m_set.seconds()<1.f)
      Zeni::render_image("ding_glow1", ul, lr);
    else{
      m_set.reset();
      m_set.start();
      Zeni::render_image("ding_glow0", ul, lr);
    }
  }
  if (is_taunt()) {
    float taunt_render_radius = get_radius() * 0.6f;
    Zeni::Point2f taunt_pos = rel_loc + scale * (get_radius() + taunt_render_radius) * Zeni::Vector2f(0.0f, -1.0f);
    Zeni::Point2f taunt_ul = taunt_pos - scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::Point2f taunt_lr = taunt_pos + scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::render_image("taunt_effect", taunt_ul, taunt_lr);
  }

  if (is_get_wukong_charge()) {
    render_get_wukong_charge_effect();
  }
}

void Whisper_violet::render() {
  Agent::render();
  float scale = Model_state::get_instance()->get_scale();
  Zeni::Point2f ul, lr;
  float radians_ccw;
  get_render_params(get_body().get_radius(), ul, lr, radians_ccw);

  update_render_suffix();
  if (!is_currently_moving())
    render_suffix = "0";
  if (!is_hitback() && !is_freeze() && is_attacking) {
    render_suffix = "_attack";
  }
  Zeni::Color color_filter;
  if (is_slowdown()) {
    color_filter = SLOWDOWN_COLOR;
  }
  if (is_freeze()){
    m_set.start();
    Zeni::render_image("ding_trap", ul, lr, false, color_filter);
  }
  if (radians_ccw < Zeni::Global::pi * 0.25f || radians_ccw >= Zeni::Global::pi *1.75f) {
    Zeni::render_image("whisper_violet_right" + render_suffix, ul, lr, false, color_filter);
  } else if (radians_ccw >= Zeni::Global::pi * 0.25f && radians_ccw < Zeni::Global::pi * 0.75f) {
    Zeni::render_image("whisper_violet_front" + render_suffix, ul, lr, false, color_filter);
  } else if (radians_ccw >= Zeni::Global::pi * 0.75f && radians_ccw < Zeni::Global::pi * 1.25f) {
    Zeni::render_image("whisper_violet_left" + render_suffix, ul, lr, false, color_filter);
  } else {
    Zeni::render_image("whisper_violet_back" + render_suffix, ul, lr, false, color_filter);
  }
  if (is_slowdown() && effect_timers[SLOWDOWN] > SLOWDOWN_TIME * 0.75f) {
    Zeni::render_image("slowdown_effect", ul, lr);
  }
  if(is_freeze()){
    if(m_set.seconds()<0.5f)
      Zeni::render_image("ding_glow0", ul, lr);
    else if (m_set.seconds()<1.f)
      Zeni::render_image("ding_glow1", ul, lr);
    else{
      m_set.reset();
      m_set.start();
      Zeni::render_image("ding_glow0", ul, lr);
    }
  }
  if (is_taunt()) {
    float taunt_render_radius = get_radius() * 0.6f;
    Zeni::Point2f taunt_pos = rel_loc + scale * (get_radius() + taunt_render_radius) * Zeni::Vector2f(0.0f, -1.0f);
    Zeni::Point2f taunt_ul = taunt_pos - scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::Point2f taunt_lr = taunt_pos + scale * taunt_render_radius * Zeni::Vector2f(1.0f, 1.0f);
    Zeni::render_image("taunt_effect", taunt_ul, taunt_lr);
  }

  if (is_get_wukong_charge()) {
    render_get_wukong_charge_effect();
  }
}