/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include <vector>
#include <map>
#include "Collision.h"
//#include "Spell.h"
#include "map.h"
#include <cmath>
#include "Utility.h"

#include "Player.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_II {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State() :
    joystick_x(0.f),
    joystick_y(0.f),
    m_time_passed(0.f),
    m_collision_rectangle(Point2f(384., 284.), Vector2f(32., 32.)),
    p1(100.0f, 200.0f, 15.0f, Point2f(300.0f, 200.0f)),
	p2(100.0f, 200.0f, 15.0f, Point2f(500.0f, 300.0f)),
	p3(100.0f, 200.0f, 15.0f, Point2f(500.0f, 200.0f)),
	p4(100.0f, 200.0f, 15.0f, Point2f(300.0f, 300.0f))
  {
    set_pausable(true);

    m_set.start();
    
    set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 0), HORI1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 0), VERT1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 0), B1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 0), X1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 0), Y1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 0), L1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 1), HORI2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 1), VERT2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 1), B2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 1), X2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 1), Y2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 1), L2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 2), HORI3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 2), VERT3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 2), B3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 2), X3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 2), Y3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 2), L3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 3), HORI4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 3), VERT4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 3), B4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 3), X4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 3), Y4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 3), L4);
	//brick1 = new Flame::Map_brick(Point2f(0.0f, 610.0f), Vector2f(800.0f,10.0f), String("brick"));
	//rec1 = new Flame::Map_structure_rec(Point2f(200.0f, 200.0f), Vector2f(50.0f, 50.0f), Point2f(200.0f, 200.0f), Vector2f(50.0f, 50.0f), String("brick"));
	
	// wall on bottom
	Map_obj_list.push_back(new Flame::Map_brick(Point2f(-20.0f, 600.0f), 
								      		    Vector2f(820.0f,20.0f), 
		     									String("brick")));
	// wall on top
	Map_obj_list.push_back(new Flame::Map_brick(Point2f(-20.0f, -20.0f), 
								      		    Vector2f(820.0f,20.0f), 
		     									String("brick")));
	  
	// wall on left
	Map_obj_list.push_back(new Flame::Map_brick(Point2f(-20.0f, -20.0f), 
								      		    Vector2f(20.0f,620.0f), 
		     									String("brick")));

	// wall on right
	Map_obj_list.push_back(new Flame::Map_brick(Point2f(800.0f, -20.0f), 
								      		    Vector2f(20.0f,640.0f), 
		     									String("brick")));	

	// rec structure
	Map_obj_list.push_back(new Flame::Map_structure_rec(Point2f(200.0f, 200.0f), 
														Vector2f(50.0f, 50.0f), 
														Point2f(200.0f, 200.0f), 
														Vector2f(50.0f, 50.0f), 
														String("brick")));
	
	// rec half block half through
	Map_obj_list.push_back(new Flame::Map_structure_rec(Point2f(400.0f, 400.0f), 
														Vector2f(50.0f, 50.0f), 
														Point2f(400.0f, 420.0f), 
														Vector2f(50.0f, 30.0f), 
														String("house")));
  }

