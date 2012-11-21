#ifndef MODEL_STATE_H
#define MODEL_STATE_H

/*
 * Model_state.h defines a Model_state Singleton declaring and implementing a set of functions
 * that keep track of the game-world state. It is a unique and globally-accessible object.
 * usage: Flame::Modell::get_instance()->foo()
 */

#include <zeni.h>
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
    void init(int level);

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
    std::vector<Monster *> * get_monster_list_ptr()
    {return &monster_list;}
    std::vector<Spell *> * get_spell_list_ptr()
    {return &spell_list;}
    std::vector<Map *> * get_map_obj_list_ptr()
    {return &map_obj_list;}

    // never try to remove from outside!!!!!!!!!
    void remove_player(Player * player_ptr);
    void remove_monster(Monster * monster_ptr);
    void remove_spell(Spell * spell_ptr);
    void remove_map_obj(Map * map_ptr);

    // check if a monster can move to the specific location
    bool can_move(Collision_object * collision_obj);

    // check if a player can move to the specific location, and trigger the map object to change
    bool can_move_player(Collision_object * collision_obj);

    // accessors
    float get_scale()
    {return scale;}
    Zeni::Point2f get_center_location() const
    {return center_location;}

    friend class Model_state_destroyer;
  private:
    // constructor is private thus nobody else can create one
    Model_state();
    static Model_state * ptr; // only one, class-wide

    /* member variables */
    Render_list_t render_list;
    std::vector<Sim_object *> sim_obj_list;
    std::vector<Player *> player_list;
    std::vector<Monster *> monster_list;
    std::vector<Spell *> spell_list;
    std::vector<Map *> map_obj_list;
    Zeni::Point2f center_location;
    float scale;

    /* prevent copy constructor/assignment and destructor */
    Model_state(const Model_state&);
    Model_state& operator= (const Model_state&);
    ~Model_state();
  };

}

#endif