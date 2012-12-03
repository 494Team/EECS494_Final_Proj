#include "Play_level_one.h"
using namespace std;
using namespace Zeni;
using namespace Flame;

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

Play_level_one::Play_level_one()
  :m_time_passed(0.f),
  lvl(1),
  dialog(&m_set)
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

    Model_state::get_instance()->init(lvl, &m_set);
    Wanderer* wanderer = new Wanderer(Zeni::Point2f(100, 200));
    Model_state::get_instance()->add_monster(wanderer);
    Whisper* whisper_1 = new Whisper(Zeni::Point2f(400, 100));
    Model_state::get_instance()->add_monster(whisper_1);
    Redboy* redboy_inst = new Redboy(Zeni::Point2f(300, 300));
    Model_state::get_instance()->add_monster(redboy_inst);

    begin_dialog(&dialog, lvl);
}

void Play_level_one::monster_init(const int &lvl_){
  if (lvl_ == 1){
    Model_state::get_instance()->init(lvl, &m_set);
    Wanderer* wanderer = new Wanderer(Zeni::Point2f(100, 200));
    Model_state::get_instance()->add_monster(wanderer);
    Whisper* whisper_1 = new Whisper(Zeni::Point2f(400, 100));
    Model_state::get_instance()->add_monster(whisper_1);
    Redboy* redboy_inst = new Redboy(Zeni::Point2f(300, 300));
    Model_state::get_instance()->add_monster(redboy_inst);
  }
}

