#ifndef MAP_H
#define MAP_H
#include "Model_state.h"
#include "Collision.h"
#include "Sim_object.h"

namespace Flame {
    class Map : public Sim_object{
    public:
        Map(const Zeni::Point2f& location_ = Zeni::Point2f(),
            const Zeni::Vector2f &size_ = Zeni::Vector2f());


		virtual Zeni::Collision::Parallelepiped get_body() = 0;
        virtual Zeni::Point2f get_location() const{return location;};
        virtual bool can_move(const Zeni::Collision::Capsule&) = 0;
        virtual bool can_move(const Zeni::Collision::Parallelepiped&) = 0;
        virtual bool can_move_player(const Zeni::Collision::Capsule&) = 0;
        virtual void reset() = 0;
		Zeni::Point2f get_center(){return location + size / 2;};
		//virtual Zeni::Point2f get_center() = 0;
    protected:
        //  virtual void render() = 0;
        Zeni::Point2f location;
        Zeni::Vector2f size;
    };

	/*
	class Map_puzzle : public Sim_object{
	public:
		Map_puzzle(const Zeni::Point2f &location_ = Zeni::Point2f(),
			const Zeni::Vector2f &size_ = Zeni::Vector2f())
			:location(location_),
			soz;
		
		virtual Zeni::Point2f get_location() const{return location;};
		virtual void reset() = 0;
		Zeni::Point2f get_center(){return location + size / 2;};
	protected:
		Zeni::Point2f location;
		Zeni::Vector2f size;
	};
	*/


	class Map_light_beam : public Map{
	public:
		Map_light_beam(const Zeni::Point2f &location_ = Zeni::Point2f(),
					   const Zeni::Vector2f &dir_ = Zeni::Vector2f(),
             const int &parent_ = 0);
		void update(float time = 0.f);
		Zeni::Collision::Parallelepiped get_body(){return Zeni::Collision::Parallelepiped();};  
		void reset(){};
		bool can_move(const Zeni::Collision::Capsule&){return true;};
		bool can_move(const Zeni::Collision::Parallelepiped&){return true;};
		bool can_move_player(const Zeni::Collision::Capsule &){return true;};
		void render();
    void set_location(const Zeni::Point2f &location_ = Zeni::Point2f()){render_start = location_;render_end = location_;};
    void set_dir(const Zeni::Vector2f &dir_ = Zeni::Vector2f()){dir = dir_;};
    void set_player(const int player_no_ = 0){player = player_no_;};
	private:
		Zeni::Vector2f dir;
		Zeni::Collision::Capsule collision_body;
		Zeni::Point2f end_point;
		Zeni::Point2f render_start, render_end, rel_lu, rel_dr, rel_about;
		float rel_ccw, dis;
    int player;
    Map_light_beam *child;
    ~Map_light_beam(){
      if(child)
        Model_state::get_instance()->remove_map_puzzle_obj(child);
    };
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
        virtual void update(float);
        bool can_move(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        bool can_move(const Zeni::Collision::Parallelepiped& other)
        { return !collision_body.intersects(other); }
        bool can_move_player(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        void reset(){};
	//	Zeni::Point2f get_center(){return location + size / 2;}
		Zeni::Collision::Parallelepiped get_body(){return collision_body;};
	protected:
        Zeni::Collision::Parallelepiped collision_body;
        //Collision_rectangle collision_body;
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
		//Zeni::Point2f get_center(){return location + size / 2;}
        virtual void update(float);
        bool can_move(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        bool can_move(const Zeni::Collision::Parallelepiped& other)
        { return !collision_body.intersects(other); }
        bool can_move_player(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        void reset(){};
		Zeni::Collision::Parallelepiped get_body(){return collision_body;};
	private:
        Zeni::Collision::Parallelepiped collision_body;
        //Collision_rectangle collision_body;
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
	//	Zeni::Point2f get_center(){return location + size / 2;}
        Zeni::Point2f get_location()const{return Map::get_location();};
        void render();
        virtual void update(float);
        bool can_move(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        bool can_move(const Zeni::Collision::Parallelepiped& other)
        { return !collision_body.intersects(other); }
        bool can_move_player(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        void reset(){};
		Zeni::Collision::Parallelepiped get_body(){return Zeni::Collision::Parallelepiped();};        
    private:
        Zeni::Collision::Capsule collision_body;
        //Collision_circle collision_body;
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
        illuminated(false),
        illuminate_texture(illuminate_texture_)
        {};
	//	Zeni::Point2f get_center(){return location + size / 2;}
        void reset();
        bool can_move(const Zeni::Collision::Capsule&);
        bool can_move(const Zeni::Collision::Parallelepiped&)
        { return true; }
        bool can_move_player(const Zeni::Collision::Capsule& other);
        void render();
		bool get_illuminated(){return illuminated;};		        
    private:
        bool illuminated;
        Zeni::String illuminate_texture;
  };

}


#endif
