#include "Spells.h"
#include "Model_state.h"
#include "Agent.h"
#include "Monster.h"
#include <vector>
#include <ctime>
#include <cstdlib>

using std::vector;
using namespace Zeni;

namespace Flame {

  Attack_spell::Attack_spell(const Point2f& location_,
                             const Vector2f& orientation_,
                             float radius_,
                             float attack_strength_,
                             bool is_player_,
                             bool heal_self_,
                             Player * player_ptr_) :
    body(location_, orientation_, radius_),
    player_ptr(player_ptr_),
    attack_strength(attack_strength_),
    is_player(is_player_),
    heal_self(heal_self_),
    orientation(orientation_)
    {}

  void Attack_spell::update(float)
  {
    if (is_player) {
      vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
      for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
        if (body.intersect((*it)->get_body())) {
          vector<attack_effect> effects;
          effects.push_back(HITBACK);
          (*it)->get_hit(attack_strength, effects, player_ptr, orientation);
          if (heal_self)
            player_ptr->dec_health(-.5f * attack_strength);
          Model_state::get_instance()->add_spell(new Get_hit((*it)->get_location() + Vector2f(0.f, 5.f)));
        }
    }
    else {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (body.intersect((*it)->get_body())) {
          (*it)->get_hit(attack_strength, vector<attack_effect>(), player_ptr);
          Model_state::get_instance()->add_spell(new Get_hit((*it)->get_location() + Vector2f(0.f, 5.f)));
        }
    }
    disable_spell();
  }

  // Tripitaka

  Disintegrate::Disintegrate(Player * player_ptr_, float damage_) :
    Spell(99999.9f),
    player_ptr(player_ptr_),
    length(kDisintegrate_length),
    render_time(0.f),
    damage(damage_)
    {Disintegrate::update_body();}

  Point2f Disintegrate::get_location() const
    {return player_ptr->get_current_location();}

  void Disintegrate::update_body()
  {
      Point3f endpoint_a = Point3f(player_ptr->get_current_location().x, player_ptr->get_current_location().y, kCollision_object_height);
      Vector3f orientation = Vector3f(player_ptr->get_current_orientation().x, player_ptr->get_current_orientation().y, 0.f).normalized();
      orientation *= kDisintegrate_length;
      Point3f endpoint_b = endpoint_a + orientation;
      body = Collision::Capsule(endpoint_a, endpoint_b, kDisintegrate_radius);
  }

  void Disintegrate::update(float time)
  {
    render_time += time;
    vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
    vector<attack_effect> effects;
    Monster * target_ptr = nullptr;
    update_body();
    for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
      if (body.intersects((*it)->get_body()))
        if (!target_ptr || (
              (*it)->get_location() - player_ptr->get_current_location()).magnitude() <
                (target_ptr->get_current_location() - player_ptr->get_current_location()).magnitude())
          target_ptr = *it;
    if (target_ptr) {
      target_ptr->get_hit(damage, vector<attack_effect>(), player_ptr);
      length = (target_ptr->get_current_location() - player_ptr->get_current_location()).magnitude();
    }
    else
      length = kDisintegrate_length;
  }

  void Disintegrate::render()
  {
    float scale = Model_state::get_instance()->get_scale();
    Point2f location = (player_ptr->get_current_location() - Model_state::get_instance()->get_center_location())
                        * scale + Point2f(400.f, 300.f);
    location -= scale * Vector2f(kDisintegrate_radius, length);
    float theta = player_ptr->get_current_orientation().angle_between(Vector2f(0.f, 1.f));
    if (player_ptr->get_current_orientation().x < 0.f)
      theta = 2 * Global::pi - theta;
    theta += Global::pi;
    String texture = "disintegrate0";
    if (int((render_time - int(render_time)) * 10) % 2)
      texture = "disintegrate1";
    if (length < 256.f) {
      texture = "disintegrate_short0";
        if (int((render_time - int(render_time)) * 10) % 2)
          texture = "disintegrate_short1";
    }
    render_image(texture,
                 location,
                 location + scale * Vector2f(2 * kDisintegrate_radius, length),
                 theta,
                 1.f,
                 location + scale * Vector2f(kDisintegrate_radius, length));
  }

  Ding::Ding(const Point2f& location_, Player * player_ptr_, float damage_) :
    Resizable_spell(location_, kDing_size, Vector2f(), kDing_life_time),
    player_ptr(player_ptr_),
    render_time(0.f),
    damage(damage_)
    {}

