#include "Model_state.h"
#include "Player.h"
#include "Monster.h"
#include "Spell.h"
#include <algorithm>
#include <map>
#include <utility>

using std::find;
using std::map;
using std::pair; using std::make_pair;
using std::vector;
using Zeni::Point2f;

namespace Flame {

  Model_state_destroyer model_destroyer;

  Model_state * Model_state::ptr = nullptr;

  Model_state_destroyer::~Model_state_destroyer()
    {delete Model_state::ptr;}

  Model_state::Model_state() :
    center_location(Point2f()),
    scale(1.f)
  {}

  Model_state::~Model_state()
  {
    for (auto it = player_list.begin(); it != player_list.end();)
      it = player_list.erase(it);
    for (auto it = monster_list.begin(); it != monster_list.end();)
      it = monster_list.erase(it);
    for (auto it = spell_list.begin(); it != spell_list.end();)
      it = spell_list.erase(it);
    for (auto it = render_list.begin(); it != render_list.end(); ++it) {
      delete *it;
      render_list.erase(it);
    }
  }

  Model_state * Model_state::get_instance()
  {
    if (!ptr)
      ptr = new Model_state();
    return ptr;
  }

  void Model_state::remove_from_render_list(Sim_object * sim_ptr)
  {render_list.erase(sim_ptr);}

  void Model_state::update()
  {
    scale = 5.f;
    // calculate scale
		for(int i = 0; i < player_list.size(); ++i)
			for(int j = 1; j < player_list.size(); ++j) {
        float dis_x = abs(player_list[(i + j) % 4]->get_location().x - player_list[i]->get_location().x);
        float dis_y = abs(player_list[(i + j) % 4]->get_location().y - player_list[i]->get_location().y);
        float frac = dis_x / dis_y;
        if (dis_y != 0. && frac < 4.f / 3.f) {
          if (200.f / dis_y < 1.f) {
            if (scale > 1.f)
              scale = 1.f;
          }
          else if (200.0f / dis_y > 5.f) {
            if (scale > 5.f)
				      scale = 5.f;
          }
			    else {
            if (scale > 200.f / dis_y)
				      scale = 200.f / dis_y;
            }
		    }
		    else {
			    if (800.f / 3 / dis_x < 1.f) {
            if (scale > 1.f)
				      scale = 1.f;
          }
			    else if (800.0f / 3 / dis_x > 5.f) {
            if (scale > 5.f)
				      scale = 5.f;
          }
			    else {
				    if (scale > 800.f / 3 / dis_x)
              scale = 800.f / 3 / dis_x;
          }
		    }

      }

    // get center_location's x and y
    float center_x = 0.f;
    float center_y = 0.f;
    for (vector<Player *>::iterator it = player_list.begin(); it != player_list.end(); ++it) {
      center_x += (*it)->get_location().x / 4;
      center_y += (*it)->get_location().y / 4;
    }

    center_location = Point2f(center_x, center_y);
	}

}
