#ifndef MODEL_STATE_H
#define MODEL_STATE_H

/*
 * Model_state.h defines a Model_state Singleton declaring and implementing a set of functions
 * that keep track of the game-world state. It is a unique and globally-accessible object.
 * usage: Flame::Modell::get_instance()->foo()
 */

#include <zeni.h>
#include <zenilib.h>
#include "Utility.h"
#include <vector>

namespace Flame {

  // forward declarations
  class Collision_object;
  class Sim_object;
  class Player;
  class Map;
  class Monster;
  class Spell;

  // destroy the Model when program terminates
  class Model_state_destroyer {
  public:
    ~Model_state_destroyer();
  };

  class Model_state {
  public:
    static Model_state * get_instance();

    /* services member functions defined here */
    void init(int level, Zeni::Chronometer<Zeni::Time> * timer_);

    // should be called after all players are updated
    void update_scale_and_center();

    // call update() for all sim_objects
    void update(float time = 0.f);
    // render all objects in render_list
    void render();

    Render_list_t * get_render_list_ptr()
    {return &render_list;}
    std::vector<Player *> * get_player_list_ptr()
    {return &player_list;}
    std::vector<Player *> * get_dead_player_list_ptr()
    {return &dead_player_list;}
    std::vector<Monster *> * get_monster_list_ptr()
    {return &monster_list;}
    std::vector<Spell *> * get_spell_list_ptr()
    {return &spell_list;}
    std::vector<Map *> * get_map_obj_list_ptr()
    {return &map_obj_list;}
    std::vector<Map *> * get_map_puzzle_obj_list_ptr()
    {return &map_puzzle_obj_list;}
    std::vector<Map *> * get_map_door_obj_list_ptr()
    {return &map_door_obj_list;}
    std::vector<kPlayer_type> * get_character_list_ptr()
    {return &character_list;}


    // never try to add yourself! use these functions!!!!!!!
    void add_player(Player * player_ptr);
    void add_monster(Monster * monster_ptr);
    void add_spell(Spell * spell_ptr);
    void add_map_obj(Map * map_obj_ptr);
    void add_map_puzzle_obj(Map * map_obj_ptr);

    // never try to remove yourself! use these functions!!!!!!!
    std::vector<Player *>::iterator remove_player(Player * player_ptr);
    std::vector<Player *>::iterator player_rise_from_dead_list(Player * player_ptr);
    std::vector<Player *>::iterator move_player_to_dead_list(Player * player_ptr);
    std::vector<Monster *>::iterator remove_monster(Monster * monster_ptr);
    std::vector<Spell *>::iterator remove_spell(Spell * spell_ptr);
    std::vector<Map *>::iterator remove_map_obj(Map * map_ptr);
    std::vector<Map *>::iterator remove_map_puzzle_obj(Map * map_ptr);

    // check if a monster/spell/player can move to the specific location
    bool can_move(const Zeni::Collision::Capsule& collision_body);
    bool can_move(const Zeni::Collision::Parallelepiped& collision_body);

    bool can_monster_move(const Zeni::Collision::Capsule& collision_body);
    bool can_player_move(const Zeni::Collision::Capsule& collision_body);

    // check if a player can move to the specific location, and trigger the map object to change
    bool can_move_player(const Zeni::Collision::Capsule& collision_body);

    // accessors
    Zeni::Chronometer<Zeni::Time> * get_timer() const
    {return timer;}
    float get_scale()
    {return scale;}
    Zeni::Point2f get_center_location() const
    {return center_location;}

    friend class Model_state_destroyer;

    int get_money_amount() {
      return money;
    }
    int get_initial_player_num() const
      {return initial_player_num;}
    void set_initial_player_num(int initial_player_num_)
      {initial_player_num = initial_player_num_;}
    int get_player_pos_in_list(const int list_pos) { //get controller
      if (list_pos > 3 || list_pos < 0)
        return -1;
      else
        return player_pos_in_list[list_pos];
    }
    int get_player_list_index(int controller) {
        int list_pos = -1;
        for (int i=0; i<4; i++) {
          if (controller >= 0 &&
              controller <= 3 && 
              Model_state::get_instance()->get_player_pos_in_list(i) == controller) {
            list_pos = i;
            break;
          }
        }
        return list_pos;
    }
    bool is_controller_alive(const int controller) {
      return controller_alive[controller];
    }
  private:
    int initial_player_num;
    bool controller_alive[4]; //usage: controller_alive[controller]
    int player_pos_in_list[4]; //update when any player die
    int money;
    void clear();

    // constructor is private thus nobody else can create one
    Model_state();
    static Model_state * ptr; // only one, class-wide

    /* member variables */
    Render_list_t render_list;
    std::vector<Sim_object *> next_loop_update_list;
    std::vector<Sim_object *> sim_obj_list;
    std::vector<Sim_object *> remove_list;
    std::vector<Player *> player_list;
    std::vector<Player *> dead_player_list;
    std::vector<Monster *> monster_list;
    std::vector<Spell *> spell_list;
    std::vector<Map *> map_obj_list;
    std::vector<Map *> map_puzzle_obj_list;
    std::vector<Map *> map_door_obj_list;
    std::vector<kPlayer_type> character_list;
    Zeni::Point2f center_location;
    Zeni::Chronometer<Zeni::Time> * timer;
    float scale;

    /* prevent copy constructor/assignment and destructor */
    Model_state(const Model_state&);
    Model_state& operator= (const Model_state&);
    ~Model_state();
  };

}

#endif
