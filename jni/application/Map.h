#ifndef MAP_H
#define MAP_H

#include "Collision.h"
#include "Sim_object.h"

namespace Flame {
    class Map : public Sim_object{
    public:
        Map(const Zeni::Point2f& location_ = Zeni::Point2f(),
            const Zeni::Vector2f &size_ = Zeni::Vector2f());
        
        virtual Zeni::Point2f get_location() const{return location;};
        virtual void update(const float &scale_, const Zeni::Point2f &center_) = 0;
        virtual bool can_move(Collision_object *moving_obj_) = 0;
		virtual bool can_move_player(Collision_object *moving_obj_) = 0;
		virtual void reset() = 0;
    protected:
      //  virtual void render() = 0;
        Zeni::Point2f location;
        Zeni::Vector2f size;
    };
    
    class Map_brick : public Map{
    public:
        Map_brick(const Zeni::Point2f &location_ = Zeni::Point2f(),
                  const Zeni::Vector2f &size_ = Zeni::Vector2f(),
				  const float &texture_size_x_ = 0.f,
				  const float &texture_size_y_ = 0.f,
                  const Zeni::String &texture_ = Zeni::String());
        Zeni::Point2f get_location()const{return Map::get_location();};
        virtual void render();
        void update(const float &scale_, const Zeni::Point2f &center_);
        virtual bool can_move_player(Collision_object *moving_obj_);
		virtual bool can_move(Collision_object *moving_obj_);
		void reset(){};
	protected:
		Collision_rectangle collision_body;
		Zeni::String Brick_texture;
		Zeni::Quadrilateral<Zeni::Vertex2f_Texture> brick;
    private:
        Zeni::Point2f True_p0, True_p1, True_p2, True_p3;
        Zeni::Point2f Tile_p0, Tile_p1, Tile_p2, Tile_p3;
        Zeni::Vertex2f_Texture p0, p1, p2, p3;
        float texture_size;
    };

	class Map_structure_rec : public Map{
	public:
		Map_structure_rec(const Zeni::Point2f &render_location_ = Zeni::Point2f(),
					  const Zeni::Vector2f &render_size_ = Zeni::Vector2f(),
					  const Zeni::Point2f &collide_location_ = Zeni::Point2f(),
					  const Zeni::Vector2f &collide_size_ = Zeni::Vector2f(),
					  const Zeni::String &texture_ = Zeni::String());
		Zeni::Point2f get_location()const{return Map::get_location();};
        void render();
        void update(const float &scale_, const Zeni::Point2f &center_);
		bool can_move_player(Collision_object *moving_obj_);
        bool can_move(Collision_object *moving_obj_);
		void reset(){};
	private:
        Collision_rectangle collision_body;
		Zeni::Point2f render_location, rel_location;
		Zeni::Vector2f render_size, rel_size;
        Zeni::String structure_texture;
	};
	
	class Map_structure_cir : public Map{
	public:
		Map_structure_cir(const Zeni::Point2f &render_center_ = Zeni::Point2f(),
					  const float &render_radius_ = 0.f,
					  const Zeni::Point2f &collide_center_ = Zeni::Point2f(),
					  const float &collide_radius_ = 0.f,
					  const Zeni::String &texture_ = Zeni::String());
		Zeni::Point2f get_location()const{return Map::get_location();};
        void render();
        void update(const float &scale_, const Zeni::Point2f &center_);
        bool can_move(Collision_object *moving_obj_);
		bool can_move_player(Collision_object *moving_obj_);
		void reset(){};
	private:
        Collision_circle collision_body;
		Zeni::Point2f render_center, rel_center;
		float render_radius;
		Zeni::Vector2f rel_size;
        Zeni::String structure_texture;
	};

	class Map_floor_illuminate : public Map_brick{
	public:
		Map_floor_illuminate(const Zeni::Point2f &location_ = Zeni::Point2f(),
							 const Zeni::Vector2f &size_ = Zeni::Vector2f(),
							 const Zeni::String &texture_ = Zeni::String(),
							 const Zeni::String &illuminate_texture_ = Zeni::String()):
		Map_brick(location_, size_, size_.x, size_.y, texture_),
		illuminate_texture(illuminate_texture_),
		illuminated(false){};
		void reset();
		bool can_move_player(Collision_object *moving_obj_);
		bool can_move(Collision_object *moving_obj_);
		void render();
	private:
		bool illuminated;
		Zeni::String illuminate_texture;
	};

}


#endif