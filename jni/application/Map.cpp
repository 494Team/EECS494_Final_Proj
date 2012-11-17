#include <zenilib.h>
#include "Map.h"
using namespace Zeni;
namespace Flame {
    Map::Map(const Point2f &location_, const Vector2f &size_){
        location = location_;
        size = size_;
    }
    
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
    
    Map_brick::Map_brick(const Point2f &location_,
                         const Vector2f &size_,
                         const String &texture_)
    :Map(location_, size_),
    collision_body(location_, size_)
    {
        True_p0 = location_;
        True_p1 = location_ + Vector2f(0.0f, size_.y);
        True_p2 = location_ + size_;
        True_p3 = location_ + Vector2f(size_.x, 0.0f);
        
        Tile_p0 = Point2f(0.0f, 0.0f);
        Tile_p1 = Point2f(0.0f, size_.y / 10.0f);
        Tile_p2 = Point2f(size_.x / 10.0f, size_.y/10.0f);
        Tile_p3 = Point2f(size_.x / 10.0f, 0.0f);
        
        Brick_texture = texture_;
    }
    
    
    void Map_brick::update(const float &scale_, const Zeni::Point2f &center_){
        p0 = Vertex2f_Texture((True_p0 - center_) * scale_ + Point2f(400.0f, 300.0f), Tile_p0);
        p1 = Vertex2f_Texture((True_p1 - center_) * scale_ + Point2f(400.0f, 300.0f), Tile_p1);
        p2 = Vertex2f_Texture((True_p2 - center_) * scale_ + Point2f(400.0f, 300.0f), Tile_p2);
        p3 = Vertex2f_Texture((True_p3 - center_) * scale_ + Point2f(400.0f, 300.0f), Tile_p3);

        brick[0] = p0;
        brick[1] = p1;
        brick[2] = p2;
        brick[3] = p3;
    }
    
    void Map_brick::render(){
        Video &vr = get_Video();
   
        
        Material Brick_material(Brick_texture);
        brick.fax_Material(&Brick_material);
        vr.render(brick);
    }
    
    bool Map_brick::can_move(Collision_object &moving_obj_){
        return moving_obj_.collide(collision_body);
    }
    
}