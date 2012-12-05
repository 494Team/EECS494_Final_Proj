#include "Model_state.h"
#include "Player.h"
#include "Map.h"
#include "Monster.h"
#include "Spell.h"
#include <algorithm>
#include <functional>
#include <map>
#include <utility>

using std::bind; using std::find; using std::for_each;
using std::placeholders::_1;
using std::map;
using std::pair; using std::make_pair;
using std::vector;
using Zeni::Point2f; using Zeni::Chronometer; using Zeni::Time;

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
  {clear();}

  Model_state * Model_state::get_instance()
  {
    if (!ptr)
      ptr = new Model_state();
    return ptr;
  }

  void Model_state::init(int level, Chronometer<Time> * timer_)
  {
    // enter game for the first time
    if (level==0) {
      money = 0;
    }

    clear();
    timer = timer_;
    player_list.push_back(new Player(timer, 100.f, 200.f, 16.f, Point2f(300.f, 200.f), WUKONG));
    player_list.push_back(new Player(timer, 100.f, 200.f, 16.f, Point2f(400.f, 300.f), BAJIE));
    player_list.push_back(new Player(timer, 100.f, 200.f, 16.f, Point2f(500.f, 200.f), SANZANG));
    player_list.push_back(new Player(timer, 100.f, 200.f, 16.f, Point2f(300.f, 300.f), SHASENG));
    
    // wall vertical
    map_obj_list.push_back(new Map_brick(Point2f(-40.f, 0.f), Vector2f(40.f, 2000.f), 20.f, 20.f, "rock"));
    map_obj_list.push_back(new Map_brick(Point2f(1000.f, 0.f), Vector2f(40.f, 2000.f), 20.f, 20.f, "rock"));
    map_obj_list.push_back(new Map_brick(Point2f(2040.f, 0.f), Vector2f(40.f, 2000.f), 20.f, 20.f, "rock"));
    map_obj_list.push_back(new Map_brick(Point2f(3080.f, 0.f), Vector2f(40.f, 2000.f), 20.f, 20.f, "rock"));
    /*
    map_obj_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(40.f,580.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(500.0f, 200.0f), Vector2f(40.f,340.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(700.0f - 40.f, 200.0f), Vector2f(40.f,540.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1100.0f, -40.0f), Vector2f(40.f, 540.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1300.0f - 40.f, -40.0f), Vector2f(40.f, 540.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1600.0f, 300.0f), Vector2f(40.f, 440.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1800.0f -40.f, 300.0f), Vector2f(40.f, 600.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(2000.0f, -40.0f), Vector2f(40.f, 1280.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(600.0f, 600.0f - 40.f), Vector2f(40.f, 340.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(-40.0f, 600.0f-40.0f), Vector2f(40.f, 1180.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1800.0f - 40.f, 1700.0f), Vector2f(40.f, 100.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(2000.0f, 1500.0f - 40.f), Vector2f(40.f, 580.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(400.0f, 1200.0f), Vector2f(40.f, 300.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(-40.0f, 1800.0f - 40.f), Vector2f(40.f, 280.0f), 20.0f, 20.f, String("rock")));
    */

    // wall horizontal
    map_obj_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(3160.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(-40.0f, 2000.0f), Vector2f(3160.f, 40.0f), 20.0f, 20.f, String("rock")));
    /*
    map_obj_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(1100.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1300.0f, -40.0f), Vector2f(700.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(0.0f, 500.0f), Vector2f(500.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(500.0f, 200.0f), Vector2f(200.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1100.0f, 500.0f-40.0f), Vector2f(200.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1300.0f, -40.0f), Vector2f(700.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(1600.0f, 300.0f-40.0f), Vector2f(200.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(400.0f, 1200.f), Vector2f(1600.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(400.0f, 1500.0f - 40.f), Vector2f(1600.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(0.0f, 1800.0f-40.0f), Vector2f(1800.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(0.0f, 2000.0f), Vector2f(2000.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(0.0f, 600-40.0f), Vector2f(600.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(600.0f, 900.f-40.0f), Vector2f(1200.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(700.0f, 700.f), Vector2f(900.f, 40.0f), 20.0f, 20.f, String("rock")));
    map_obj_list.push_back(new Map_brick(Point2f(0.0f, 1700.f), Vector2f(1800.f, 40.0f), 20.0f, 20.f, String("rock")));

    */
    // transmission gate
    map_obj_list.push_back(new Map_transmission_gate(Point2f(800.0f,200.0f), Point2f(200.f, 1500.f), Vector2f(20.f, 20.f), "clock"));
    // illuminate floor
    map_obj_list.push_back(new Map_floor_illuminate(Point2f(320.0f, 400.0f), Point2f(20.f, 20.f), "floor", "ifloor"));

    map_door_obj_list.push_back(new Map_door(Point2f(300.f, -20.f), Vector2f(50.f, 50.f), "close_door", "open_door"));
    (*map_door_obj_list.back()).add_trigger(map_obj_list.back());

    map_puzzle_obj_list.push_back(new Map_light_beam(Point2f(3.0f, 100.0f), Vector2f(1.0f, 0.0f), 0, "light_beam"));
    map_puzzle_obj_list.push_back(new Map_laser(Point2f(3.f, 500.f), Vector2f(1.f, 0.f), "laser"));

    for (auto it = player_list.begin(); it != player_list.end(); ++it) {
      sim_obj_list.push_back(*it);
      render_list.insert(*it);
    }
    for (auto it = map_obj_list.begin(); it != map_obj_list.end(); ++it) {
      sim_obj_list.push_back(*it);
      render_list.insert(*it);
    }

    for (auto it = map_puzzle_obj_list.begin(); it != map_puzzle_obj_list.end(); ++it) {
      sim_obj_list.push_back(*it);
      render_list.insert(*it);
    }

    for (auto it = map_door_obj_list.begin(); it != map_door_obj_list.end(); ++it) {
      sim_obj_list.push_back(*it);
      render_list.insert(*it);
    }

    update_scale_and_center(); // get initial scale and location
  }

  void Model_state::update_scale_and_center()
  {
    scale = 3.f;
    // calculate scale
    for(int i = 0; i < int(player_list.size()); ++i)
      for(int j = 1; j < int(player_list.size()); ++j) {
        float dis_x = abs(player_list[(i + j) % int(player_list.size())]->get_location().x - player_list[i]->get_location().x);
        float dis_y = abs(player_list[(i + j) % int(player_list.size())]->get_location().y - player_list[i]->get_location().y);
        float frac = dis_x / dis_y;
        if (dis_y != 0. && frac < 4.f / 3.f) {
          if (200.f / dis_y < 0.5f) {
            if (scale > 0.5f)
              scale = 0.5f;
          }
          else if (200.0f / dis_y > 3.f) {
            if (scale > 3.f)
              scale = 3.f;
          }
          else {
            if (scale > 200.f / dis_y)
              scale = 200.f / dis_y;
            }
        }
        else {
          if (800.f / 3 / dis_x < 0.5f) {
            if (scale > 0.5f)
              scale = 0.5f;
          }
          else if (800.0f / 3 / dis_x > 3.f) {
            if (scale > 3.f)
              scale = 3.f;
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
      center_x += (*it)->get_location().x / int(player_list.size());
      center_y += (*it)->get_location().y / int(player_list.size());
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
    for (auto it = spell_list.begin(); it != spell_list.end();)
      if (!(*it)->is_active())
        it = remove_spell(*it);
      else
        ++it;
    for (auto it = monster_list.begin(); it != monster_list.end();)
      if (!(*it)->is_alive())
        it = remove_monster(*it);
      else
        ++it;
    for (auto it = next_loop_update_list.begin(); it != next_loop_update_list.end(); ++it)
      sim_obj_list.push_back(*it);
    next_loop_update_list.clear();
  }

  void Model_state::render()
  {for_each(render_list.begin(), render_list.end(), bind(&Sim_object::render, _1));}

  void Model_state::add_player(Player * player_ptr)
  {
    render_list.insert(player_ptr);
    sim_obj_list.push_back(player_ptr);
    player_list.push_back(player_ptr);
  }

  void Model_state::add_monster(Monster * monster_ptr)
  {
    render_list.insert(monster_ptr);
    sim_obj_list.push_back(monster_ptr);
    monster_list.push_back(monster_ptr);
  }

  void Model_state::add_spell(Spell * spell_ptr)
  {
    render_list.insert(spell_ptr);
    next_loop_update_list.push_back(spell_ptr);
    spell_list.push_back(spell_ptr);
  }

  void Model_state::add_map_obj(Map * map_obj_ptr)
  {
    render_list.insert(map_obj_ptr);
    sim_obj_list.push_back(map_obj_ptr);
    map_obj_list.push_back(map_obj_ptr);
  }

  void Model_state::add_map_puzzle_obj(Map * map_obj_ptr)
  {
    render_list.insert(map_obj_ptr);
    next_loop_update_list.push_back(map_obj_ptr);
    map_puzzle_obj_list.push_back(map_obj_ptr);
  }

  vector<Player *>::iterator Model_state::remove_player(Player * player_ptr)
  {
    auto it = player_list.erase(find(player_list.begin(), player_list.end(), player_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), player_ptr));
    render_list.erase(player_ptr);
    return it;
  }

  vector<Monster *>::iterator Model_state::remove_monster(Monster * monster_ptr)
  {
    auto it = monster_list.erase(find(monster_list.begin(), monster_list.end(), monster_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), monster_ptr));
    render_list.erase(monster_ptr);
    return it;
  }

  vector<Spell *>::iterator Model_state::remove_spell(Spell * spell_ptr)
  {
    auto it = spell_list.erase(find(spell_list.begin(), spell_list.end(), spell_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), spell_ptr));
    render_list.erase(spell_ptr);
    return it;
  }

  vector<Map *>::iterator Model_state::remove_map_obj(Map * map_obj_ptr)
  {
    auto it = map_obj_list.erase(find(map_obj_list.begin(), map_obj_list.end(), map_obj_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), map_obj_ptr));
    render_list.erase(map_obj_ptr);
    return it;
  }

  vector<Map *>::iterator Model_state::remove_map_puzzle_obj(Map * map_obj_ptr)
  {
    auto it = map_puzzle_obj_list.erase(find(map_puzzle_obj_list.begin(), map_puzzle_obj_list.end(), map_obj_ptr));
    sim_obj_list.erase(find(sim_obj_list.begin(), sim_obj_list.end(), map_obj_ptr));
    auto it1 = find(next_loop_update_list.begin(), next_loop_update_list.end(), map_obj_ptr);
    if (it1 != next_loop_update_list.end())
      next_loop_update_list.erase(it1);
    render_list.erase(map_obj_ptr);
    return it;
  }

  bool Model_state::can_move(const Zeni::Collision::Capsule& collision_body)
  {
    for (auto it = map_obj_list.begin(); it != map_obj_list.end(); ++it)
      if (!(*it)->can_move(collision_body))
        return false;
    return true;
  }

  bool Model_state::can_move(const Zeni::Collision::Parallelepiped& collision_body)
  {
    for (auto it = map_obj_list.begin(); it != map_obj_list.end(); ++it)
      if (!(*it)->can_move(collision_body))
        return false;
    return true;
  }

  bool Model_state::can_move_player(const Zeni::Collision::Capsule& collision_body)
  {
    bool can_move = true;
    for (auto it = map_obj_list.begin(); it != map_obj_list.end(); ++it)
      can_move = (*it)->can_move_player(collision_body);
    return can_move;
  }

  void Model_state::clear()
  {
    for (auto it = sim_obj_list.begin(); it != sim_obj_list.end(); ++it)
      delete *it;
    render_list.clear();
    next_loop_update_list.clear();
    sim_obj_list.clear();
    player_list.clear();
    monster_list.clear();
    spell_list.clear();
    map_obj_list.clear();
    map_puzzle_obj_list.clear();
    map_door_obj_list.clear();
  }

}
