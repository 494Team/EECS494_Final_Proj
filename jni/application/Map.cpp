#include <zenilib.h>
#include "Map.h"
#include "Model_state.h"
#include "Utility.h"
using namespace Zeni;
namespace Flame {
    Map::Map(const Point2f &location_, const Vector2f &size_){
        location = location_;
        size = size_;
    }
/*    
    void Map::render(const String &texture_, const Color &filter){
        render_image(texture_,
                     location,
                     location + size,
                     0.0f,
                     1.0f,
                     location + 0.5 * size,
                     false,
                     filter
                     );
    }
  */  
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

    /*
    bool Map_brick::can_move_player(Collision_object *moving_obj_)
    {return !collision_body.collide(*moving_obj_);}

    bool Map_brick::can_move(Collision_object *moving_obj_)
    {return !collision_body.collide(*moving_obj_);}
    */

    // Map_structure_rec functions;
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

    /*
	  bool Map_structure_rec::can_move(Collision_object *moving_obj_)
    {return !collision_body.collide(*moving_obj_);}
	
	  bool Map_structure_rec::can_move_player(Collision_object *moving_obj_)
    {return !collision_body.collide(*moving_obj_);}
    */

    // Map_structure_cir functions;
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

    /*
    bool Map_structure_cir::can_move(Collision_object *moving_obj_)
    {return !collision_body.collide(*moving_obj_);}

    bool Map_structure_cir::can_move_player(Collision_object *moving_obj_)
    {return !collision_body.collide(*moving_obj_);}
    

    bool Map_floor_illuminate::can_move_player(Collision_object *moving_obj_){
        if (collision_body.collide(*moving_obj_))
            illuminated = true;
        return true;
    }

    bool Map_floor_illuminate::can_move(Collision_object * moving_obj_)
    {return true;}
    */
	
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

}