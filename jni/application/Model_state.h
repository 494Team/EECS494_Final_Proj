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
  class Sim_object;
  class Player;
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
    // should be called after all players are updated
    void update();

    Render_list_t * get_sim_list_ptr()
    {return &render_list;}
    std::vector<Player *> * get_player_list_ptr()
    {return &player_list;}
    std::vector<Monster *> * get_monster_list_ptr()
    {return &monster_list;}
    std::vector<Spell *> * get_spell_list_ptr()
    {return &spell_list;}

    // never try to remove from outside!!!!!!!!!
    void remove_from_render_list(Sim_object * sim_ptr);

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
    std::vector<Player *> player_list;
    std::vector<Monster *> monster_list;
    std::vector<Spell *> spell_list;
    Zeni::Point2f center_location;
    float scale;

    /* prevent copy constructor/assignment and destructor */
    Model_state(const Model_state&);
    Model_state& operator= (const Model_state&);
    ~Model_state();
  };

}

#endif
