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
          (*it)->dec_health(attack_strength);
          if (heal_self)
            player_ptr->dec_health(.5f * attack_strength);
        }
    }
    else {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (body.intersect((*it)->get_body()))
          (*it)->dec_health(attack_strength);
    }
    disable_spell();
  }

  Healing_spell::Healing_spell(const Point2f& location_, const Vector2f& orientation_) :
    Moving_spell_circle(location_ + 50 * orientation_.normalized(),
                        orientation_,
                        Vector2f(kAmitabha_size, kAmitabha_size),
                        kAmitabha_speed,
                        kAmitabha_life_time)
    {}

  void Healing_spell::update(float time)
  {
    Moving_spell_circle::update(time);
    if (is_active()) {
      vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
      for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
        if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
          (*it)->dec_health(kAmitabha_healing_amount);
          disable_spell();
          break;
        }
    }
  }

  void Healing_spell::render()
  {Moving_spell::render("healing_spell");}

}