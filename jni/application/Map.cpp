#include <zenilib.h>
#include <zeni.h>
#include <vector>
#include <sstream>
#include "Player.h"
#include "Map.h"

#include "Utility.h"
#include <cassert>
using namespace Zeni;
using namespace std;
namespace Flame {
  Map::Map(const Point2f &location_, const Vector2f &size_){
        location = location_;
        size = size_;
    }

  // --------------------------------------
  Map_brick::Map_brick(const Point2f &location_,
                         const Vector2f &size_,
                         const float &texture_size_x_,
                         const float &texture_size_y_,
                         const String &texture_)
    : Map(location_, size_),
      collision_body(Point3f(location_.x, location_.y, 0.f),
                     Vector3f(size_.x, 0.f, 0.f),
                     Vector3f(0.f, size_.y, 0.f),
                     Vector3f(0.f, 0.f, kCollision_object_height))
    {
        True_p0 = location_;
        True_p1 = location_ + Vector2f(0.0f, size_.y);
        True_p2 = location_ + size_;
        True_p3 = location_ + Vector2f(size_.x, 0.0f);
        
        Tile_p0 = Point2f(0.0f, 0.0f);
        Tile_p1 = Point2f(0.0f, size_.y / texture_size_y_);
        Tile_p2 = Point2f(size_.x / texture_size_x_, size_.y / texture_size_y_);
        Tile_p3 = Point2f(size_.x / texture_size_x_, 0.0f);
        
        Brick_texture = texture_;
    }
    
    
  void Map_brick::update(float) {
        float scale = Model_state::get_instance()->get_scale();
        Point2f center = Model_state::get_instance()->get_center_location();
		
        p0 = Vertex2f_Texture((True_p0 - center) * scale + Point2f(400.0f, 300.0f), Tile_p0);
        p1 = Vertex2f_Texture((True_p1 - center) * scale + Point2f(400.0f, 300.0f), Tile_p1);
        p2 = Vertex2f_Texture((True_p2 - center) * scale + Point2f(400.0f, 300.0f), Tile_p2);
        p3 = Vertex2f_Texture((True_p3 - center) * scale + Point2f(400.0f, 300.0f), Tile_p3);

        brick[0] = p0;
        brick[1] = p1;
        brick[2] = p2;
        brick[3] = p3;
    }
    
  void Map_brick::render() {
        Video &vr = get_Video();

        Material Brick_material(Brick_texture);
        brick.fax_Material(&Brick_material);
        vr.render(brick);
    }

  // --------------------------------------
  Map_structure_rec::Map_structure_rec(const Zeni::Point2f &render_location_ ,
                                         const Zeni::Vector2f &render_size_,
                                         const Zeni::Point2f &collide_location_,
                                         const Zeni::Vector2f &collide_size_ ,
                                         const Zeni::String &texture_ )
    : Map(collide_location_, collide_size_),
      collision_body(Point3f(collide_location_.x, collide_location_.y, 0.f),
                     Vector3f(collide_size_.x, 0.f, 0.f),
                     Vector3f(0.f, collide_size_.y, 0.f),
                     Vector3f(0.f, 0.f, kCollision_object_height)),
      render_location(render_location_),
      render_size(render_size_),
      structure_texture(texture_)
    {}
	
  void Map_structure_rec::update(float) {
        float scale = Model_state::get_instance()->get_scale();
        Point2f center = Model_state::get_instance()->get_center_location();

        rel_location = (render_location - center) * scale + Point2f(400.0f, 300.0f);
        rel_size = render_size * scale;
    }

  void Map_structure_rec::render()
    {render_image(structure_texture, rel_location, rel_location + rel_size);}

  // --------------------------------------
  Map_structure_cir::Map_structure_cir(const Zeni::Point2f &render_center_ ,
                                         const float &render_radius_,
                                         const Zeni::Point2f &collide_center_,
                                         const float &collide_radius_ ,
                                         const Zeni::String &texture_ )
    : Map(collide_center_ + Vector2f(-collide_radius_, -collide_radius_), 
      Vector2f(collide_radius_ * 2, collide_radius_ * 2)),
      collision_body(Point3f(collide_center_.x, collide_center_.y, 0.f),
                     Point3f(collide_center_.x, collide_center_.y, kCollision_object_height),
                     collide_radius_),
      render_center(render_center_),
      render_radius(render_radius_),
      structure_texture(texture_)
    {}
	