  void Ding::update(float time)
  {
    Resizable_spell::update(time);
    render_time += time;
    if (render_time > 1.f)
      render_time = 1.f;
    vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
    vector<attack_effect> effects;
    effects.push_back(FREEZE);
    for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
      if (get_body().intersects((*it)->get_body()))
        (*it)->get_hit(damage, effects, player_ptr);
  }

  void Ding::render()
  {Resizable_spell::render("ding", Color(render_time, 0.89f, 0.78f, 0.04f));}

  Healing_spell::Healing_spell(const Point2f& location_, const Vector2f& orientation_, float damage_) :
    Moving_spell_circle(location_ + 50.f * orientation_.normalized(),
                        orientation_,
                        Vector2f(kHealing_size, kHealing_size),
                        kHealing_speed,
                        kHealing_life_time),
    damage(damage_)
    {}

  void Healing_spell::update(float time)
  {
    Moving_spell_circle::update(time);
    if (is_active()) {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          (*it)->dec_health(damage);
          disable_spell();
          break;
        }
    }
  }

  void Healing_spell::render()
  {Moving_spell::render("healing_spell");}

  // Monkey King
  Cudgel_fury::Cudgel_fury(const Point2f& location_,
                           const Vector2f& orientation_,
                           const float size_,
                           Player * player_ptr_,
                           Chronometer<Time>* game_time_,
                           float damage_)
  : Moving_spell_circle(location_, orientation_, Zeni::Vector2f(size_*5, size_*5), 0.0f, kCudgelfury_last),
    player_ptr(player_ptr_),
    render_flag(0),
    last_render_time(0.0f),
    game_time(game_time_),
    damage(damage_)
  {monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();}

  void Cudgel_fury::update(float time)
  {

    //Moving_spell_circle::update(time);
    set_location(player_ptr->get_location());
    Moving_spell_circle::update_body();
    Spell::update(time);

    float current_time = game_time->seconds();
    float passed_time = current_time - last_render_time;
    if (passed_time > kCudgelfury_render_gap) {
      last_render_time = current_time;
      render_flag = (render_flag + 1) % (kCudgelfury_render_max+1);
    }
    
    if (is_active()) {
      for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          vector<attack_effect> effects;
          //effects.push_back(HITBACK);
          (*it)->get_hit(damage, effects, player_ptr);
          //disable_spell();
          break;
        }
    } else {
      player_ptr->cudgel_fury_end();
    }
    
  }

  void Cudgel_fury::render()
  {
    Zeni::String texture;// = "cudgel_fury";
    Zeni::Point2f center = get_center_location();//player_ptr->get_location();
    float scale = Model_state::get_instance()->get_scale();
    Zeni::Point2f rel_loc = player_ptr->get_rel_loc();
    float size = player_ptr->get_radius();
    //Vector2f orient = get_current_orientation();
    //float rad = orient_vec_to_radians(orient);
    Zeni::Point2f ul, dr;
    ul = Point2f(rel_loc.x - size*3.0f, (rel_loc.y - size*3.0f));
    dr = Point2f(rel_loc.x + size*3.0f, (rel_loc.y + size*3.0f));
    switch (render_flag) {
      case 0:
        texture = "cudgel_fury0";
        break;
      case 1:
        texture = "cudgel_fury1";
        break;
      case 2:
        texture = "cudgel_fury2";
        break;
      default: //case 3
        texture = "cudgel_fury3";
        break;
    }
    render_image(texture,
                   ul,
                   dr,
                   0.0f,
                   scale,
                   rel_loc,
                   false);
  }

  // Pigsy
  Taunt::Taunt(const Point2f& location_,
               Player * player_ptr_) :
    Resizable_spell(location_, Vector2f(kTaunt_radius, kTaunt_radius), Vector2f(), kTaunt_life_time),
    player_ptr(player_ptr_)
    {
      vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
      for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()))
          (*it)->increase_hatred(kTaunt_hatred, player_ptr);
    }

  void Taunt::render()
  {Resizable_spell::render("brick");}

  // Friar Sand
  Arrow_attack::Arrow_attack(const Point2f& location_,
                             const Vector2f& orientation_,
                             Player * player_ptr_,
                             float damage_) :
    Moving_spell_rectangle(location_, 
                           orientation_,
                           kArrow_size,
                           kArrow_speed,
                           kArrow_life_time),
    player_ptr(player_ptr_),
    damage(damage_)
    {}

  void Arrow_attack::update(float time)
  {
    Moving_spell_rectangle::update(time);
    if (is_active()) {
      vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
      for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          vector<attack_effect> effects;
          effects.push_back(HITBACK);
          (*it)->get_hit(damage, effects, player_ptr, get_orientation());
          Model_state::get_instance()->add_spell(new Get_hit((*it)->get_location() + Vector2f(0.f, 5.f)));
          disable_spell();
          break;
        }
    }
  }

  void Arrow_attack::render()
  {Moving_spell::render("arrow");}

  Magic_arrow_ice::Magic_arrow_ice(const Point2f& location_,
                                   const Vector2f& orientation_,
                                   Player * player_ptr_,
                                   float damage_) :
    Moving_spell_rectangle(location_, orientation_, kMagic_arrow_size,
                           kMagic_arrow_speed, kMagic_arrow_life_time),
    player_ptr(player_ptr_),
    damage(damage_)
  {}

  void Magic_arrow_ice::update(float time)
  {
    Moving_spell_rectangle::update(time);
    if (is_active()) {
      vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
      for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          vector<attack_effect> effects;
          effects.push_back(HITBACK);
          (*it)->get_hit(damage, effects, player_ptr, get_orientation());
          Magic_arrow_ice_effect effect(get_center_location());
          Model_state::get_instance()->add_spell(new Get_hit((*it)->get_location() + Vector2f(0.f, 5.f)));
          disable_spell();
          break;
        }
    }
  }

  void Magic_arrow_ice::render()
  {Moving_spell::render("magic_arrow_ice");}

  Magic_arrow_fire::Magic_arrow_fire(const Point2f& location_,
                                     const Vector2f& orientation_,
                                     Player * player_ptr_,
                                     float damage_) :
    Moving_spell_rectangle(location_, orientation_, kMagic_arrow_size,
                           kMagic_arrow_speed, kMagic_arrow_life_time),
    player_ptr(player_ptr_),
    damage(damage_)
  {}

  void Magic_arrow_fire::update(float time)
  {
    Moving_spell_rectangle::update(time);
    if (is_active()) {
      vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
      for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          vector<attack_effect> effects;
          effects.push_back(HITBACK);
          (*it)->get_hit(damage, effects, player_ptr, get_orientation());
          Model_state::get_instance()->add_spell(new Magic_arrow_fire_effect(get_center_location()));
          Model_state::get_instance()->add_spell(new Get_hit((*it)->get_location() + Vector2f(0.f, 5.f)));
          disable_spell();
          break;
        }
    }
  }

  void Magic_arrow_fire::render()
  {Moving_spell::render("magic_arrow_fire");}

  Magic_arrow_ice_effect::Magic_arrow_ice_effect(const Point2f& location_, Player * player_ptr_) :
    Resizable_spell(location_, kMagic_arrow_effect_size, Vector2f(), 0.f),
    player_ptr(player_ptr_)
  {
    vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
    vector<attack_effect> effects;
    effects.push_back(FREEZE);
    for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
      if (get_body().intersects((*it)->get_body()) && (*it)->is_alive())
        (*it)->get_hit(0.f, effects, player_ptr);
  }
    
  Magic_arrow_fire_effect::Magic_arrow_fire_effect(const Point2f& location_, Player * player_ptr_) :
    Resizable_spell(location_, kMagic_arrow_effect_size, Vector2f(), kMagic_arrow_effect_life_time),
    player_ptr(player_ptr_),
    timer(0.f)
  {}

  void Magic_arrow_fire_effect::update(float time)
  {
    Resizable_spell::update(time);
    timer += time;
    vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
    for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
      if (get_body().intersects((*it)->get_body()) && (*it)->is_alive())
        (*it)->get_hit(kMagic_arrow_effect_damage, vector<attack_effect>(), player_ptr);
  }

  void Magic_arrow_fire_effect::render()
  {
    if (timer - int(timer) < 0.25f || (timer - int(timer) > 0.5f && timer - int(timer) < 0.75f))
      Resizable_spell::render("fire_effect0");
    else
      Resizable_spell::render("fire_effect1");
  }

  Strafe::Strafe(const Point2f& location_, const Vector2f& orientation_, Player * player_ptr_, float damage_) :
    Spell(kArrow_life_time),
    location(location_),
    arrow0(location_, orientation_)
  {
    Vector3f orientation0 = Vector3f(orientation_.x, orientation_.y, 0.f);
    Vector3f orientation1 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), Global::pi / 8) * orientation0;
    Vector3f orientation2 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), Global::pi / 4) * orientation0;
    Vector3f orientation3 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), -Global::pi / 8) * orientation0;
    Vector3f orientation4 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), -Global::pi / 4) * orientation0;
    arrow1 = Arrow_attack(location_, Vector2f(orientation1.x, orientation1.y), player_ptr_, damage_);
    arrow2 = Arrow_attack(location_, Vector2f(orientation2.x, orientation2.y), player_ptr_, damage_);
    arrow3 = Arrow_attack(location_, Vector2f(orientation3.x, orientation3.y), player_ptr_, damage_);
    arrow4 = Arrow_attack(location_, Vector2f(orientation4.x, orientation4.y), player_ptr_, damage_);
  }

  void Strafe::update(float time)
  {
    if (arrow0.is_active())
      arrow0.update(time);
    if (arrow1.is_active())
      arrow1.update(time);
    if (arrow2.is_active())
      arrow2.update(time);
    if (arrow3.is_active())
      arrow3.update(time);
    if (arrow4.is_active())
      arrow4.update(time);
    location = arrow0.get_location();
  }

  void Strafe::render()
  {
    if (arrow0.is_active())
      arrow0.render();
    if (arrow1.is_active())
      arrow1.render();
    if (arrow2.is_active())
      arrow2.render();
    if (arrow3.is_active())
      arrow3.render();
    if (arrow4.is_active())
      arrow4.render();
  }

  Trap::Trap(const Zeni::Point2f& location_, Player * player_ptr_, float damage_) :
    Resizable_spell(location_, kTrap_size, Vector2f(), kTrap_life_time),
    player_ptr(player_ptr_),
    remain_times(5),
    timer(0.f),
    damage(damage_)
    {}

  void Trap::update(float time)
  {
    Resizable_spell::update(time);
    if (timer > 0.f) {
      timer -= time;
      return;
    }
    srand(int(std::time(NULL)));
    vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
    for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
      if (Resizable_spell::get_body().intersects((*it)->get_body())) {
        for (int i = 0; i < 10; i++) {
          float dir_scale = 1.2f * (rand() % 3 - 1) * (rand() % 201 - 100) / 100.f;
          float speed = rand() % 50 + kTrap_attack_speed;
          Vector2f orientation = (*it)->get_location() - get_center_location();
          Vector2f counter_orientation = Vector2f(orientation.y, orientation.x);
          Spell * new_spell = new Trap_attack(get_center_location(),
                                              orientation + dir_scale * counter_orientation,
                                              speed,
                                              player_ptr,
                                              damage);
          Model_state::get_instance()->add_spell(new_spell);
        }
        timer = 3.f;
        if (!--remain_times)
          disable_spell();
        break;
      }
  }

  void Trap::render()
  {
    if (timer > 2.5f)
      Resizable_spell::render("trap_active");
    else
      Resizable_spell::render("trap_inactive");
  }

  Trap_attack::Trap_attack(const Point2f& location_,
                           const Vector2f& orientation_,
                           float speed_,
                           Player * player_ptr_,
                           float damage_) :
    Moving_spell_circle(location_,
                        orientation_,
                        kTrap_attack_size, speed_, kTrap_attack_life_time),
    orientation(orientation_),
    counter_orientation(Vector2f(-orientation_.y, orientation_.x)),
    is_left(-1),
    time_counter(kTrap_attack_period),
    render_timer(0.f),
    player_ptr(player_ptr_),
    damage(damage_)
    {}

  void Trap_attack::update(float time)
  {
    Moving_spell_circle::update(time);
    time_counter -= time;
    render_timer += time;
    if (time_counter < 0.f) {
      is_left = -is_left;
      time_counter = kTrap_attack_period;
      set_orientation(orientation + 0.1f * is_left * counter_orientation);
    }
    vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
    for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
      if (get_body().intersects((*it)->get_body())) {
        (*it)->get_hit(damage, vector<attack_effect>(), player_ptr);
        Spell::disable_spell();
        break;
      }
  }

  void Trap_attack::render()
  {
    if (render_timer - int(render_timer) < 0.25f)
      Moving_spell::render("lightning0");
    else if (render_timer - int(render_timer) < 0.5f || render_timer - int(render_timer) > 0.75f)
      Moving_spell::render("lightning1");
    else if (render_timer - int(render_timer) < 0.75f)
      Moving_spell::render("lightning2");
  }

  Track_attack::Track_attack(const Point2f& location_,
                             const Vector2f& size_,
                             float speed_,
                             float damage_,
                             float life_time_,
                             vector<Flame::attack_effect> attack_effect_,
                             Agent * target_ptr_,
                             Player * player_ptr_) :
    Moving_spell_circle(location_,
                        target_ptr_->get_location() - location_,
                        size_,
                        speed_,
                        life_time_),
    damage(damage_),
    attack_effect(attack_effect_),
    target_ptr(target_ptr_),
    player_ptr(player_ptr_)
    {}

  void Track_attack::update(float time)
  {
    Render_list_t * render_list_ptr = Model_state::get_instance()->get_render_list_ptr();
    if (render_list_ptr->find(target_ptr) == render_list_ptr->end()) {
      disable_spell();
      return;
    }
    set_orientation(target_ptr->get_location() - get_location());
    Moving_spell_circle::update(time);
    if (get_body().intersects(target_ptr->get_body())) {
        target_ptr->get_hit(damage, attack_effect, player_ptr);
        disable_spell();
    }
  }

  void Track_attack::render()
  {Moving_spell::render("brick");}

  // Boss1
  Fire_ball::Fire_ball(const Point2f& location_, const Vector2f& orientation_) :
    Moving_spell_circle(location_ + 50.f * orientation_.normalized(),
                        orientation_,
                        Vector2f(kFireball_size, kFireball_size),
                        kFireball_speed,
                        kFireball_life_time)
    {}

  void Fire_ball::update(float time)
  {
    Moving_spell_circle::update(time);
    if (is_active()) {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          (*it)->dec_health(kFireball_damage);
          Model_state::get_instance()->add_spell(new Get_hit((*it)->get_location() + Vector2f(0.f, 5.f)));
          disable_spell();
          break;
        }
    }
  }

  void Fire_ball::render()
  {Moving_spell::render("fire_ball");}

  Hell_spikes::Hell_spikes(const Point2f& location_) :
    Resizable_spell(location_, kHell_spikes_size, Vector2f(),
                    kHell_spikes_pre_time + kHell_spikes_life_time),
    pre_time(kHell_spikes_pre_time)
  {}

  Ring_of_fire::Ring_of_fire(const Point2f& location_, const Vector2f& orientation_) :
    Moving_spell_circle(location_ + 50.f * orientation_.normalized(),
                        orientation_,
                        kRing_of_fire_size,
                        kRing_of_fire_speed,
                        kRing_of_fire_life_time)
    {}

  void Ring_of_fire::update(float time)
  {
    Moving_spell_circle::update(time);
    if (is_active()) {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          (*it)->dec_health(kFireball_damage);
          Model_state::get_instance()->add_spell(new Get_hit((*it)->get_location() + Vector2f(0.f, 5.f)));
          disable_spell();
          break;
        }
    }
  }

  void Ring_of_fire::render()
  {
    Vector2f orientation = get_orientation();
    float theta = orientation.angle_between(Vector2f(0.f, 1.f));
    float scale = Model_state::get_instance()->get_scale();
    Vector2f size = get_size();
    if (orientation.x < 0.f)
      theta = 2 * Global::pi - theta;
    theta += Global::pi;
    render_image("ring_of_fire",
                 get_relative_location() - scale * size / 2 - Vector2f(size.x / 4, 0.f),
                 get_relative_location() + scale * size / 2 + Vector2f(size.x / 4, size.y),
                 theta,
                 1.f,
                 get_relative_location());
  }

  void Hell_spikes::update(float time)
  {
    Resizable_spell::update(time);
    if (pre_time > 0.f) {
      pre_time -= time;
      return;
    }
    vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
    for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
      if (get_body().intersects((*it)->get_body()) && (*it)->is_alive())
        (*it)->dec_health(kHell_spikes_damage);
  }

  void Hell_spikes::render()
  {
    if (pre_time > 0.f)
      Resizable_spell::render("hell_spikes_pre",
        Color((kHell_spikes_pre_time - pre_time) / kHell_spikes_pre_time, 0.89f, 0.78f, 0.04f));
    else
      Resizable_spell::render("hell_spikes");
  }

}