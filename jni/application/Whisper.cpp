#include "Whisper.h"
#include "zenilib.h"

using namespace Flame;

Whisper::Whisper(const Zeni::Point2f &location_) 
: Monster(WHISPER_HEALTH, WHISPER_SPEED, WHISPER_RADIUS, WHISPER_ATTACK_GAP, location_),
  damage(WHISPER_DAMAGE),
  spell_radius(WHISPER_SPELL_RADIUS),
  made_decision(false),
  decision_time(1.0f)
{}

void Whisper::attack() {
  Monster::attack();
  Fire_ball* test_spell = new Fire_ball(get_location(), target->get_location() - get_location());
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
    if ((*it)->get_body().intersects(path_obj)) {
      path_clear = false;
      break;
    }
  }
  return path_clear;
}

void Whisper::update(float time) {
  Monster::update(time);
  if (is_freeze()) {
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
      make_move(time);
    } else {
      attack();
      made_decision = false;
    }
  } else {
    if (get_current_time() - get_prev_attack_time() > ATTACK_DURATION && !made_decision) {
      made_decision = true;
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
    }
    make_move(time);
  }
}

void Whisper::render() {
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
  if (!is_hitback() && !is_freeze() && is_attacking) {
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