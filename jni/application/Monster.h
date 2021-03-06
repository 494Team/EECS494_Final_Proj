#ifndef MONSTER_H
#define MONSTER_H

#include <vector>
#include <map>
#include "Player.h"
#include "Agent.h"

namespace Flame {
  class Monster : public Agent {
  public:
    Monster(
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const float &attack_gap_ = 0.0f,
      const float &view_range_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f());

    // increase hatred for a specific player
    void increase_hatred(const float &hate_, Player* player);
    // clear hatred for a player
    void clear_hatred(Player* player);

    // find player with the highest hatred
    Player * highest_hatred();
    // find player nearest to this monster
    Player * nearest_player();

    void attack();

    // is previous attack finished?
    bool can_attack();

    float get_current_time() {return current_time;}
    float get_prev_attack_time() {return prev_attack_time;}

    // increase current_time by time. MUST be called at every perform_logic()
    void update_current_time(float time) {current_time += time;}

    // set prev_attack_time every time an attack happens
    void set_prev_attack_time(float prev_attack_time_) {prev_attack_time = prev_attack_time_;}

    void update_rel_loc();

    void make_move(float time, bool false_move = false);

    void update(float time);

    void get_hit(const float &damage, const std::vector<attack_effect> &effects, Player* attacker, Zeni::Vector2f coming_ori = Zeni::Vector2f());

    void get_render_params(float render_radius, Zeni::Point2f &ul, Zeni::Point2f &lr, float &radians_ccw);
    
    void update_render_suffix();

    bool is_invincible() const {return invincible;}
    void set_invincible(bool inv_) {invincible = inv_;}
    void render_get_wukong_charge_effect();
      
  protected:
    float calc_angle_between(const Zeni::Vector2f &a, const Zeni::Vector2f &b);

    Player* target;
    Player* last_target;
    std::vector<Player *> players;
    std::map<Player *, float> hatred;
    Zeni::Point2f rel_loc;

    bool is_attacking;
    float attack_gap;
    Zeni::String render_suffix;
    float view_range;
  private:
    float current_time;
    float prev_attack_time;
    float last_render_change_time;
    bool invincible;
  };
}

#endif