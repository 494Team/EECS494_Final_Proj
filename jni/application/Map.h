#ifndef MAP_H
#define MAP_H
#include "Model_state.h"
#include "Collision.h"
#include "Sim_object.h"
#include "Utility.h"
#include <vector>
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
      virtual bool triggerred() = 0;
      virtual void reset() = 0;
      virtual bool walk_thru() = 0;
      virtual void add_trigger(Flame::Map *trigger_) = 0;
		  Zeni::Point2f get_center(){return location + size / 2;};
      //is this OK?
      Zeni::Vector2f get_size() {return size;}
		protected:
      virtual void render() = 0;
      Zeni::Point2f location;
      Zeni::Vector2f size;
    };



	class Map_light_beam : public Map{
	public:
		Map_light_beam(const Zeni::Point2f &location_ = Zeni::Point2f(),
					   const Zeni::Vector2f &dir_ = Zeni::Vector2f(),
             const int &parent_ = 0,
             const Zeni::String &texture_ = Zeni::String());
		virtual void update(float time = 0.f);
		Zeni::Collision::Parallelepiped get_body(){return Zeni::Collision::Parallelepiped();};  
		void reset(){};
    void add_trigger(Flame::Map*){};
		bool can_move(const Zeni::Collision::Capsule&){return true;};
		bool can_move(const Zeni::Collision::Parallelepiped&){return true;};
		bool can_move_player(const Zeni::Collision::Capsule &){return true;};
		void render();
    bool walk_thru(){return true;};
    void set_location(const Zeni::Point2f &location_ = Zeni::Point2f())
    {render_start = location_;render_end = location_;};

    void set_dir(const Zeni::Vector2f &dir_ = Zeni::Vector2f()){dir = dir_;};
    void set_player(const int player_no_ = 0){player = player_no_;};
    bool triggerred(){return true;};
    ~Map_light_beam(){
      if(child)
        Model_state::get_instance()->remove_map_puzzle_obj(child);
    };
	protected:
		Zeni::Vector2f dir;
		Zeni::Collision::Capsule collision_body;
		Zeni::Point2f end_point;
		Zeni::Point2f render_start, render_end, rel_lu, rel_dr, rel_about;
		float rel_ccw, dis;
    int player;
    Map_light_beam *child;
    Zeni::String texture;
	};
  
  class Map_laser : public Map_light_beam{
  public:
    Map_laser(const Zeni::Point2f &location_ = Zeni::Point2f(),
      const Zeni::Vector2f &dir_ = Zeni::Vector2f(),
      const Zeni::String &texture_ = Zeni::String());
    void update(float time = 0.f);
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
        void add_trigger(Flame::Map*){};
        bool can_move(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        bool can_move(const Zeni::Collision::Parallelepiped& other)
        { return !collision_body.intersects(other); }
        bool can_move_player(const Zeni::Collision::Capsule& other)
        { return !collision_body.intersects(other); }
        void reset(){};
        bool walk_thru(){return false;};
        bool triggerred(){return true;};
		    Zeni::Collision::Parallelepiped get_body(){return collision_body;};
	  protected:
        Zeni::Collision::Parallelepiped collision_body;
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
      virtual void render();
	    virtual void update(float);
      virtual void add_trigger(Flame::Map*){};
      virtual bool can_move(const Zeni::Collision::Capsule& other)
      { return !collision_body.intersects(other); }
      virtual bool can_move(const Zeni::Collision::Parallelepiped& other)
      { return !collision_body.intersects(other); }
      virtual bool can_move_player(const Zeni::Collision::Capsule& other)
      { return !collision_body.intersects(other); }
      bool walk_thru(){return false;};
      void reset(){};
      bool triggerred(){return true;};
	    Zeni::Collision::Parallelepiped get_body(){return collision_body;};
	  protected:
      Zeni::Collision::Parallelepiped collision_body;
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
      virtual void update(float);
      void add_trigger(Flame::Map*){};
      bool can_move(const Zeni::Collision::Capsule& other)
      { return !collision_body.intersects(other); }
      bool can_move(const Zeni::Collision::Parallelepiped& other)
      { return !collision_body.intersects(other); }
      bool can_move_player(const Zeni::Collision::Capsule& other)
      { return !collision_body.intersects(other); }
      void reset(){};
      bool walk_thru(){return false;};
      bool triggerred(){return true;};
	  	Zeni::Collision::Parallelepiped get_body()
      {return Zeni::Collision::Parallelepiped();};        
    private:
      Zeni::Collision::Capsule collision_body;
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
                             const Zeni::String &illuminate_texture_ = Zeni::String())
                             :Map_brick(location_, size_, size_.x, size_.y, texture_),
      illuminated(false),
      illuminate_texture(illuminate_texture_)
      {};
	    void reset();
      bool can_move(const Zeni::Collision::Capsule&);
      bool can_move(const Zeni::Collision::Parallelepiped&)
      { return true; }
      bool triggerred(){return illuminated;};
      bool walk_thru(){return true;};
      bool can_move_player(const Zeni::Collision::Capsule& other);
      void render();
	    bool get_illuminated(){return illuminated;};		        
    private:
      bool illuminated;
      Zeni::String illuminate_texture;
    };

    class Map_door : public Map_structure_rec{
    public:
      Map_door(const Zeni::Point2f &location_ = Zeni::Point2f(),
             const Zeni::Vector2f &size_ = Zeni::Point2f(),
             const Zeni::String &closed_texture_ = Zeni::String(),
             const Zeni::String &open_texture_ = Zeni::String()
             );
      virtual void render();
      virtual void add_trigger(Flame::Map *trigger_)
      {trigger_list.push_back(trigger_);};
      bool triggerred(){return true;};
      bool walk_thru(){return false;};
      virtual bool can_move(const Zeni::Collision::Capsule& other)
      {
        if (open)
          return true;
        else
          return Map_structure_rec::can_move(other);
      };
      virtual bool can_move(const Zeni::Collision::Parallelepiped& other){
        if (open)
          return true;
        else
         return Map_structure_rec::can_move(other);
      };
      virtual bool can_move_player(const Zeni::Collision::Capsule& other){
        if (open)
          return true;
        else
          return Map_structure_rec::can_move(other);
      };
    protected:
      void create_body();
      void destroy_body()
      {
        collision_body = Zeni::Collision::Parallelepiped();
      };
      Zeni::String open_texture, closed_texture;
      bool open;
      std::vector<Map* > trigger_list;   
    };

    class Map_transmission_gate : public Map_door{
    public:
      Map_transmission_gate(const Zeni::Point2f &location_ = Zeni::Point2f(),
        const Zeni::Point2f &target_position_ = Zeni::Point2f(),
        const Zeni::Vector2f &size_ = Zeni::Point2f(),
        const Zeni::String &texture_ = Zeni::String());
      void render();
      bool can_move(const Zeni::Collision::Capsule&)
      {
        return true;
      };
      bool can_move(const Zeni::Collision::Parallelepiped&){
        return true;
      };
      bool can_move_player(const Zeni::Collision::Capsule&){
        return true;
      };
    private:
      Zeni::Point2f target_position;
    };

    class Map_lava : public Map_brick {
    public:
      Map_lava(const Zeni::Point2f& location_ = Zeni::Point2f(),
               const Zeni::Vector2f& size_ = Zeni::Vector2f(),
               float texture_size_x_ = 0.f,
               float texture_size_y_ = 0.f);
      virtual Zeni::Point2f get_location() const
      {return Map_brick::get_location() + Zeni::Vector2f(0.f, -500.f);}
      virtual void update(float time = 0.f);
    private:
      float render_timer;
    };

}


#endif