void Play_level_one::on_event(const Zeni_Input_ID &, const float &confidence, const int &action){
  switch(action) {
      case HORI1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.move_hori = confidence / 2;
        break;
      case VERT1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.move_vert = confidence / 2;
        break;
      case L1:
        (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.l = confidence > 0.5 ? true : false;
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

      case HORI3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.move_hori = confidence / 2;
        break;
      case VERT3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.move_vert = confidence / 2;
        break;
      case L3:
        (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.l = confidence > 0.5 ? true : false;
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

      default:
        break;
    }

    if(confidence == 1.0f) {
      switch(action) {
        case MENU: {
          Game &game = get_Game();
          game.push_state(new Popup_Menu_State);
          break;
        }
        case A1:
          if (dialog.is_goingon()) {
            dialog.proceed();
          } else {
            (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(A1);
          }
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

        case A2:
          if (dialog.is_goingon()) {
            dialog.proceed();
          } else {
            (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(A2);
          }
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

        case A3:
          if (dialog.is_goingon()) {
            dialog.proceed();
          } else {
            (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(A3);
          }
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

        case A4:
          if (dialog.is_goingon()) {
            dialog.proceed();
          } else {
            (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(A4);
          }
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
}

void Play_level_one::perform_logic(){
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

void Play_level_one::render_status_helper(int p_x, Player* p_ptr) {
  Zeni::String player_texture;
  Point2f loc(20.0f + p_x*200.0f, 20.0f);
  Point2f head_size(50.0f, 50.0f);
  switch (p_ptr->get_player_type()) {
    case SANZANG:
      player_texture = "tripitaka_head";
      break;
    case WUKONG:
      player_texture = "monkey_king_head";
      break;
    case SHASENG:
      player_texture = "friar_sand_head";
      break;
    case BAJIE:
      player_texture = "pigsy_head";
      break;
    default:
      break;
  }
  render_image(player_texture, loc, loc + head_size);
  
  float CD1percent = p_ptr->get_CD1_percent();
  float CD2percent = p_ptr->get_CD2_percent();
  float CD3percent = p_ptr->get_CD3_percent();
  float kCDbar_length = 50.0f;
  float kCDbar_width = 10.0f;
  Zeni::String kCDbar_color = "blue";

    
  Zeni::Video &vr = Zeni::get_Video();
  Zeni::Colors &cr = Zeni::get_Colors();

  Point2f CDbar_loc(loc.x, loc.y + head_size.y + 5.0f);
  Zeni::Vertex2f_Color p00(CDbar_loc, cr[kCDbar_color]);
  Zeni::Vertex2f_Color p01(CDbar_loc + Zeni::Point2f(0.0f, kCDbar_width), cr[kCDbar_color]);
  Zeni::Vertex2f_Color p02(CDbar_loc + Zeni::Point2f(CD1percent * kCDbar_length, kCDbar_width), cr[kCDbar_color]);
  Zeni::Vertex2f_Color p03(CDbar_loc + Zeni::Point2f(CD1percent * kCDbar_length, 0.0f), cr[kCDbar_color]);
  Zeni::Quadrilateral<Zeni::Vertex2f_Color> bar1(p00, p01, p02, p03);
  CDbar_loc += Point2f(kCDbar_length + 5.0f, 0.0f);
  Zeni::Vertex2f_Color p10(CDbar_loc, cr[kCDbar_color]);
  Zeni::Vertex2f_Color p11(CDbar_loc + Zeni::Point2f(0.0f, kCDbar_width), cr[kCDbar_color]);
  Zeni::Vertex2f_Color p12(CDbar_loc + Zeni::Point2f(CD2percent * kCDbar_length, kCDbar_width), cr[kCDbar_color]);
  Zeni::Vertex2f_Color p13(CDbar_loc + Zeni::Point2f(CD2percent * kCDbar_length, 0.0f), cr[kCDbar_color]);
  Zeni::Quadrilateral<Zeni::Vertex2f_Color> bar2(p10, p11, p12, p13);

  CDbar_loc += Point2f(kCDbar_length + 5.0f, 0.0f);
  Zeni::Vertex2f_Color p20(CDbar_loc, cr[kCDbar_color]);
  Zeni::Vertex2f_Color p21(CDbar_loc + Zeni::Point2f(0.0f, kCDbar_width), cr[kCDbar_color]);
  Zeni::Vertex2f_Color p22(CDbar_loc + Zeni::Point2f(CD3percent * kCDbar_length, kCDbar_width), cr[kCDbar_color]);
  Zeni::Vertex2f_Color p23(CDbar_loc + Zeni::Point2f(CD3percent * kCDbar_length, 0.0f), cr[kCDbar_color]);
  Zeni::Quadrilateral<Zeni::Vertex2f_Color> bar3(p20, p21, p22, p23);

  vr.render(bar1);
  vr.render(bar2);
  vr.render(bar3);
}

void Play_level_one::render(){
  Video &vr = get_Video();
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);

    /* render the map */
    Point2f center_location = Model_state::get_instance()->get_center_location();
    float scale = Model_state::get_instance()->get_scale();
    Quadrilateral<Vertex2f_Texture> map;
    Point2f Map_center(400.0f,300.0f);
    Point2f Map_p0 = (Point2f(0.0f, 0.0f) - center_location) * scale + Map_center;
    Point2f Map_p1 = (Point2f(0.0f, 2000.0f) - center_location) * scale + Map_center;
    Point2f Map_p2 = (Point2f(2000.0f, 2000.0f) - center_location) * scale + Map_center;
    Point2f Map_p3 = (Point2f(2000.0f, 0.0f) - center_location) * scale+ Map_center;
    Vertex2f_Texture text_p0(Map_p0, Point2f(0.0f,0.0f));
    Vertex2f_Texture text_p1(Map_p1, Point2f(0.0f, 100.0f));
    Vertex2f_Texture text_p2(Map_p2, Point2f(100.0f, 100.0f));
    Vertex2f_Texture text_p3(Map_p3, Point2f(100.0f, 0.0f));
    map[0] = text_p0;
    map[1] = text_p1;
    map[2] = text_p2;
    map[3] = text_p3;
    Material a("floor");
    map.fax_Material(&a);
    vr.render(map);

    Model_state::get_instance()->render();

    /* render the PLAYER STATUS */
    std::vector<Player *> * plist = Model_state::get_instance()->get_player_list_ptr();

    int p_x = 0;
    for (vector<Player *>::iterator it = plist->begin(); it != plist->end(); it++) {
      render_status_helper(p_x++, *it);
    }

    dialog.render();
}
 