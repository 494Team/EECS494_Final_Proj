#include "Spells.h"
#include "Monster.h"
#include <vector>

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
    attack_strength(attack_strength_),
    is_player(is_player_),
    heal_self(heal_self_),
    player_ptr(player_ptr_)
    {}

  void Attack_spell::update(float)
  {
    if (is_player) {
      vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
      for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
        if (body.intersect((*it)->get_body())) {
          vector<attack_effect> effects;
          effects.push_back(HITBACK);
          (*it)->get_hit(attack_strength, effects, nullptr);
          if (heal_self)
            player_ptr->dec_health(-.5f * attack_strength);
        }
    }
    else {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (body.intersect((*it)->get_body()))
          (*it)->get_hit(attack_strength, vector<attack_effect>(), player_ptr);
    }
    disable_spell();
  }

  // Tripitaka
  Healing_spell::Healing_spell(const Point2f& location_, const Vector2f& orientation_) :
    Moving_spell_circle(location_ + 50.f * orientation_.normalized(),
                        orientation_,
                        Vector2f(kHealing_size, kHealing_size),
                        kHealing_speed,
                        kHealing_life_time)
    {}

  void Healing_spell::update(float time)
  {
    Moving_spell_circle::update(time);
    if (is_active()) {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          (*it)->dec_health(kHealing_healing_amount);
          disable_spell();
          break;
        }
    }
  }

  void Healing_spell::render()
  {Moving_spell::render("healing_spell");}

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
  Arrow_attack::Arrow_attack(const Point2f& location_, const Vector2f& orientation_) :
    Moving_spell_rectangle(location_, 
                           orientation_,
                           kArrow_size,
                           kArrow_speed,
                           kArrow_life_time)
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
          (*it)->get_hit(kArrow_damage, effects, nullptr);
          disable_spell();
          break;
        }
    }
  }

  void Arrow_attack::render()
  {Moving_spell::render("arrow");}

  Magic_arrow::Magic_arrow(const Point2f& location_, const Vector2f& orientation_)
  : Arrow_attack(location_, orientation_)
  {
  }

  void Magic_arrow::render()
  {Moving_spell::render("magic_arrow_ice");}

  Strafe::Strafe(const Point2f& location_, const Vector2f& orientation_) :
    Spell(kArrow_life_time),
    location(location_),
    arrow0(location_, orientation_)
  {
    Vector3f orientation0 = Vector3f(orientation_.x, orientation_.y, 0.f);
    Vector3f orientation1 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), Global::pi / 8) * orientation0;
    Vector3f orientation2 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), Global::pi / 4) * orientation0;
    Vector3f orientation3 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), -Global::pi / 8) * orientation0;
    Vector3f orientation4 = Quaternion::Axis_Angle(Vector3f(0.f, 0.f, 1.f), -Global::pi / 4) * orientation0;
    arrow1 = Arrow_attack(location_, Vector2f(orientation1.x, orientation1.y));
    arrow2 = Arrow_attack(location_, Vector2f(orientation2.x, orientation2.y));
    arrow3 = Arrow_attack(location_, Vector2f(orientation3.x, orientation3.y));
    arrow4 = Arrow_attack(location_, Vector2f(orientation4.x, orientation4.y));
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

  Trap::Trap(const Zeni::Point2f& location_) :
    Resizable_spell(location_, kTrap_size, Vector2f(), kTrap_life_time)
    {}

  void Trap::update(float time)
  {
    Resizable_spell::update(time);
    vector<Monster *> * monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
    for (auto it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it)
      if (Resizable_spell::get_body().intersects((*it)->get_body())) {
        (*it)->dec_health(kTrap_damage);
        Spell::disable_spell();
        break;
      }
  }

  void Trap::render()
  {Resizable_spell::render("brick");}

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
          disable_spell();
          break;
        }
    }
  }

  void Fire_ball::render()
  {Moving_spell::render("fire_ball");}

  Fire_spikes::Fire_spikes(const Point2f& location_) :
    Resizable_spell(location_, kFire_spike_size, Vector2f(), kFire_spike_life_time)
  {}

  void Fire_spikes::update(float time)
  {
    Resizable_spell::update(time);
    vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
    for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
      if (get_body().intersects((*it)->get_body()) && (*it)->is_alive())
        (*it)->dec_health(kFire_spike_damage);
  }

  void Fire_spikes::render()
  {Resizable_spell::render("brick");}

}