private:
	Flame::Player p1, p2, p3, p4;

	void on_push() {
    //get_Window().mouse_grab(true);
		get_Window().mouse_hide(true);
		get_Game().joy_mouse.enabled = false;
	}

	void on_pop() {
    //get_Window().mouse_grab(false);
		get_Window().mouse_hide(false);
		get_Game().joy_mouse.enabled = true;
	}

	void on_cover() {
	    get_Window().mouse_hide(false);
	    get_Game().joy_mouse.enabled = true;
	}

	void on_uncover() {
	    get_Window().mouse_hide(true);
	    get_Game().joy_mouse.enabled = false;
	}

	void on_event(const Zeni_Input_ID &, const float &confidence, const int &action) {
		switch(action) {
			case MENU:
				if(confidence == 1.0f) {
					Game &game = get_Game();
					game.push_state(new Popup_Menu_State);
				}
				break;

			case HORI1:
				p1.ctrl.move_hori = confidence / 2;
				break;
			
			case VERT1:
				p1.ctrl.move_vert = confidence / 2;
				break;

			case L1:
				p1.ctrl.l = confidence > 0.5 ? true : false;
				break;

			case A1:
				p1.fire(A1);
				break;

			case HORI2:
				p2.ctrl.move_hori = confidence / 2;
				break;

			case VERT2:
				p2.ctrl.move_vert = confidence / 2;
          		break;
			
			case L2:
				p2.ctrl.l = confidence > 0.5 ? true : false;
				break;

			case A2:
				p2.fire(A2);
				break;

			case HORI3:
				p3.ctrl.move_hori = confidence / 2;
				break;

			case VERT3:
				p3.ctrl.move_vert = confidence / 2;
          		break;
			
			case L3:
				p3.ctrl.l = confidence > 0.5 ? true : false;
				break;

			case A3:
				p3.fire(A3);
				break;

			case HORI4:
				p4.ctrl.move_hori = confidence / 2;
				break;

			case VERT4:
				p4.ctrl.move_vert = confidence / 2;
          		break;
			
			case L4:
				p4.ctrl.l = confidence > 0.5 ? true : false;
				break;

			case A4:
				p4.fire(A4);
				break;

			default:	
				break;
		}
	}

	float get_scale(Zeni::Point2f &True_loc_player1_, Zeni::Point2f &True_loc_player2_){
		float Dis_x = abs(True_loc_player1_.x - True_loc_player2_.x);
		float Dis_y = abs(True_loc_player1_.y - True_loc_player2_.y);
		float Frac = Dis_x / Dis_y;
    
		if (Dis_y != 0 &&Frac < 4.0f / 3.0f){
			if (200.0f / Dis_y < 1)
				return 1;
			else if (200.0f / Dis_y > 5)
				return 5;
			else
				return 200.0f / Dis_y;
		}
		else{
			if (800.0f/3/Dis_x < 1)
				return 1;
			else if (800.0f/3/Dis_x > 5)
				return 5;
			else
				return 800.0f/3/Dis_x;
		}
	}

	void update_location(Point2f &True_loc_player1,
						 Point2f &True_loc_player2,
						 Point2f &True_loc_player3,
						 Point2f &True_loc_player4){

		vector<Point2f> Player_list;
		map<float, pair<int,int> > dis_list;

		Player_list.push_back(True_loc_player1);
		Player_list.push_back(True_loc_player2);
		Player_list.push_back(True_loc_player3);
		Player_list.push_back(True_loc_player4);


		for(int i = 0; i < 4; ++i){
			for(int j = 1; j < 4; ++j)
				dis_list.insert(pair<float, pair<int,int>>(get_scale(Player_list[(i+j)%4], Player_list[i]), pair<int,int>(i,(j+i)%4)));
		}
		map<float,pair<int,int> >::iterator it = dis_list.begin();
		Center_loc_player = Point2f((True_loc_player1.x + True_loc_player2.x + True_loc_player3.x + True_loc_player4.x) / 4, 
									(True_loc_player1.y + True_loc_player2.y + True_loc_player3.y + True_loc_player4.y) / 4);

		scale = it->first;
	}

	void perform_logic() {
		const float time_passed = m_set.seconds();
		float processing_time = time_passed - m_time_passed;
		m_time_passed = time_passed;

		for (float time_step = 0.05f;
			processing_time > 0.0f;
			processing_time -= time_step) {
				p1.update(time_step, scale, Center_loc_player, &Map_obj_list);
				p2.update(time_step, scale, Center_loc_player, &Map_obj_list);
				p3.update(time_step, scale, Center_loc_player, &Map_obj_list);
				p4.update(time_step, scale, Center_loc_player, &Map_obj_list);
				update_location(p1.get_location(), p2.get_location(), p3.get_location(), p4.get_location());

				for (std::vector<Flame::Map *>::iterator it = Map_obj_list.begin();
					it != Map_obj_list.end();
					++it)
					(*it)->update(scale, Center_loc_player);
												
		}
	}

	void render()
	{
		Video &vr = get_Video();
		get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
    
		Quadrilateral<Vertex2f_Texture> map;
		Point2f Map_center(400.0f,300.0f);
	
		Point2f Map_p0 = (Point2f(0.0f, 0.0f) - Center_loc_player) * scale + Map_center;
		Point2f Map_p1 = (Point2f(0.0f, 600.0f) - Center_loc_player) * scale + Map_center;
		Point2f Map_p2 = (Point2f(800.0f, 600.0f) - Center_loc_player) * scale + Map_center;
		Point2f Map_p3 = (Point2f(800.0f, 0.0f) - Center_loc_player) * scale+ Map_center;
		Vertex2f_Texture text_p0(Map_p0, Point2f(0.0f,0.0f));
		Vertex2f_Texture text_p1(Map_p1, Point2f(0.0f, 30.0f));
		Vertex2f_Texture text_p2(Map_p2, Point2f(40.0f, 30.0f));
		Vertex2f_Texture text_p3(Map_p3, Point2f(40.0f, 0.0f));
    
		map[0] = text_p0;
		map[1] = text_p1;
		map[2] = text_p2;
		map[3] = text_p3;
    
		Material a("grass");
		map.fax_Material(&a);
		vr.render(map);
		
		for (std::vector<Flame::Map *>::iterator it = Map_obj_list.begin();
			it != Map_obj_list.end();
			++it)
			(*it)->render();
				
		p1.render();
		p2.render();
		p3.render();
		p4.render();
	}

  float joystick_x;
  float joystick_y;
  float scale;
  Zeni::Point2f Center_loc_player;
  float m_time_passed;
  Flame::Map_brick *brick1;
  Flame::Map_structure_rec *rec1;
  Chronometer<Time> m_set;
  Flame::Collision_rectangle m_collision_rectangle;
  vector<Flame::Map*> Map_obj_list;

};

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text(
#if defined(_WINDOWS)
                   "ALT+F4"
#elif defined(_MACOSX)
                   "Apple+Q"
#else
                   "Ctrl+Q"
#endif
                           " to Quit",
                   Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("zenilib Application");

      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;

      return new Title_State<Play_State, Instructions_State>("Zenipex Library\nApplication");
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
