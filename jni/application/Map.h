#ifndef MAP_H
#define MAP_H

#include "Collision.h"
#include "Sim_object.h"

namespace Flame {
    class Map : public Sim_object{
    public:
        Map(const Zeni::Point2f& location_ = Zeni::Point2f(),
            const Zeni::Vector2f &size_ = Zeni::Vector2f());
        
        virtual Zeni::Point2f get_location(){return location;};
        virtual void update(float time = 0.0f){};
        virtual bool can_move(Collision_object &moving_obj_) = 0;
    protected:
        virtual void render(const Zeni::String &texture_, const Zeni::Color &filter = Zeni::Color());
        Zeni::Point2f location;
        Zeni::Vector2f size;
    };
    
    class Map_brick : public Map{
    public:
        Map_brick(const Zeni::Point2f &location_ = Zeni::Point2f(),
                  const Zeni::Vector2f &size_ = Zeni::Vector2f(),
                  const Zeni::String &texture_ = Zeni::String());
        Zeni::Point2f get_location(){return Map::get_location();};
        void render();
        void update(const float &scale_, const Zeni::Point2f &center_);
        bool can_move(Collision_object &moving_obj_);
    private:
        Collision_rectangle collision_body;
        Zeni::Quadrilateral<Zeni::Vertex2f_Texture> brick;
        Zeni::Point2f True_p0, True_p1, True_p2, True_p3;
        Zeni::Point2f Tile_p0, Tile_p1, Tile_p2, Tile_p3;
        Zeni::Vertex2f_Texture p0, p1, p2, p3;
        Zeni::String Brick_texture;
    };
}


#endif