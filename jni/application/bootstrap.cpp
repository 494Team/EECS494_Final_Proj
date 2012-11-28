/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include "Collision.h"
//#include "Spell.h"
#include "map.h"
#include <cmath>
#include "Utility.h"
#include "Model_state.h"
#include "Player.h"
#include "Wanderer.h"
#include "Whisper.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;
using namespace Flame;

class Play_State : public Gamestate_II {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State() :
    m_time_passed(0.f)
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

    Model_state::get_instance()->init(0);
    Wanderer* wanderer = new Wanderer(Zeni::Point2f(100, 200));
    Model_state::get_instance()->add_monster(wanderer);
    Whisper* whisper = new Whisper(Zeni::Point2f(400, 100));
    Model_state::get_instance()->add_monster(whisper);
  }

private:

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
        (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.move_hori = confidence / 2;
        break;

      case VERT1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.move_vert = confidence / 2;
        break;

      case L1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      case A1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(A1);
        break;
      case B1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(B1);
        break;
      case X1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(X1);
        break;
      case Y1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(Y1);
        break;

      case HORI2:
        (*Model_state::get_instance()->get_player_list_ptr())[1]->ctrl.move_hori = confidence / 2;
        break;

      case VERT2:
        (*Model_state::get_instance()->get_player_list_ptr())[1]->ctrl.move_vert = confidence / 2;
              break;

      case L2:
        (*Model_state::get_instance()->get_player_list_ptr())[1]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      case A2:
        (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(A2);
        break;
      case B2:
        (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(B2);
        break;
      case X2:
        (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(X2);
        break;
      case Y2:
        (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(Y2);
        break;

      case HORI3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.move_hori = confidence / 2;
        break;

      case VERT3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.move_vert = confidence / 2;
              break;

      case L3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      case A3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(A3);
        break;
      case B3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(B3);
        break;
      case X3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(X3);
        break;
      case Y3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(Y3);
        break;

      case HORI4:
        (*Model_state::get_instance()->get_player_list_ptr())[3]->ctrl.move_hori = confidence / 2;
        break;

      case VERT4:
        (*Model_state::get_instance()->get_player_list_ptr())[3]->ctrl.move_vert = confidence / 2;
              break;

      case L4:
        (*Model_state::get_instance()->get_player_list_ptr())[3]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      case A4:
        (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(A4);
        break;
      case B4:
        (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(B4);
        break;
      case X4:
        (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(X4);
        break;
      case Y4:
        (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(Y4);
        break;


      default:
        break;
    }
  }

  void perform_logic() {
    const float time_passed = m_set.seconds();
    float processing_time = time_passed - m_time_passed;
    m_time_passed = time_passed;

    float time_step = 0.005f;
    while (processing_time > 0.0f + EPSILON) {
      if (processing_time < time_step) {
        time_step = processing_time;
      }
      processing_time -= time_step;
      
      auto map_obj_list_ptr = Model_state::get_instance()->get_map_obj_list_ptr();
      for (auto it = map_obj_list_ptr->begin(); it != map_obj_list_ptr->end(); ++it)
        (*it)->reset();

      Model_state::get_instance()->update(time_step);
    }
    Model_state::get_instance()->update_scale_and_center();
  }

  void render()
  {
    Video &vr = get_Video();
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);

    /* render the map */
    Point2f center_location = Model_state::get_instance()->get_center_location();
    float scale = Model_state::get_instance()->get_scale();
    Quadrilateral<Vertex2f_Texture> map;
    Point2f Map_center(400.0f,300.0f);
    Point2f Map_p0 = (Point2f(0.0f, 0.0f) - center_location) * scale + Map_center;
    Point2f Map_p1 = (Point2f(0.0f, 600.0f) - center_location) * scale + Map_center;
    Point2f Map_p2 = (Point2f(800.0f, 600.0f) - center_location) * scale + Map_center;
    Point2f Map_p3 = (Point2f(800.0f, 0.0f) - center_location) * scale+ Map_center;
    Vertex2f_Texture text_p0(Map_p0, Point2f(0.0f,0.0f));
    Vertex2f_Texture text_p1(Map_p1, Point2f(0.0f, 30.0f));
    Vertex2f_Texture text_p2(Map_p2, Point2f(40.0f, 30.0f));
    Vertex2f_Texture text_p3(Map_p3, Point2f(40.0f, 0.0f));
    map[0] = text_p0;
    map[1] = text_p1;
    map[2] = text_p2;
    map[3] = text_p3;
    Material a("floor");
    map.fax_Material(&a);
    vr.render(map);

    Model_state::get_instance()->render();
  }

  float m_time_passed;
  Chronometer<Time> m_set;

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

class Preparation_State : public Widget_Gamestate {
  Preparation_State(const Preparation_State &);
  Preparation_State operator=(const Preparation_State &);

  class Menu_Button : public Text_Button {
  public:
    Menu_Button()
      : Text_Button(Point2f(20.0f, 20.0f), Point2f(220.0f, 80.0f), "system_36_800x600", "Back to Top")
    {
    }
  private:
    void reset_statistics() {
      //
    }
    
    void on_accept() {
      reset_statistics();
      get_Game().pop_state();
    }
  } menu_button;

  class Play_Button : public Text_Button {
  public:
    Play_Button()
    : Text_Button(Point2f(520.0f, 20.0f), Point2f(780.0f, 80.0f), "system_36_800x600", "Default characters")
    {
    }
  private:
    void on_accept() {
      get_Game().push_state(new Play_State());
    }
  } play_button;

public:
  Preparation_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
    set_pausable(true);
    m_widgets.lend_Widget(play_button);
    m_widgets.lend_Widget(menu_button);
  }

private:
  void on_push() {
    get_Window().mouse_grab(true);
    //get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;
  }
 
  void on_pop() {
    get_Window().mouse_grab(false);
    //get_Window().mouse_hide(false);
    //get_Game().joy_mouse.enabled = true;
  }

  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();
    render_image("pigsy_front0",
       Point2f(600.0f, 150.0f),
       Point2f(800.0f, 500.0f));
    render_image("friar_sand_front0",
       Point2f(400.0f, 150.0f),
       Point2f(650.0f, 500.0f));
    render_image("monkey_king_front0",
       Point2f(200.0f, 150.0f),
       Point2f(450.0f, 500.0f));
    render_image("tripitaka_front0",
       Point2f(50.0f, 150.0f),
       Point2f(250.0f, 500.0f));
    render_image("p1cursor",
       Point2f(30.0f, 100.0f),
       Point2f(80.0f, 200.0f));
    render_image("p2cursor",
       Point2f(30.0f, 200.0f),
       Point2f(80.0f, 300.0f));
    render_image("p3cursor",
       Point2f(30.0f, 300.0f),
       Point2f(80.0f, 400.0f));
    render_image("p4cursor",
       Point2f(30.0f, 400.0f),
       Point2f(80.0f, 500.0f));
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
      return new Title_State<Preparation_State, Instructions_State>("Zenipex Library\nApplication");
      //return new Title_State<Play_State, Instructions_State>("Zenipex Library\nApplication");
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