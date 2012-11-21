#include "Model_state.h"
#include "Player.h"
#include "Map.h"
#include "Monster.h"
#include "Spell.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <utility>

using std::bind; using std::find; using std::for_each;
using std::placeholders::_1;
using std::cerr; using std::endl;
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
    for (auto it = sim_obj_list.begin(); it != sim_obj_list.end();)
      it = sim_obj_list.erase(it);
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

  void Model_state::init(int level)
  {
    render_list.clear();
    sim_obj_list.clear();
    player_list.clear();
    monster_list.clear();
    spell_list.clear();
    player_list.push_back(new Player(100.f, 200.f, 16.f, Point2f(300.f, 200.f)));
    player_list.push_back(new Player(100.f, 200.f, 16.f, Point2f(500.f, 300.f)));
    player_list.push_back(new Player(100.f, 200.f, 16.f, Point2f(500.f, 200.f)));
    player_list.push_back(new Player(100.f, 200.f, 16.f, Point2f(300.f, 300.f)));
	  // wall on bottom
	  map_obj_list.push_back(new Map_brick(Point2f(-20.0f, 600.0f), Vector2f(820.0f,20.0f), 20.0f, 20.f, String("rock")));
	  // wall on top
	  map_obj_list.push_back(new Map_brick(Point2f(-20.0f, -20.0f), Vector2f(820.0f,20.0f), 20.0f, 20.f, String("rock")));
	  
	  // wall on left
	  map_obj_list.push_back(new Map_brick(Point2f(-20.0f, -20.0f), Vector2f(20.0f,620.0f), 20.0f, 20.f, String("rock")));

	  // wall on right
	  map_obj_list.push_back(new Map_brick(Point2f(800.0f, -20.0f), Vector2f(20.0f,640.0f), 20.0f, 20.f, String("rock")));	

	  // illuminate floor
	  map_obj_list.push_back(new Map_floor_illuminate(Point2f(320.0f, 400.0f), Point2f(20.f, 20.f), "floor", "ifloor"));

	  // rec structure
	  map_obj_list.push_back(new Map_structure_rec(Point2f(200.0f, 200.0f), Vector2f(50.0f, 50.0f), Point2f(200.0f, 200.0f),
                                                 Vector2f(50.0f, 50.0f), String("rock")));
	
	  // rec half block half through
	  map_obj_list.push_back(new Map_structure_rec(Point2f(400.0f, 400.0f), Vector2f(50.0f, 50.0f), Point2f(400.0f, 410.0f),
                                                 Vector2f(50.0f, 40.0f), String("house")));
    for (auto it = player_list.begin(); it != player_list.end(); ++it) {
      sim_obj_list.push_back(*it);
      render_list.insert(*it);
    }
    for (auto it = map_obj_list.begin(); it != map_obj_list.end(); ++it) {
      sim_obj_list.push_back(*it);
      render_list.insert(*it);
    }
    update_scale_and_center(); // get initial scale and location
  }
  
  void Model_state::update_scale_and_center()
  {
    scale = 5.f;
    // calculate scale
		for(int i = 0; i < int(player_list.size()); ++i)
			for(int j = 1; j < int(player_list.size()); ++j) {
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

  void Model_state::update(float time)
  {
    for (auto it = sim_obj_list.begin(); it != sim_obj_list.end(); ++it) {
      render_list.erase(*it);
      (*it)->update(time);
      render_list.insert(*it);
    }
  }

  void Model_state::render()
  {for_each(render_list.begin(), render_list.end(), bind(&Sim_object::render, _1));}

  void Model_state::remove_player(Player * player_ptr)
  {
    player_list.erase(find(player_list.begin(), player_list.end(), player_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), player_ptr));
    render_list.erase(player_ptr);
  }
  
  void Model_state::remove_monster(Monster * monster_ptr)
  {
    monster_list.erase(find(monster_list.begin(), monster_list.end(), monster_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), monster_ptr));
    render_list.erase(monster_ptr);
  }
  
  void Model_state::remove_spell(Spell * spell_ptr)
  {
    spell_list.erase(find(spell_list.begin(), spell_list.end(), spell_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), spell_ptr));
    render_list.erase(spell_ptr);
  }
  
  void Model_state::remove_map_obj(Map * map_obj_ptr)
  {
    map_obj_list.erase(find(map_obj_list.begin(), map_obj_list.end(), map_obj_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), map_obj_ptr));
    render_list.erase(map_obj_ptr);
  }

  bool Model_state::can_move(Collision_object * collision_obj)
  {
    for (auto it = map_obj_list.begin(); it != map_obj_list.end(); ++it)
      if (!(*it)->can_move(collision_obj))
        return false;
    return true;
  }

  bool Model_state::can_move_player(Collision_object * collision_obj)
  {
    bool can_move = true;
    for (auto it = map_obj_list.begin(); it != map_obj_list.end(); ++it)
      can_move = (*it)->can_move_player(collision_obj);
    return can_move;
  }
  
}