  void Map_structure_cir::update(float){
        float scale = Model_state::get_instance()->get_scale();
        Point2f center = Model_state::get_instance()->get_center_location();
		
        rel_center = (render_center - center) * scale + Point2f(400.0f, 300.0f);
        rel_size = Vector2f(render_radius *2, render_radius * 2) * scale;
    }

  void Map_structure_cir::render()
    {render_image(structure_texture, rel_center - rel_size / 2, rel_center + rel_size / 2);}
   

	// --------------------------------------
  bool Map_floor_illuminate::can_move_player(const Collision::Capsule& other)
    {
      if (collision_body.intersects(other))
        illuminated = true;
      return true;
    }

  bool Map_floor_illuminate::can_move(const Collision::Capsule&)
    {return true;}

  void Map_floor_illuminate::reset()
    {illuminated = false;}

  void Map_floor_illuminate::render() {
        Video &vr = get_Video();
        Material Brick_material;

        if(illuminated)
            Brick_material = Material(illuminate_texture);
        else
            Brick_material = Material(Brick_texture);

        brick.fax_Material(&Brick_material);
        vr.render(brick);
    }


  // --------------------------------------
  Map_light_beam::Map_light_beam(const Point2f &location_, const Vector2f &dir_, const int &player_, const String &texture_)
		:Map(location_), dir(dir_.normalized()), rel_ccw(0.f), render_start(location_), dis(0.f), player(player_),child(NULL),texture(texture_)
	{
		render_end = render_start;
		collision_body = Collision::Capsule (Point3f(render_start.x, render_start.y, kCollision_object_height / 2),
										    Vector3f(render_end.x, render_end.y, kCollision_object_height / 2),
											kCollision_object_height / 2);
	};


  void Map_light_beam::update(float time){
		vector<Map* > *map_obj_list = Model_state::get_instance()->get_map_obj_list_ptr();
		vector<Player* > *player_list = Model_state::get_instance()->get_player_list_ptr();
		
    bool player_collide = false;
    int player_collide_no = 0;
    Point2f player_location;

		float short_dis = dis + 1000.f * time;
    Vector2f new_dir;
    Map_light_beam *tmp;
		
    for (vector<Map *>::iterator it = map_obj_list->begin();
			it != map_obj_list->end();
			++it)
    {
				if (collision_body.intersects((*it)->get_body()))
        {
    			if (short_dis > (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z)))
						short_dis = (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z));
				}
		}

    int cnt = 0;
    Player *collided_player = nullptr;
		for (vector<Player *>::iterator it = player_list->begin();
			   it != player_list->end();
			   ++it)
    {
      ++cnt;
      if(cnt!=player)
      {
			  if (collision_body.intersects((*it)->get_body()))
        {
			    if (short_dis > (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z)))
          {
        	  short_dis = (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z));

            collided_player = *it;
            player_location = (*it)->get_location();
            player_collide_no = cnt;
            player_collide = true;
          }
        }
      }
    }

    if (player_collide)
    { 
      Vector2f orient = collided_player->get_current_orientation();
      Quaternion change = Quaternion::Axis_Angle(Vector3f(0.0f, 0.0f, 1.0f), -Global::pi / 2);
			Vector3f dir3 = Vector3f(dir.x, dir.y, 0.f);
			dir3 = change*dir3;

			new_dir = Vector2f(dir3.x, dir3.y);
      
      if (child != NULL)
      {
        child->set_location(player_location);
        child->set_dir(new_dir);
        child->set_player(player_collide_no);
      }
      else
      {
        tmp = new Map_light_beam(player_location, new_dir, player_collide_no,"light_beam");
        Model_state::get_instance()->add_map_puzzle_obj(tmp);   
        child = tmp;
      }
    }
    else
    {
      if(child != NULL)
      {
        Model_state::get_instance()->remove_map_puzzle_obj(child);
        delete child;
        child = NULL;
      }
    }
    
	  dis = short_dis;

	  render_end = dis * dir + render_start;
		collision_body = Collision::Capsule (Point3f(render_start.x, render_start.y, kCollision_object_height / 2),
		                           				   Vector3f(render_end.x, render_end.y, kCollision_object_height / 2),
											                   kCollision_object_height / 2);
		float scale = Model_state::get_instance()->get_scale();
		Point2f center = Model_state::get_instance()->get_center_location();
		float dis = (render_start - render_end).magnitude();
    rel_lu = (render_start + Point2f(0.f, +kCollision_object_height / 2) - center) * scale + Point2f(400.f, 300.0f);
		rel_dr = (render_start + Point2f(dis, -kCollision_object_height / 2) - center) * scale + Point2f(400.f, 300.0f);
    rel_about = (render_start -center) * scale + Point2f(400.f, 300.f);
		rel_ccw = (render_end - render_start).theta();
	};

	void Map_light_beam::render(){
		render_image(texture,rel_lu,rel_dr,-rel_ccw,1.0f,rel_about);
	};


  //-------------------------------------
  Map_door::Map_door(const Point2f &location_,
                     const Vector2f &size_,
                     const String &closed_texture_,
                     const String &open_texture_
                     )
                     :Map_structure_rec(location_,
                                        size_,
                                        location_,
                                        size_),
                     open_texture(open_texture_),
                     closed_texture(closed_texture_),
                     open(false)
  {
    
  };
  
  void Map_door::create_body(){
    collision_body=Collision::Parallelepiped(Point3f(render_location.x, render_location.y, 0.f),
                     Vector3f(render_size.x, 0.f, 0.f),
                     Vector3f(0.f, render_size.y, 0.f),
                     Vector3f(0.f, 0.f, kCollision_object_height));
  };
    
  void Map_door::render()
  {
    open = true;
    for (vector<Map*>::iterator it = trigger_list.begin();
         it != trigger_list.end();
         ++it)
    {
      if (!(*it)->triggerred()){
        open = false;
        break;
      }
    }
    
    if(open){
      destroy_body();
      render_image(open_texture, rel_location, rel_location + rel_size);
    }
    else{
      create_body();
      render_image(closed_texture, rel_location, rel_location+rel_size);
    }
  };


  // ------------------------------------
  Map_laser::Map_laser(const Point2f &location_,
    const Vector2f &dir_,
    const String &texture_)
    :Map_light_beam(location_, dir_, 0, texture_)
  {};

  void Map_laser::update(float time){
    vector<Map* > *map_obj_list = Model_state::get_instance()->get_map_obj_list_ptr();
		vector<Player* > *player_list = Model_state::get_instance()->get_player_list_ptr();
		
    bool player_collide = false;
    bool map_obj_collide = false;
    int player_collide_no;
    Point2f player_location;

		float short_dis = dis + 500.f * time;
    Vector2f new_dir;
		
    for (vector<Map *>::iterator it = map_obj_list->begin();
			it != map_obj_list->end();
			++it)
    {
				if (!dynamic_cast<Map_laser *>(*it) && !dynamic_cast<Map_gate *>(*it) && collision_body.intersects((*it)->get_body()))
        {
          map_obj_collide = true;
					if (short_dis > (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z))) {
						short_dis = (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z));
          }
				}
		}

    int cnt = 0;
    Player *collided_player;
		for (vector<Player *>::iterator it = player_list->begin();
			   it != player_list->end();
			   ++it)
    {
      ++cnt;
      if(cnt!=player)
      {
			  if (collision_body.intersects((*it)->get_body()))
        {
			    if (short_dis > (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z)))
          {
        	  short_dis = (*it)->get_body().shortest_distance(Point3f(render_start.x, render_start.y, render_start.z));

            collided_player = *it;
            player_location = (*it)->get_location();
            player_collide_no = cnt;
            player_collide = true;
            map_obj_list = false;
					}
        }
      }
    }

    dis = short_dis;

    if (player_collide)
    { 
      vector<attack_effect> empty;
      collided_player->get_hit(time*10.f, empty);       
    }
    else if (map_obj_collide)
    {
      dis = 0.f;
    }
    
	 
	  render_end = dis * dir + render_start;
		collision_body = Collision::Capsule (Point3f(render_start.x, render_start.y, kCollision_object_height / 2),
		                           				   Vector3f(render_end.x, render_end.y, kCollision_object_height / 2),
											                   kCollision_object_height / 2);
		float scale = Model_state::get_instance()->get_scale();
		Point2f center = Model_state::get_instance()->get_center_location();
		float dis = (render_start - render_end).magnitude();
    rel_lu = (render_start + Point2f(0.f, +kCollision_object_height / 2) - center) * scale + Point2f(400.f, 300.0f);
		rel_dr = (render_start + Point2f(dis, -kCollision_object_height / 2) - center) * scale + Point2f(400.f, 300.0f);
    rel_about = (render_start -center) * scale + Point2f(400.f, 300.f);
		rel_ccw = (render_end - render_start).theta();
  }

  // ----------------------------------------
  Map_transmission_gate::Map_transmission_gate(const Point2f &location_,
    const Point2f &target_position_,
    const Vector2f &size_,
    const String &texture_)
    :Map_door(location_,size_,texture_,texture_),
  target_position(target_position_)
  {};

  void Map_transmission_gate::render(){
    bool all_here = true;
    vector<Player*> *player_list = Model_state::get_instance()->get_player_list_ptr();
    for (vector<Player *>::iterator it = player_list->begin();
      it != player_list->end();
     ++it){
       if((*it)->get_body().intersects(collision_body)){
         all_here = false;
           (*it)->set_position(target_position);
       //  break;
       }
    }
      /*
    if(all_here){
      for (vector<Player *>::iterator it = player_list->begin();
          it != player_list->end();
          ++it)
      {
        (*it)->set_position(target_position);
      }
    }
       */
    render_image(open_texture, rel_location, rel_location + rel_size);

  };

  Map_lava::Map_lava(const Point2f& location_,
                     const Vector2f& size_,
                     float texture_size_x_,
                     float texture_size_y_) :
    Map_brick(location_, size_, texture_size_x_, texture_size_y_, "lava0"),
    render_timer(0.f)
    {}

  void Map_lava::update(float time)
  {
    Map_brick::update(time);
    render_timer += time;
    int n = int((render_timer - (int(render_timer) / 3 * 3)) * 10);
    stringstream ss;
    ss << "lava" << n;
    Brick_texture = ss.str();
  }

  Map_gate::Map_gate(const Point2f& location_, const Vector2f& size_, int stage_, const String& color_, bool kill_all_) :
    Map(location_, size_),
    active(false),
    stage(stage_),
    color(color_),
    kill_all(kill_all_)
    {
      Point2f c_loc = location_ + size_ / 2;
      Point3f loc3f = Point3f(c_loc.x, c_loc.y, 0.f);
      center_location = Model_state::get_instance()->get_center_location();
      scale = Model_state::get_instance()->get_scale();
      body = Collision::Capsule(loc3f, loc3f + Vector3f(0.f, 0.f, kCollision_object_height), size_.x / 2);
      rel_location = (location_ - center_location) * scale + Point2f(400.0f, 300.0f);
    }

  void Map_gate::update(float)
  {
    vector<Player *> * player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
    active = false;
    int num_players = 0;
    for (auto it = player_list_ptr->begin(); it != player_list_ptr->end(); ++it)
      if (body.intersects((*it)->get_body())) {
        active = true;
        num_players++;
      }
    if (!kill_all || Model_state::get_instance()->get_monster_list_ptr()->empty()) {
      if (num_players == int(player_list_ptr->size()))
        Model_state::get_instance()->set_next_stage(stage);
    }
    scale = Model_state::get_instance()->get_scale();
    center_location = Model_state::get_instance()->get_center_location();
    rel_location = (get_location() - center_location) * scale + Point2f(400.0f, 300.0f);
  }

  void Map_gate::render()
  {
    if (!active) {
      if (!kill_all || Model_state::get_instance()->get_monster_list_ptr()->empty())
        render_image("gate_" + color, rel_location, rel_location + scale * get_size());
      else
        render_image("gate_inactive", rel_location, rel_location + scale * get_size());
    }
    else {
      if (kill_all && !Model_state::get_instance()->get_monster_list_ptr()->empty()) {
        render_image("gate_inactive", rel_location, rel_location + scale * get_size());
        stringstream ss;
        ss << Model_state::get_instance()->get_monster_list_ptr()->size();
        String text = String(ss.str());
        text += " Monsters left! Kill them all!";
        Font &fr = get_Fonts()["system_36_800x600"];
        fr.render_text(text,
                       Point2f(200.f, 150.f),
                       get_Colors()["white"],
                       ZENI_LEFT);
      }
      else
        render_image("gate_active", rel_location, rel_location + scale * get_size());
    }
  }

}