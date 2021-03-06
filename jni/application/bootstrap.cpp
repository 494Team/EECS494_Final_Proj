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
#include "Play_level_one.h"
#include "Collision.h"
//#include "Spell.h"
#include "map.h"
#include <cmath>
#include "Utility.h"
#include "Model_state.h"
#include "Player.h"
#include "Wanderer.h"
#include "Whisper.h"
#include "Dialog_box.h"
#include "Redboy.h"
#include "Minimap.h"
#include "level.h"
#include <iostream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;
using namespace Flame;

class Victory_State : public Widget_Gamestate {
  Victory_State(const Victory_State &);
  Victory_State operator=(const Victory_State &);

public:
  Victory_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
    m_set.start();
    m_change.start();
    get_Sound().set_BGM("sfx/victory_bgm");
  }

private:
  Chronometer<Time> m_set, m_change;
  void on_push() {
    get_Window().mouse_hide(true);
  }
  void on_pop() {
    get_Window().mouse_hide(false);
    get_Sound().set_BGM("sfx/bgm");
  }
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Vertex2f_Texture p0 = Vertex2f_Texture(Point2f(0.f, 0.f),Point2f(0.f  -m_set.seconds(),0.f));
    Vertex2f_Texture p1 = Vertex2f_Texture(Point2f(800.f, 0.f),Point2f(1.f  -m_set.seconds(),0.f));
    Vertex2f_Texture p2 = Vertex2f_Texture(Point2f(800.f, 600.f),Point2f(1.f  -m_set.seconds(),1.f));
    Vertex2f_Texture p3 = Vertex2f_Texture(Point2f(0.f, 600.f),Point2f(0.f  -m_set.seconds(),1.f));
    Quadrilateral<Zeni::Vertex2f_Texture> bg(p0, p1, p2, p3);
    Material bg_texture("victory_bg");
    bg.fax_Material(&bg_texture);
    get_Video().render(bg);

    if (m_change.seconds() < 0.25f)
      render_image("victory0", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    else if (m_change.seconds() < 0.5f)
      render_image("victory1", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    else{
      m_change.reset();
      m_change.start();
      render_image("victory0", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    }
    Zeni::Font &l_ft = get_Fonts()["victory_ft"];
    l_ft.render_text("Victory!",
                     Point2f(400.0f, 160.0f - 0.5f*l_ft.get_text_height()),
                     get_Colors()["red"],
                     ZENI_CENTER);
    l_ft.render_text("Hit 'START' to return menu!",
                     Point2f(400.0f, 260.0f - 0.5f*l_ft.get_text_height()),
                     get_Colors()["orange"],
                     ZENI_CENTER);
  }
};

class Upgrade_state :public Gamestate_II { //public Widget_Gamestate,
  Upgrade_state(const Upgrade_state &);
  Upgrade_state operator=(const Upgrade_state &);
private:
  int skill_point_tmp[4];
  int attack_tmp[4];
  int defense_tmp[4];
  int hpmp_regen_tmp[4];
  int speed_tmp[4];
  bool confirmed[4];
  std::vector<Player *> * player_list_ptr;
  int player_number;
  Chronometer<Time>* game_time;
  Time_HQ last_highlight_move[4];
public:
  Upgrade_state(Chronometer<Time>* game_time_)
  : chosen_num(0),
    game_time(game_time_)
  {
    set_pausable(true);
    /*
    highlight_move_clock = new Chronometer<Time>;
    highlight_move_clock->start();
    last_highlight_move = highlight_move_clock->seconds() - kHighlight_move_CD;
    */
    for (int i=0; i<4; i++) {
      last_highlight_move[i] = Zeni::get_Timer_HQ().get_time();
    }
    //set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), CONFIRM1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 0), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 0), HORI1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 0), VERT1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 0), B1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), CONFIRM2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 1), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 1), HORI2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 1), VERT2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 1), B2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), CONFIRM3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 2), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 2), HORI3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 2), VERT3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 2), B3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), CONFIRM4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 3), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 3), HORI4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 3), VERT4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 3), B4);

    player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
    player_number = player_list_ptr->size();
    
    for (int i=0; i < 4; i++) {
      confirmed[i] = false;
      load_abilities(i);
      /*
      if ((*player_list_ptr)[i]->ptype == SHASENG)
        cursor_pos[i] = kCursor_min;
      else
      */
        cursor_pos[i] = kCursor_min+1;
      p_confirmed[i] = false;
      p_color[i] = Color();
    }
    m_set.start();
  }

private:
  Chronometer<Time> m_set;
  void load_abilities(const int controller) {
    /*
    for (int i=0; i<4; i++) {
      skill_point_tmp[i] = 0;
      attack_tmp[i] = 0;
      defense_tmp[i] = 0;
      hpmp_regen_tmp[i] = 0;
      speed_tmp[i] = 0;
    }*/

    skill_point_tmp[controller] = 0;
    attack_tmp[controller] = 0;
    defense_tmp[controller] = 0;
    hpmp_regen_tmp[controller] = 0;
    speed_tmp[controller] = 0;
    std::vector<Player *> * plist = Model_state::get_instance()->get_player_list_ptr();
    int list_pos = 0;
    Player * tmp = NULL;
    for (vector<Player *>::iterator it = plist->begin(); it != plist->end(); it++) 
      if(controller == Model_state::get_instance()->get_player_pos_in_list(list_pos++))
        tmp = *it;
    if(tmp){
      skill_point_tmp[controller] = tmp->get_skill_point();
      attack_tmp[controller] = tmp->attack;
      defense_tmp[controller] = tmp->defense;
      hpmp_regen_tmp[controller] = tmp->hpmp_regen;
      speed_tmp[controller] = tmp->speed;
    }
      /*
      if (controller != -1) {
          skill_point_tmp[controller] = (*it)->get_skill_point();
          attack_tmp[controller] = (*it)->attack;
          defense_tmp[controller] = (*it)->defense;
          hpmp_regen_tmp[controller] = (*it)->hpmp_regen;
          speed_tmp[controller] = (*it)->speed;
      }*/

    
  }
  void store_abilities() {
    std::vector<Player *> * plist = Model_state::get_instance()->get_player_list_ptr();
    int list_pos = 0;
    int controller;
    for (vector<Player *>::iterator it = plist->begin(); it != plist->end(); it++) {
      controller = Model_state::get_instance()->get_player_pos_in_list(list_pos++);
      if (controller != -1) {
          (*it)->set_skill_point(skill_point_tmp[controller]);
          (*it)->attack = attack_tmp[controller];
          (*it)->defense = defense_tmp[controller];
          (*it)->hpmp_regen = hpmp_regen_tmp[controller];
          (*it)->speed = speed_tmp[controller];
      }
    }
  }
  void on_push() {
    //get_Window().mouse_grab(true);
    game_time->pause_all();
    get_Window().mouse_hide(true);
    get_Game().joy_mouse.enabled = false;
  }
  void on_pop() {
    //get_Window().mouse_grab(false);
    store_abilities();
    get_Window().mouse_hide(false);
    get_Game().joy_mouse.enabled = true;
    game_time->unpause_all();
  }
  void on_cover() {
      get_Window().mouse_hide(false);
      get_Game().joy_mouse.enabled = true;
  }
  void on_uncover() {
      get_Window().mouse_hide(true);
      get_Game().joy_mouse.enabled = false;
  }

  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }
  
  void highlight_move(const int controller, const bool is_down) {
    const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
    Time_HQ* last_highlight_move_ptr = &last_highlight_move[controller];
    if (float(current_time.get_seconds_since(*last_highlight_move_ptr) > kHighlight_move_CD)) {
       *last_highlight_move_ptr = current_time;
        int list_index = Model_state::get_instance()->get_player_list_index(controller);
        if (!p_confirmed[controller]) {
          if (is_down && cursor_pos[controller] < kShop_cursor_max-1) {
            cursor_pos[controller]++;
          } else if (!is_down && cursor_pos[controller] > 1) {
            cursor_pos[controller]--;
          } else if (!is_down &&
                     (*player_list_ptr)[list_index]->ptype == SHASENG &&
                     cursor_pos[controller] > 0) {
            cursor_pos[controller]--;
          }
        }
    }
  }

  /*
  SANZANG,
  WUKONG,
  SHASENG,
  BAJIE
  */
  bool p_confirmed[4];
  kPlayer_type chosen_char[4];
  int cursor_pos[4];
  Color p_color[4];
  int chosen_num;

  void move_and_confirm(const int controller) {
    //int list_index = Model_state::get_instance()->get_player_list_index(controller);
    if (controller >=0 && controller <= 3) {
      cursor_pos[controller] = 5;
      click_a(controller);
    }
  }
  void click_a(const int controller) {
    int list_index = Model_state::get_instance()->get_player_list_index(controller);
    if (list_index != -1 && !p_confirmed[controller]) {
      int pos = cursor_pos[controller];
      switch (pos) {
        case 0: //switch arrow
          if ((*player_list_ptr)[list_index]->ptype == SHASENG) {
            //switch_magic_arrow();
            (*player_list_ptr)[list_index]->switch_magic_arrow();
          }
          break;
        case 1:
          if (skill_point_tmp[controller] > 0 && attack_tmp[controller] < kAttack_max) {
            attack_tmp[controller]++;
            skill_point_tmp[controller]--;
          }
          break;
        case 2:
          if (skill_point_tmp[controller] > 0 && defense_tmp[controller] < kDefense_max) {
            defense_tmp[controller]++;
            skill_point_tmp[controller]--;
          }
          break;
        case 3:
          if (skill_point_tmp[controller] > 0 && hpmp_regen_tmp[controller] < kHpmp_regen_max) {
            hpmp_regen_tmp[controller]++;
            skill_point_tmp[controller]--;
          }
          break;
        case 4:
          if (skill_point_tmp[controller] > 0 && speed_tmp[controller] < kSpeed_max) {
            speed_tmp[controller]++;
            skill_point_tmp[controller]--;
          }
          break;
        case 5: //confirm
          p_confirmed[controller] = true;
          chosen_num++;
          break;
        case 6: //cancel
          load_abilities(controller);
          break;
        default:
          break;
      }
      if (chosen_num == player_number) {
        get_Game().pop_state();
        //get_Game().push_state(new Play_State());
      }
    }
  }

  void on_event(const Zeni_Input_ID &, const float &confidence, const int &action) {
    switch(action) {
      case VERT1:
        if (confidence >= kVert_move_threshold)
          highlight_move(0, true);
        else if (confidence <= -kVert_move_threshold)
          highlight_move(0, false);
        break;
      case VERT2:
        if (confidence >= kVert_move_threshold)
          highlight_move(1, true);
        else if (confidence <= -kVert_move_threshold)
          highlight_move(1, false);
        break;
      case VERT3:
        if (confidence >= kVert_move_threshold)
          highlight_move(2, true);
        else if (confidence <= -kVert_move_threshold)
          highlight_move(2, false);
        break;
      case VERT4:
        if (confidence >= kVert_move_threshold)
          highlight_move(3, true);
        else if (confidence <= -kVert_move_threshold)
          highlight_move(3, false);
        break;
      default:
        break;
    }
    //get_Game().push_state(new Play_State());
    if(confidence >= 1.0f) {
      switch(action) {
        case BACK:
          break;
        case CONFIRM1: 
          move_and_confirm(0);
          break;
        case CONFIRM2: 
          move_and_confirm(1);
          break;
        case CONFIRM3: 
          move_and_confirm(2);
          break;
        case CONFIRM4: 
          move_and_confirm(3);
          break;

        case A1:
          click_a(0);
          break;
        case A2:
          click_a(1);
          break;
        case A3:
          click_a(2);
          break;
        case A4:
          click_a(3);
          break;
        default:
          break;
      }
    }
  }
  void render_player_helper(int controller, Player* p_ptr) {
    Zeni::String player_texture;
    Point2f loc;
    
    float margin = 20.0f;
    switch (controller) {
      case 0:
        //loc = Point2f(margin, margin * 2 + 40.0f);
        loc = Point2f(30.f, 70.f);
        render_image("upgrade_p1",Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
        break;
      case 1:
        //loc = Point2f(margin + 400.0f, margin * 2+ 40.0f);
        loc = Point2f(415.f, 70.f);
        render_image("upgrade_p2",Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
        break;
      case 2:
        //loc = Point2f(margin, margin * 2 + 300.0f);
        loc = Point2f(30.f, 335.f);
        render_image("upgrade_p3",Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
        break;
      default: //case 3:
        //loc = Point2f(margin + 400.0f, margin * 2 + 300.0f);
        render_image("upgrade_p4",Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
        loc = Point2f(415.f, 335.f);
        break;
    }

    //Point2f size(140.0f, 200.0f);
    Point2f size(335.f, 195.f);
    switch (p_ptr->get_player_type()) {
      case SANZANG:
        //player_texture = "tripitaka_front0";
        player_texture = "upgrade_sanzang";
        break;
      case WUKONG:
        //player_texture = "monkey_king_front0";
        player_texture = "upgrade_wukong";
        break;
      case SHASENG:
        //player_texture = "friar_sand_front0";
        player_texture = "upgrade_shaseng";
        break;
      case BAJIE:
        //player_texture = "pigsy_front0";
        player_texture = "upgrade_bajie";
        break;
      default:
        break;
    }
    //if(m_set.seconds()< 0.25f)
      render_image(player_texture+"0", loc, loc + size);
    //else if (m_set.seconds() < 0.5f)
      //render_image(player_texture+"1", loc, loc + size);
    //else{
      //m_set.reset();
      //m_set.start();
      //render_image(player_texture+"0", loc, loc + size);
    //}


    //render status
     
      Zeni::Font &fr = get_Fonts()["shop_ft"];
    float button_size = fr.get_text_height() * 0.8f;    
 
    char* str = new char[10];
    //sprintf(str, "%d", controller+1);//speed_lvl);
    String text_buf;
    //= "Player ";
    /*text_buf += str;
    fr.render_text(text_buf,
                   loc - Point2f(0.0f, 30.0f),
                   get_Colors()["white"],
                   ZENI_LEFT);
                   */
    //loc += Point2f(150.f, 10.f);
    Point2f offset = Point2f(150.f, 10.f);
    //Point2f(140.0f, 40.0f)
    Point2f Bar_loc[7];
    Bar_loc[0] = loc + offset - Point2f(button_size * 1.1f, 2.5f*fr.get_text_height());
    Bar_loc[1] = loc + offset;
    Bar_loc[2] = Bar_loc[1] + Point2f(0.0f, fr.get_text_height());
    Bar_loc[3] = Bar_loc[2] + Point2f(0.0f, fr.get_text_height());
    Bar_loc[4] = Bar_loc[3] + Point2f(0.0f, fr.get_text_height());
    Bar_loc[5] = Bar_loc[4] + Point2f(0.0f, 2*fr.get_text_height());
    Bar_loc[6] = Bar_loc[5] + Point2f(0.0f, fr.get_text_height());

    if (p_ptr->get_player_type() == SHASENG) 
    {
      text_buf = "Switch magic arrow:";
      Zeni::String ice_button_color, fire_button_color;
      //= "white";
      if (p_ptr->is_fire_magic_arrow()) {
        ice_button_color = "white_light";
        fire_button_color = "red";
      } else {
        ice_button_color = "blue";
        fire_button_color = "white_light";
      }
      fr.render_text(text_buf,
                     Bar_loc[0],
                     get_Colors()["white"],
                     ZENI_LEFT);
      fr.render_text("ICE",
                     Bar_loc[0] + Point2f(40.0f, fr.get_text_height()),
                     get_Colors()[ice_button_color],
                     ZENI_LEFT);
      fr.render_text("FIRE",
                     Bar_loc[0] + Point2f(120.0f, fr.get_text_height()),
                     get_Colors()[fire_button_color],
                     ZENI_LEFT);
    }

    sprintf(str, "%d", attack_tmp[controller]);//speed_lvl);
    text_buf = "Attack: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[1],
                   get_Colors()["white"],
                   ZENI_LEFT);

    
    //loc += Point2f(0.0f, fr.get_text_height());
    sprintf(str, "%d", defense_tmp[controller]);//speed_lvl);
    text_buf = "Defense: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[2],
                   get_Colors()["white"],
                   ZENI_LEFT);
    

    //loc += Point2f(0.0f, fr.get_text_height());
    sprintf(str, "%d", hpmp_regen_tmp[controller]);//speed_lvl);
    text_buf = "Hp&Mp regen: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[3],
                   get_Colors()["white"],
                   ZENI_LEFT);

    //loc += Point2f(0.0f, fr.get_text_height());
    sprintf(str, "%d", speed_tmp[controller]);//speed_lvl);
    text_buf = "Speed: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[4],
                   get_Colors()["white"],
                   ZENI_LEFT);
    Color filter;
    Color disable_button = Color(1.0f, 0.5f, 0.5f, 0.5f);
    filter = cursor_pos[controller] == 1 ? Color() : disable_button;
    if (skill_point_tmp[controller]!=0)
      render_image("plus_button", Bar_loc[1] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[1] + Point2f(-button_size*0.1f, button_size), false, filter); 
    filter = cursor_pos[controller] == 2 ? Color() : disable_button;
    if (skill_point_tmp[controller]!=0)
      render_image("plus_button", Bar_loc[2] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[2] + Point2f(-button_size*0.1f, button_size), false, filter);
    filter = cursor_pos[controller] == 3 ? Color() : disable_button;
    if (skill_point_tmp[controller]!=0)
      render_image("plus_button", Bar_loc[3] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[3] + Point2f(-button_size*0.1f, button_size), false, filter); 
    filter = cursor_pos[controller] == 4 ? Color() : disable_button;
    if (skill_point_tmp[controller]!=0)
      render_image("plus_button", Bar_loc[4] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[4] + Point2f(-button_size*0.1f, button_size), false, filter); 

    sprintf(str, "%d", skill_point_tmp[controller]);//speed_lvl);
    text_buf = "Upgrade points: ";
    text_buf += str;
    fr.render_text(text_buf,
                   Bar_loc[4] + Point2f(40.0f, fr.get_text_height()),
                   get_Colors()["red"],
                   ZENI_LEFT);
    //loc += Point2f(0.0f, 1.5f * fr.get_text_height());
    fr.render_text("Confirm",
                   Bar_loc[5],
                   get_Colors()["white"],
                   ZENI_LEFT);
    //loc += Point2f(0.0f, fr.get_text_height());
    fr.render_text("Cancel",
                   Bar_loc[6],
                   get_Colors()["white"],
                   ZENI_LEFT);

    
    fr.render_text("* To continue the game, all players need to confirm",
                   Point2f(200.0f, 560.0f),
                   get_Colors()["white"],
                   ZENI_LEFT);
    int player_list_index = Model_state::get_instance()->get_player_list_index(controller);
    float Shaseng_indent = (*player_list_ptr)[player_list_index]->ptype == SHASENG ? 0.f:0.f;//button_size*1.1f : 0.0f;
    Point2f highlight_size(200.0f + 2*Shaseng_indent, fr.get_text_height());
    //render_image("highlight", Bar_loc[cursor_pos[player_list_index]] - Point2f(button_size * 1.1f - Shaseng_indent, 0.0f), Bar_loc[cursor_pos[player_list_index]] + highlight_size);
    render_image("highlight", Bar_loc[cursor_pos[controller]] - Point2f(button_size * 1.1f - Shaseng_indent, 0.0f), Bar_loc[cursor_pos[controller]] + highlight_size);
    /*
    Zeni::Font &fr2 = get_Fonts()["shop_title"];
    fr2.render_text("Upgrade Abilities",
                   Point2f(400.0f, 10.0f),
                   get_Colors()["white"],
                   ZENI_CENTER);
                   */
  }

  void render() {
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
    int list_pos = 0;
    int controller;
    render_image("upgrade_bg",Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    for (vector<Player *>::iterator it = player_list_ptr->begin(); it != player_list_ptr->end(); it++) {
      controller = Model_state::get_instance()->get_player_pos_in_list(list_pos++);
      if (controller != -1)
        render_player_helper(controller, *it);
    }
  }
};

class Play_State : public Gamestate_II {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);
  Dialog_box dialog;
  int revival_num;
  int stage;
  int curr_lvl;
  std::vector<Level*> levels;
  Time_HQ dialog_entering_time;
public:
  Play_State() :
    m_time_passed(0.f),
    stage(1),
    show_die(false),
    dialog(&m_set, &dialog_entering_time),
    victory(false),
    prev_lvl(0),
    now_lvl(0),
    render_lvl_up_effect(false),
    lvl_effect_time(0.f)
  {
    revival_num = Model_state::get_instance()->get_initial_player_num() * kRevival_max_per_player;
    set_pausable(true);
    m_cd_start[0] = false;
    m_cd_start[1] = false;
    m_cd_start[2] = false;
    m_cd_start[3] = false;

    set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), MENU1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 0), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 0), HORI1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 0), VERT1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 0), B1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 0), X1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 0), Y1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 0), L1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), MENU2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 1), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 1), HORI2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 1), VERT2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 1), B2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 1), X2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 1), Y2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 1), L2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), MENU3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 2), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 2), HORI3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 2), VERT3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 2), B3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 2), X3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 2), Y3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 2), L3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), MENU4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 3), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 3), HORI4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 3), VERT4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 3), B4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 3), X4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 3), Y4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 3), L4);

    levels.push_back(new Level_1());
    levels.push_back(new Level_2());


    lvl_up_effect_pos = Point2f(-390.f, 116.f);
    Model_state::get_instance()->init(stage, &m_set);
    //curr_lvl = 1;
    //set_stage(4);
    curr_lvl = 0;
    set_stage(stage);
    //!!! test
    //dialog.start(stage);
    //begin_dialog(&dialog, stage);
    m_set.start();
  }

private:
  void set_stage(int stage_, bool restart = false, bool change_lvl = false) {
    //end the player action before changing stage
    //for example cudgel_fury
    for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
        it != Model_state::get_instance()->get_player_list_ptr()->end();
        ++it){
      (*it)->end_action();
    }

    Model_state::get_instance()->clear_without_player();
    if (!change_lvl && curr_lvl == 0) {
      float x = 440.f;
      for (auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
           it != Model_state::get_instance()->get_player_list_ptr()->end();
           ++it) {
        (*it)->set_position(Point2f(x, 980));
        x += 40.f;
      }
    }
    else {
      int prev_stage = Model_state::get_instance()->get_prev_stage();
      if (stage_ == 1) {
        if (!prev_stage || restart) {
          float x = 660;
          for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
              it != Model_state::get_instance()->get_player_list_ptr()->end();
              ++it){
            (*it)->set_position(Point2f(x,3000));
            x += 40.f;
          }
        }
        else if (stage == 2) {
          int i = 0;
          for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
              it != Model_state::get_instance()->get_player_list_ptr()->end();
              ++it) {
            if (i == 0)
              (*it)->set_position(Point2f(1230.f, 30.f));
            else if (i == 1)
              (*it)->set_position(Point2f(1410.f, 30.f));
            else if (i == 2)
              (*it)->set_position(Point2f(1230.f, 210.f));
            else
              (*it)->set_position(Point2f(1410.f, 210.f));
            i++;
          }
        }
        else {
          int i = 0;
          for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
              it != Model_state::get_instance()->get_player_list_ptr()->end();
              ++it) {
            if (i == 0)
              (*it)->set_position(Point2f(5790.f, 4590.f));
            else if (i == 1)
              (*it)->set_position(Point2f(5970.f, 4590.f));
            else if (i == 2)
              (*it)->set_position(Point2f(5690.f, 4770.f));
            else
              (*it)->set_position(Point2f(5970.f, 4590.f));
            i++;
          }
        }
      }
      else if (stage_ == 2){
        if (stage == 1) {
          int i = 0;
          for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
              it != Model_state::get_instance()->get_player_list_ptr()->end();
              ++it) {
            if (i == 0)
              (*it)->set_position(Point2f(1230.f, 5790.f));
            else if (i == 1)
              (*it)->set_position(Point2f(1410.f, 5790.f));
            else if (i == 2)
              (*it)->set_position(Point2f(1230.f, 5970.f));
            else
              (*it)->set_position(Point2f(1410.f, 5970.f));
            i++;
          }
        }
        else {
          int i = 0;
          for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
              it != Model_state::get_instance()->get_player_list_ptr()->end();
              ++it) {
            if (i == 0)
              (*it)->set_position(Point2f(4590.f, 30.f));
            else if (i == 1)
              (*it)->set_position(Point2f(4770.f, 30.f));
            else if (i == 2)
              (*it)->set_position(Point2f(4590.f, 210.f));
            else
              (*it)->set_position(Point2f(4770.f, 210.f));
            i++;
          }
        }
      }
      else if (stage_ == 3){
       if (stage == 1) {
          int i = 0;
          for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
              it != Model_state::get_instance()->get_player_list_ptr()->end();
              ++it) {
            if (i == 0)
              (*it)->set_position(Point2f(30.f, 1230.f));
            else if (i == 1)
              (*it)->set_position(Point2f(210.f, 1230.f));
            else if (i == 2)
              (*it)->set_position(Point2f(30.f, 1410.f));
            else
              (*it)->set_position(Point2f(210.f, 1410.f));
            i++;
          }
        }
        else {
          int i = 0;
          for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
              it != Model_state::get_instance()->get_player_list_ptr()->end();
              ++it) {
            if (i == 0)
              (*it)->set_position(Point2f(5790.f, 1230.f));
            else if (i == 1)
              (*it)->set_position(Point2f(5970.f, 1230.f));
            else if (i == 2)
              (*it)->set_position(Point2f(5790.f, 1410.f));
            else
              (*it)->set_position(Point2f(5970.f, 1410.f));
            i++;
          }
        }
      }
      else {
        float x = 240.f;
        for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
            it != Model_state::get_instance()->get_player_list_ptr()->end();
            ++it) {
          (*it)->set_position(Point2f(x, 580.f));
          x += 40.f;
        }
      }
    }


    //dialog section
    if (curr_lvl == 0 && stage_ == 1) {
      dialog.start(1);
    } else if (curr_lvl == 0 && stage_ == 3) {
      dialog.start(2);
    } else if (curr_lvl == 0 && stage == 3 && stage_ == 1) {
      dialog.start(3);
    } else if (curr_lvl == 1 && stage_ == 4) {
      dialog.start(4);
    }
    if (change_lvl)
      set_level(1);
    stage = stage_;

    levels[curr_lvl]->init_map(stage_);
    std::vector<Map *> map_list = levels[curr_lvl]->get_map_list();
    for (auto it = map_list.begin(); it != map_list.end(); ++it)
      Model_state::get_instance()->add_map_obj(*it);
    std::vector<Monster*> monster_list = levels[curr_lvl]->get_monster_list();
    for (auto it = monster_list.begin(); it != monster_list.end(); ++it)
      Model_state::get_instance()->add_monster(*it);
    if (curr_lvl == 0 && stage_ == 3)
      Model_state::get_instance()->set_prev_stage(0);
    else if (restart || (curr_lvl == 0 && stage == 3 && stage_ == 1))
      revival_num = Model_state::get_instance()->get_initial_player_num() * kRevival_max_per_player;
    else
      Model_state::get_instance()->set_prev_stage(stage);
    Model_state::get_instance()->set_next_stage(0);



  }

  void set_level(const int level_) {
    curr_lvl = level_;
  }

  void restart_stage() {
    std::vector<Player*>* dead_player_list = Model_state::get_instance()->get_dead_player_list_ptr();
    for (auto it = dead_player_list->begin(); it != dead_player_list->end();) {
      it = Model_state::get_instance()->player_rise_without_setting_pos(*it);
    }
    //revival_num = Model_state::get_instance()->get_initial_player_num() * kRevival_max_per_player;
    set_stage(1, true);
  }
    
  void begin_dialog(Dialog_box* dialog_ptr, int stage) {
    //m_set.pause_all();
    dialog_ptr->start(stage);
  }

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

  void get_into_upgradeshop() {
    get_Game().push_state(new Upgrade_state(&m_set));
  }

  void on_event(const Zeni_Input_ID &, const float &confidence, const int &action) {
    Player* p_ptr;
    std::vector<Player*>* player_list_ptr = (Model_state::get_instance()->get_player_list_ptr());
    int controller = 0;
    switch(action) {
      case BACK: {
        Game &game = get_Game();
        game.push_state(new Popup_Menu_State);
        break;
      }

      case HORI1:
      case VERT1:
      case L1:
      case A1:
      case B1:
      case X1:
      case Y1:
      case MENU1:
        controller = 0;
        break;

      case HORI2:
      case VERT2:
      case L2:
      case A2:
      case B2:
      case X2:
      case Y2:
      case MENU2:
        controller = 1;
        break;

      case HORI3:
      case VERT3:
      case L3:
      case A3:
      case B3:
      case X3:
      case Y3:
      case MENU3:
        controller = 2;
        break;

      case HORI4:
      case VERT4:
      case L4:
      case A4:
      case B4:
      case X4:
      case Y4:
      case MENU4:
        controller = 3;
        break;

      default: //MENU
        break;
    }
    
    //cerr << "********action: " << action << endl;
    int list_pos = Model_state::get_instance()->get_player_list_index(controller);
    //int player_pos_in_list = Model_state::get_instance()->get_player_pos_in_list(controller);
    //if (player_pos_in_list >= 0 && player_pos_in_list <= 3) {
    if (list_pos != -1) {
      //control of alive players
      p_ptr = player_list_ptr->at(list_pos);
      /*
      cerr << "********ptype: " << p_ptr->ptype << endl;
      cerr << "********controllable: " << p_ptr->controllable() << endl;
      cerr << "********is_hitback(): " << p_ptr->is_hitback() << endl;
      cerr << "********is_charging(): " << p_ptr->is_charging() << endl;
      cerr << "********is_alive(): " << p_ptr->is_alive() << endl;
      */
      //p_ptr = player_list_ptr->at(player_pos_in_list);
        if (p_ptr->controllable()) {
          switch(action) {
            case MENU1: 
            case MENU2: 
            case MENU3: 
            case MENU4: 
              if (confidence >= 1.0f && !dialog.is_goingon()) {
                for (auto it=player_list_ptr->begin(); it!=player_list_ptr->end(); ++it) {
                  (*it)->end_action();
                }
                get_into_upgradeshop();
              }
              break;

            case HORI1:
            case HORI2:
            case HORI3:
            case HORI4:
              p_ptr->ctrl.move_hori = confidence/2;
              break;
            case VERT1:
            case VERT2:
            case VERT3:
            case VERT4:
              p_ptr->ctrl.move_vert = confidence/2;
              break;
            case L1:
            case L2:
            case L3:
            case L4:
              p_ptr->ctrl.l = confidence>0.5 ? true:false;
              break;
            case A1:
            case A2:
            case A3:
            case A4:
              if (confidence > 0.9f) {
                if (dialog.is_goingon()) {
                  dialog.proceed();
                } else {
                  p_ptr->fire(A1);
                }
              }
              break;

            case B1:
            case B2:
            case B3:
            case B4:
              if (confidence > 0.9f) {
                p_ptr->fire(B1);
              }
              break;

            case X1:
            case X2:
            case X3:
            case X4:
              if (confidence > 0.9f) {
                p_ptr->fire(X1);
              } else if (confidence < 0.1f) {
                p_ptr->button_x_release();
              }
              break;

            case Y1:
            case Y2:
            case Y3:
            case Y4:
              if (confidence > 0.9f) {
                  p_ptr->fire(Y1);
              }
              break;
            default:
              break;
          }
        }
    } else {
      //control of dead players
      std::vector<Player*>* dead_player_list_ptr = (Model_state::get_instance()->get_dead_player_list_ptr());
      if (player_list_ptr->empty()) {
        //revive all
          switch(action) {
          case A1:
          case A2:
          case A3:
          case A4:
            /*
            //cannot use rise_from_dead_list() because no one is alive now
            for (auto it = dead_player_list_ptr->begin(); it != dead_player_list_ptr->end();) {
              it = Model_state::get_instance()->player_rise_from_dead_list(*it);
            }
            */
            restart_stage();
            break;
          default:
            break;
          }
      } else {
        //revive any one
          switch(action) {
          case A1:
            for (auto it = dead_player_list_ptr->begin(); it != dead_player_list_ptr->end(); ++it) {
              if (revival_num > 0 && (*it)->get_controller() == 0 && m_resurge[0].seconds() > 5.f) {
                Model_state::get_instance()->player_rise_from_dead_list(*it);
                revival_num--;
                break;
              }
            }
            break;
          case A2:
            for (auto it = dead_player_list_ptr->begin(); it != dead_player_list_ptr->end(); ++it) {
              if (revival_num > 0 && (*it)->get_controller() == 1 && m_resurge[1].seconds() > 5.f) {
                Model_state::get_instance()->player_rise_from_dead_list(*it);
                revival_num--;
                break;
              }
            }
            break;
          case A3:
            for (auto it = dead_player_list_ptr->begin(); it != dead_player_list_ptr->end(); ++it) {
              if (revival_num > 0 && (*it)->get_controller() == 2 && m_resurge[2].seconds() > 5.f) {
                Model_state::get_instance()->player_rise_from_dead_list(*it);
                revival_num--;
                break;
              }
            }
            break;
          case A4:
            for (auto it = dead_player_list_ptr->begin(); it != dead_player_list_ptr->end(); ++it) {
              if (revival_num > 0 && (*it)->get_controller() == 3 && m_resurge[3].seconds() > 5.f) {
                Model_state::get_instance()->player_rise_from_dead_list(*it);
                revival_num--;
                break;
              }
            }
            break;
          default:
            break;
          }
      }
    }
  }
  void victory_dialog_begin() {
    dialog.start(5);
  }
  void victory_dialog_end() {
    get_Game().pop_state();
    get_Game().push_state(new Victory_State());
  }
  bool victory;
  float lvl_effect_time;
  void perform_logic() {
    const float time_passed = m_set.seconds();
    float processing_time = time_passed - m_time_passed;
    m_time_passed = time_passed;
    lvl_effect_time = processing_time;
    if (Model_state::get_instance()->get_player_list_ptr()->empty() && !show_die) {
        show_die = true;
    }
    if (!Model_state::get_instance()->get_player_list_ptr()->empty() && show_die) {
        show_die = false;
    }
    //judge victory
    if (curr_lvl == 1 &&
        stage == 4 &&
        !Model_state::get_instance()->get_player_list_ptr()->empty() &&
        Model_state::get_instance()->get_monster_list_ptr()->empty()) {
      if (!victory) {
        victory = true;
        victory_dialog_begin();
      } else if (victory && !dialog.is_goingon()) {
        victory_dialog_end();
      }
    }
    //dialog pausing start
    const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
    if (current_time.get_seconds_since(dialog_entering_time) > kDialog_render_delay &&
        m_set.is_running() &&
        dialog.is_goingon()) {
      m_set.pause_all();
    }

    float time_step = 0.01f;
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
    if (!Model_state::get_instance()->get_player_list_ptr()->empty()) {
        Model_state::get_instance()->update_scale_and_center();
    }
    if (curr_lvl == 0 && stage == 3 && Model_state::get_instance()->get_monster_list_ptr()->empty() && Model_state::get_instance()->get_map_obj_list_ptr()->size() < 9) {
      Model_state::get_instance()->add_map_obj(new Map_gate(Point2f(380.f, 380.f), Vector2f(240.f, 240.f), 1, "boss"));
    }
    int next_stage = Model_state::get_instance()->get_next_stage();
    if (next_stage) {
      if (curr_lvl == 0 && next_stage == 1)
        set_stage(next_stage, false, true);
      else
        set_stage(next_stage);
    }
  }

  void render_dead_status_helper(int controller, Player* p_ptr){
    Zeni::String player_texture;//, player_skill_texture;
    Point2f loc;
    switch (controller) {
      case 0:
        loc = Point2f(20.0f, 20.0f);
        break;
      case 1:
        loc = Point2f(20.0f + 180.0f, 20.0f);
        break;
      case 2:
        loc = Point2f(20.0f + 460.0f, 20.0f);
        break;
      default: //case 3:
        loc = Point2f(20.0f + 620.0f, 20.0f);
        break;
    }
    int s = (int)m_resurge[controller].seconds();
    if(s < 6){
      String remain(itoa(5-s));
      Zeni::Font &l_ft = get_Fonts()["dead_ft"];
       l_ft.render_text(remain,
                   loc+Vector2f(90.0f, 0.0f),
                   get_Colors()["yellow"],
                   ZENI_LEFT);
    }else{
      if(!m_dead[controller].is_running())
        m_dead[controller].start();
      if(m_dead[controller].seconds()<0.25f)
        render_image("dead", loc+Vector2f(0.f, 20.f), loc + Vector2f(180.f, 50.f));
      else if (m_dead[controller].seconds()>0.5f){
        m_dead[controller].reset();
        m_dead[controller].start();
      }
    }
    
    

    
    
    //Zeni::Video &vr = Zeni::get_Video();
    //Zeni::Colors &cr = Zeni::get_Colors();
/*
    Point2f Hpbar_loc(loc.x + 60.0f, loc.y);
    const float kHpmpbar_width = 10.0f;
    const float kHpmpbar_length = 100.0f;
    Zeni::Vertex2f_Color hp00(Hpbar_loc, cr["red"]);
    Zeni::Vertex2f_Color hp01(Hpbar_loc + Zeni::Point2f(0.0f, kHpmpbar_width), cr["red"]);
    Zeni::Vertex2f_Color hp02(Hpbar_loc + Zeni::Point2f(hp_percent * kHpmpbar_length, kHpmpbar_width), cr["red"]);
    Zeni::Vertex2f_Color hp03(Hpbar_loc + Zeni::Point2f(hp_percent * kHpmpbar_length, 0.0f), cr["red"]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> hpbar(hp00, hp01, hp02, hp03);

    Hpbar_loc += Point2f(0.0f, 20.0f);
    Zeni::Vertex2f_Color mp00(Hpbar_loc, cr["blue"]);
    Zeni::Vertex2f_Color mp01(Hpbar_loc + Zeni::Point2f(0.0f, kHpmpbar_width), cr["blue"]);
    Zeni::Vertex2f_Color mp02(Hpbar_loc + Zeni::Point2f(mp_percent * kHpmpbar_length, kHpmpbar_width), cr["blue"]);
    Zeni::Vertex2f_Color mp03(Hpbar_loc + Zeni::Point2f(mp_percent * kHpmpbar_length, 0.0f), cr["blue"]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> mpbar(mp00, mp01, mp02, mp03);
    */
    //CDbar
    
 

//    vr.render(hpbar);
  //  vr.render(mpbar);
  }

  void render_status_helper(int controller, Player* p_ptr) {
    Zeni::String player_texture, player_skill_texture;
    Point2f loc;
    switch (controller) {
      case 0:
        loc = Point2f(20.0f, 10.0f);
        break;
      case 1:
        loc = Point2f(20.0f + 180.0f, 10.0f);
        break;
      case 2:
        loc = Point2f(20.0f + 460.0f, 10.0f);
        break;
      default: //case 3:
        loc = Point2f(20.0f + 620.0f, 10.0f);
        break;
    }

    Point2f head_size(40.0f, 40.0f);
    switch (p_ptr->get_player_type()) {
      case SANZANG:
        player_texture = "tripitaka_head";
        player_skill_texture = "sanzang_skill_";
        break;
      case WUKONG:
        player_texture = "monkey_king_head";
        player_skill_texture = "wukong_skill_";
        break;
      case SHASENG:
        player_texture = "friar_sand_head";
        player_skill_texture = "shaseng_skill_";
        break;
      case BAJIE:
        player_texture = "pigsy_head";
        player_skill_texture = "bajie_skill_";
        break;
      default:
        break;
    }
    render_image(player_texture, loc, loc + head_size);

    float CD1percent = p_ptr->get_CD1_percent();
    float CD2percent = p_ptr->get_CD2_percent();
    float CD3percent = p_ptr->get_CD3_percent();
    //float hp_percent = p_ptr->get_hp()/kHp_max;
    //float mp_percent = p_ptr->get_mp()/kMp_max;
    float kCDbar_length = 30.0f;
    //float kCDbar_width = 10.0f;
    Zeni::String kCDbar_color = "blue";

    
    //Zeni::Video &vr = Zeni::get_Video();
    //Zeni::Colors &cr = Zeni::get_Colors();
/*
    Point2f Hpbar_loc(loc.x + 60.0f, loc.y);
    const float kHpmpbar_width = 10.0f;
    const float kHpmpbar_length = 100.0f;
    Zeni::Vertex2f_Color hp00(Hpbar_loc, cr["red"]);
    Zeni::Vertex2f_Color hp01(Hpbar_loc + Zeni::Point2f(0.0f, kHpmpbar_width), cr["red"]);
    Zeni::Vertex2f_Color hp02(Hpbar_loc + Zeni::Point2f(hp_percent * kHpmpbar_length, kHpmpbar_width), cr["red"]);
    Zeni::Vertex2f_Color hp03(Hpbar_loc + Zeni::Point2f(hp_percent * kHpmpbar_length, 0.0f), cr["red"]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> hpbar(hp00, hp01, hp02, hp03);

    Hpbar_loc += Point2f(0.0f, 20.0f);
    Zeni::Vertex2f_Color mp00(Hpbar_loc, cr["blue"]);
    Zeni::Vertex2f_Color mp01(Hpbar_loc + Zeni::Point2f(0.0f, kHpmpbar_width), cr["blue"]);
    Zeni::Vertex2f_Color mp02(Hpbar_loc + Zeni::Point2f(mp_percent * kHpmpbar_length, kHpmpbar_width), cr["blue"]);
    Zeni::Vertex2f_Color mp03(Hpbar_loc + Zeni::Point2f(mp_percent * kHpmpbar_length, 0.0f), cr["blue"]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> mpbar(mp00, mp01, mp02, mp03);
    */
    //CDbar
    
    Point2f CDbar_loc(loc.x, loc.y + head_size.y + 5.0f);
    Zeni::render_image(player_skill_texture + "1",CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    if (!p_ptr->spell1_mp_enough())
      Zeni::render_image("no_mana", CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    Zeni::render_image("cd_mask", CDbar_loc + Zeni::Vector2f(0, kCDbar_length *(CD1percent)) ,CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));

    CDbar_loc += Point2f(kCDbar_length + 5.0f, 0.0f);
    Zeni::render_image(player_skill_texture + "2",CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    if (!p_ptr->spell2_mp_enough())
      Zeni::render_image("no_mana", CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    Zeni::render_image("cd_mask", CDbar_loc + Zeni::Vector2f(0, kCDbar_length *(CD2percent)) ,CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));

    CDbar_loc += Point2f(kCDbar_length + 5.0f, 0.0f);
    Zeni::render_image(player_skill_texture + "3",CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    if (!p_ptr->spell3_mp_enough())
      Zeni::render_image("no_mana", CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    Zeni::render_image("cd_mask", CDbar_loc + Zeni::Vector2f(0, kCDbar_length *(CD3percent)) ,CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));

    CDbar_loc += Point2f(kCDbar_length + 5.0f, 0.0f);
    int upgrade_point = p_ptr->get_skill_point();
    //Color filter = upgrade_point > 0 ? Color() : Color(1.0f, 0.1f, 0.1f, 0.1f);
    if(upgrade_point > 0)
      Zeni::render_image("upgrade_sign",
                          loc+Point2f(head_size.x+30.f, 10.f),
                          loc+Point2f(head_size.x+30.f, 10.f)+Zeni::Vector2f(kCDbar_length, kCDbar_length)
                        );

//    vr.render(hpbar);
  //  vr.render(mpbar);
  }
  void render_lvl_up(){
    render_image("lvl_up_bg", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    lvl_up_effect_pos+=Point2f(lvl_effect_time * (800.f + 390.f) / 2.f ,0.f);
    render_image("lvl_up", lvl_up_effect_pos, lvl_up_effect_pos + Vector2f(390.f, 45.f)); 
  }


  Point2f lvl_up_effect_pos;
  void render()
  {

    Video &vr = get_Video();
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);

    /* render the map */
    Point2f center_location = Model_state::get_instance()->get_center_location();
    float scale = Model_state::get_instance()->get_scale();
    Quadrilateral<Vertex2f_Texture> map;
    Point2f Map_center(400.0f,300.0f);
    Point2f ul = levels[curr_lvl]->get_upper_left(stage);
    Point2f lr = levels[curr_lvl]->get_lower_right(stage);
    Point2f Map_p0 = (ul - center_location) * scale + Map_center;
    Point2f Map_p1 = (Point2f(ul.x, lr.y) - center_location) * scale + Map_center;
    Point2f Map_p2 = (lr - center_location) * scale + Map_center;
    Point2f Map_p3 = (Point2f(lr.x, ul.y) - center_location) * scale+ Map_center;
    /*
    Vertex2f_Texture text_p0(Map_p0, Point2f(0.0f,0.0f));
    Vertex2f_Texture text_p1(Map_p1, Point2f(0.0f, 154.0f));
    Vertex2f_Texture text_p2(Map_p2, Point2f(50.0f, 154.0f));
    Vertex2f_Texture text_p3(Map_p3, Point2f(50.0f, 0.0f));
    */
    
    Point2f orig_pt(0.0f, 0.0f);
    Vertex2f_Texture text_p0(Map_p0, orig_pt);
    Vertex2f_Texture text_p1(Map_p1, orig_pt + (lr - ul).get_j() / 100.0f);
    Vertex2f_Texture text_p2(Map_p2, orig_pt + (lr - ul) / 100.0f);
    Vertex2f_Texture text_p3(Map_p3, orig_pt + (lr - ul).get_i() / 100.0f);
    map[0] = text_p0;
    map[1] = text_p1;
    map[2] = text_p2;
    map[3] = text_p3;
    Material a("floor");
    map.fax_Material(&a);
    vr.render(map);

    Model_state::get_instance()->render();

    /* render top panel */
    
    Zeni::Colors &cr = Zeni::get_Colors();/*
    const Zeni::String kToppanel_color = "white_light";
    Zeni::Vertex2f_Color p00(Zeni::Point2f(0.0f, 0.0f), cr[kToppanel_color]);
    Zeni::Vertex2f_Color p01(Zeni::Point2f(0.0f, 90.0f), cr[kToppanel_color]);
    Zeni::Vertex2f_Color p02(Zeni::Point2f(800.0f, 90.0f), cr[kToppanel_color]);
    Zeni::Vertex2f_Color p03(Zeni::Point2f(800.0f, 0.0f), cr[kToppanel_color]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> toppanel(p00, p01, p02, p03);
    vr.render(toppanel);
    */
    /* render the PLAYER STATUS */
    std::vector<Player *> * plist = Model_state::get_instance()->get_player_list_ptr();
    int list_pos = 0;
    int controller;
    for (vector<Player *>::iterator it = plist->begin(); it != plist->end(); it++) {
      controller = Model_state::get_instance()->get_player_pos_in_list(list_pos++);
      if (controller != -1){
        render_status_helper(controller, *it);
        m_cd_start[controller] = false;
      }
      
    }

    plist = Model_state::get_instance()->get_dead_player_list_ptr();
    for (vector<Player *>::iterator it = plist->begin(); it != plist->end(); it++) {
      controller = (*it)->get_controller();
      if (controller != -1){
        if(!m_cd_start[controller]){
          m_cd_start[controller] = true;
          m_resurge[controller].reset();
          m_dead[controller].reset();
          m_resurge[controller].start();
        }
        render_dead_status_helper(controller, *it);
      }
    }
    char* str = new char[10];
    Zeni::String text_buf;
    Zeni::Font &l_ft = get_Fonts()["lvl_ft"];
    if (Model_state::get_instance()->get_initial_player_num() > 1) {
    /* render level status */
    text_buf = "Stage ";
    sprintf(str, "%d", curr_lvl + 1);
    text_buf += str;
    text_buf += "-";
    sprintf(str, "%d", stage);
    text_buf += str;
    l_ft.render_text(text_buf,
                   Point2f(20.0f, 560.0f - 2.5f * l_ft.get_text_height()),
                   get_Colors()["yellow"],
                   ZENI_LEFT);

    /* render revival status */
    sprintf(str, "%d", revival_num);
    text_buf = "Resurrection chances: ";
    text_buf += str;
    l_ft.render_text(text_buf,
                   Point2f(20.0f, 560.0f - 1.5f * l_ft.get_text_height()),
                   get_Colors()["yellow"],
                   ZENI_LEFT);
    }
    else
    {
    /* render level status */
    text_buf = "Stage ";
    sprintf(str, "%d", curr_lvl + 1);
    text_buf += str;
    text_buf += "-";
    sprintf(str, "%d", stage);
    text_buf += str;
    l_ft.render_text(text_buf,
                   Point2f(20.0f, 560.0f - 1.5f * l_ft.get_text_height()),
                   get_Colors()["yellow"],
                   ZENI_LEFT);
    }

    /* render exp status */
    int exp_level;
    float exp_percent;
    Model_state::get_instance()->get_exp_level_and_remainder(&exp_level, &exp_percent);

    sprintf(str, "%d", exp_level);
    text_buf = "Team Exp: Lv";
    text_buf += str;
    text_buf += " ";
    sprintf(str, "%.2f", exp_percent*100);
    text_buf += str;
    text_buf += "%";
    l_ft.render_text(text_buf,
                   Point2f(20.0f, 560.0f - 0.5f * l_ft.get_text_height()),
                   get_Colors()["yellow"],
                   ZENI_LEFT);

    /* render exp bar */
    Point2f exp_bar_loc(0.0f, 580.0f);
    const float kExp_bar_width = 6.0f;
    const float kExp_bar_length = 800.0f;
    Zeni::Vertex2f_Color exp00(exp_bar_loc, cr["yellow"]);
    Zeni::Vertex2f_Color exp01(exp_bar_loc + Zeni::Point2f(0.0f, kExp_bar_width), cr["yellow"]);
    Zeni::Vertex2f_Color exp02(exp_bar_loc + Zeni::Point2f(exp_percent * kExp_bar_length, kExp_bar_width), cr["yellow"]);
    Zeni::Vertex2f_Color exp03(exp_bar_loc + Zeni::Point2f(exp_percent * kExp_bar_length, 0.0f), cr["yellow"]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> exp_bar(exp00, exp01, exp02, exp03);
    vr.render(exp_bar);


    if (show_die) {
        l_ft.render_text("Player(s) Died!",
                         Point2f(400.0f, 160.0f - 0.5f*l_ft.get_text_height()),
                         get_Colors()["red"],
                         ZENI_CENTER);
        l_ft.render_text("Hit A to restart the whole level!",
                         Point2f(400.0f, 260.0f - 0.5f*l_ft.get_text_height()),
                         get_Colors()["orange"],
                         ZENI_CENTER);
    }

    render_minimap();
    dialog.render();
    Model_state::get_instance()->get_exp_level_and_remainder(&now_lvl, &re);

    if (now_lvl !=prev_lvl)
      render_lvl_up_effect = true;
    if(render_lvl_up_effect){
        if(!m_lvl_up.is_running()){
          lvl_up_effect_pos = Point2f(-390.f, 116.f);
          m_lvl_up.start();
        }
      render_lvl_up();
      if(m_lvl_up.seconds()>=2.f){
        render_lvl_up_effect = false;
        m_lvl_up.reset();
        m_lvl_up.stop();
      }
    }
    prev_lvl = now_lvl;
    /*
    if (!show_die && !Model_state::get_instance()->get_dead_player_list_ptr()->empty() && revival_num > 0) {
        l_ft.render_text("Player(s) Died!",
                         Point2f(400.0f, 160.0f - 0.5f*l_ft.get_text_height()),
                         get_Colors()["red"],
                         ZENI_CENTER);
        l_ft.render_text("Hit A for resurrection!",
                         Point2f(400.0f, 260.0f - 0.5f*l_ft.get_text_height()),
                         get_Colors()["orange"],
                         ZENI_CENTER);
    }
    */
  }
  int prev_lvl,  now_lvl;
  bool render_lvl_up_effect;
  float re,m_time_passed;
  bool m_cd_start[4];
  Chronometer<Time> m_set, m_lvl_up, m_dead[4], m_resurge[4];
  bool show_die;
};

class Story_State : public Gamestate_II {
  Story_State(const Story_State &);
  Story_State operator=(const Story_State &);
  Time_HQ dialog_entering_time;
public:
  Story_State()
    : dialog_counter(0),
      dialog(&game_time, &dialog_entering_time)
  {
    set_pausable(true);

    set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 0), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 1), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 2), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 3), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);

    dialog.start(0);
  }

private:
  int dialog_counter;
  Dialog_box dialog;
  Chronometer<Time> game_time;
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().push_state(new Popup_Menu_State);
  }

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
    if (confidence >= 0.99f)
    switch(action) {
      case A1:
      case A2:
      case A3:
      case A4:
        dialog.proceed();
        dialog_counter++;
        if (dialog_counter > dialog_max[0]-1) {
          get_Game().pop_state();
          get_Game().push_state(new Play_State());
        }
        break;
    }
  }

  void render() {
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
    if (dialog_counter > 2)
      Zeni::render_image("story_state_background", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    dialog.render();
  }
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
  void on_pop() {
    get_Window().mouse_hide(false);
    get_Game().joy_mouse.enabled = true;
  }
  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    get_Game().joy_mouse.enabled = true;
  }
  
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED){
      get_Game().pop_state();
      get_Game().push_state(new Story_State());
    }
  }

  void render() {
    Widget_Gamestate::render();
    /*
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
                   */
   Zeni::render_image("instr", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
  }
};





class Preparation_State :public Gamestate_II { //public Widget_Gamestate,
  Preparation_State(const Preparation_State &);
  Preparation_State operator=(const Preparation_State &);
public:
  Preparation_State()
  : chosen_num(0)
    //: Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
    set_pausable(true);

    set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 0), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 0), HORI1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 0), VERT1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 0), B1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), JOIN1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 1), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 1), HORI2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 1), VERT2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 1), B2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), JOIN2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 2), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 2), HORI3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 2), VERT3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 2), B3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), JOIN3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 3), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 3), HORI4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 3), VERT4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 3), B4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), JOIN4);
    
  

    for (int i=0; i < 4; i++) {
      p_decided[i] = false;
      p_color[i] = Color();
      char_available[i] = true;
      chosen_char[i] = NONE;
      p_available[i] = false;
      last_cursor_move[i] = Zeni::get_Timer_HQ().get_time();
    }
      player_count = 0;
    m_set.start();
  }

private:
  void on_push() {
    //get_Window().mouse_grab(true);
    Model_state::get_instance()->get_character_list_ptr()->clear();
    get_Window().mouse_hide(true);
    get_Game().joy_mouse.enabled = false;
  }

  void save_chosen_characters() {
    std::vector<kPlayer_type> * char_list = Model_state::get_instance()->get_character_list_ptr();
    for (int i=0; i < 4; i++) {
      char_list->push_back(chosen_char[i]);
    }
  }

  void on_pop() {
    save_chosen_characters();
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

  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
      /*
      get_Game().pop_state();
      //game.push_state(new Play_State());
      get_Game().push_state(new Popup_Menu_State);
      */
      
    }
  }

  void move_cursor(const int player_n, const int dir){
    const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
    Time_HQ* last_cursor_move_ptr = &last_cursor_move[player_n];
    if (float(current_time.get_seconds_since(*last_cursor_move_ptr) > kCursor_move_CD)) {
      *last_cursor_move_ptr = current_time;
      if (p_available[player_n] && !p_decided[player_n]){
        switch(dir){
          case kMove_right:
            if (chosen_char[player_n] == SANZANG)
              chosen_char[player_n] = WUKONG;
            else if (chosen_char[player_n] == WUKONG)
              chosen_char[player_n] = SHASENG;
            else if (chosen_char[player_n] == SHASENG)
              chosen_char[player_n] = BAJIE;
            else if (chosen_char[player_n] == BAJIE)
              chosen_char[player_n] = SANZANG;
            break;
          case kMove_left:
            if (chosen_char[player_n] == WUKONG)
              chosen_char[player_n] = SANZANG;
            else if (chosen_char[player_n] == BAJIE)
              chosen_char[player_n] = SHASENG;
            else if (chosen_char[player_n] == SHASENG)
              chosen_char[player_n] = WUKONG;
            else if (chosen_char[player_n] == SANZANG)
              chosen_char[player_n] = BAJIE;
            break;
          case kMove_up:
            if (chosen_char[player_n] == SHASENG)
              chosen_char[player_n] = SANZANG;
            else if (chosen_char[player_n] == BAJIE)
              chosen_char[player_n] = WUKONG;
            else if (chosen_char[player_n] == WUKONG)
              chosen_char[player_n] = BAJIE;
            else if (chosen_char[player_n] == SANZANG)
              chosen_char[player_n] = SHASENG;
            break;
          case kMove_down:
            if (chosen_char[player_n] == SANZANG)
              chosen_char[player_n] = SHASENG;
            else if (chosen_char[player_n] == WUKONG)
              chosen_char[player_n] = BAJIE;
            else if (chosen_char[player_n] == SHASENG)
              chosen_char[player_n] = SANZANG;
            else if (chosen_char[player_n] == BAJIE)
              chosen_char[player_n] = WUKONG;
            break;
          default:
            break;
        }
      }
    }
  }
  Chronometer<Time> m_set;
  bool char_available[4];
  bool p_decided[4];
  bool p_available[4];
  kPlayer_type chosen_char[4];
  Time_HQ last_cursor_move[4];
  //int cursor_pos[4];
  Color p_color[4];
  int chosen_num, player_count;

  void choose_char(const int controller) {
    if (p_available[controller] && (char_available[chosen_char[controller]] || p_decided[controller])) {
      chosen_num = p_decided[controller]? chosen_num - 1 : chosen_num + 1;
      if (p_decided[controller])
        p_decided[controller] = false;
      else
        p_decided[controller] = true;

      if (char_available[chosen_char[controller]])
        char_available[chosen_char[controller]] = false;
      else
        char_available[chosen_char[controller]] = true;
    }
  }

  void on_event(const Zeni_Input_ID &, const float &confidence, const int &action) {
    switch(action) {
      case HORI1:
        if (confidence >= 1.f)
          move_cursor(0,kMove_right);
        else if (confidence <= -1.f)
          move_cursor(0,kMove_left);
        break;
      case VERT1:
        if (confidence >= kVert_move_threshold)
          move_cursor(0,kMove_down);
        else if (confidence <= -kVert_move_threshold)
          move_cursor(0,kMove_up);
        break;
      case HORI2:
        if (confidence >= 1.f)
          move_cursor(1,kMove_right);
        else if (confidence <= -1.f)
          move_cursor(1,kMove_left);
        break;
      case VERT2:
        if (confidence >= kVert_move_threshold)
          move_cursor(1,kMove_down);
        else if (confidence <= -kVert_move_threshold)
          move_cursor(1,kMove_up);
        break;
      case HORI3:
        if (confidence >= 1.f)
          move_cursor(2,kMove_right);
        else if (confidence <= -1.f)
          move_cursor(2,kMove_left);
        break;
      case VERT3:
        if (confidence >= kVert_move_threshold)
          move_cursor(2,kMove_down);
        else if (confidence <= -kVert_move_threshold)
          move_cursor(2,kMove_up);
        break;
      case HORI4:
        if (confidence >= 1.f)
          move_cursor(3,kMove_right);
        else if (confidence <= -1.f)
          move_cursor(3,kMove_left);
        break;
      case VERT4:
        if (confidence >= kVert_move_threshold)
          move_cursor(3,kMove_down);
        else if (confidence <= -kVert_move_threshold)
          move_cursor(3,kMove_up);
        break;
      case JOIN1:
        if(!p_available[0]){
          p_available[0] = true;
          chosen_char[0] = SANZANG;
          ++player_count;
        }
        else if(chosen_num == player_count){
          get_Game().pop_state();
          Model_state::get_instance()->set_initial_player_num(player_count);
          //get_Game().push_state(new Play_State());
          get_Game().push_state(new Instructions_State());
        }
        break;
      case JOIN2:
        if(!p_available[1]){
          p_available[1] = true;
          chosen_char[1] = SANZANG;
          ++player_count;
        }
        else if(chosen_num == player_count){
          get_Game().pop_state();
          Model_state::get_instance()->set_initial_player_num(player_count);
          //get_Game().push_state(new Play_State());
          get_Game().push_state(new Instructions_State());
        }
        break;
      case JOIN3:
        if(!p_available[2]){
          p_available[2] = true;
          chosen_char[2] = SANZANG;
          ++player_count;
        }
        else if(chosen_num == player_count){
          get_Game().pop_state();
          Model_state::get_instance()->set_initial_player_num(player_count);
          //get_Game().push_state(new Play_State());
          get_Game().push_state(new Instructions_State());
        }
        break;
      case JOIN4:
        if(!p_available[3]){
          p_available[3] = true;
          chosen_char[3] = SANZANG;
          ++player_count;
        }
        else if(chosen_num == player_count){
          get_Game().pop_state();
          Model_state::get_instance()->set_initial_player_num(player_count);
          //get_Game().push_state(new Play_State());
          get_Game().push_state(new Instructions_State());
        }
        break;
      default:
        break;
    }
    //get_Game().push_state(new Play_State());
    if(confidence >= 1.0f) {
      switch(action) {
        case MENU: {
          //get_Game().pop_state();
          Game &game = get_Game();
          game.push_state(new Popup_Menu_State);
          break;
        }
        case A1:
          if(p_available[0])
          choose_char(0);
          break;
        case A2:
          if(p_available[1])
          choose_char(1);
          break;
        case A3:
          if(p_available[2])
            choose_char(2);
          break;
        case A4:
          if(p_available[3])
            choose_char(3);
          break;
        default:
          break;
      }
    }
  }

  void render() {
    //Widget_Gamestate::render();
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
    if (m_set.seconds() < 0.25f)
      render_image("selection0", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    else if (m_set.seconds() < 0.5f)
      render_image("selection1", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    else{
      render_image("selection0", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
      m_set.reset();
      m_set.start();
    }

    
    for  (int i = 0; i < 4; ++i){
      if (p_available[i]){
        String a;
        if(p_decided[i]){
          if(chosen_char[i] == SANZANG)
            a = String("sanzang_mask_p" + itoa(i+1));
          else if (chosen_char[i] == SHASENG)
            a = String("shaseng_mask_p" + itoa(i+1));
          else if (chosen_char[i] == WUKONG)
            a = String("wukong_mask_p" + itoa(i+1));
          else if (chosen_char[i] == BAJIE)
            a = String("bajie_mask_p" + itoa(i+1));
          render_image(a, Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
        }else{
          if(chosen_char[i] == SANZANG)
            a = String("sanzang_p" + itoa(i+1));
          else if (chosen_char[i] == SHASENG)
            a = String("shaseng_p" + itoa(i+1));
          else if (chosen_char[i] == WUKONG)
            a = String("wukong_p" + itoa(i+1));
          else if (chosen_char[i] == BAJIE)
            a = String("bajie_p" + itoa(i+1));
          render_image(a, Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
        }
      }
    }
    if (chosen_num == player_count && player_count != 0)
      render_image("start_game", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
  }
};

class Choose_Pnumber_State : public Widget_Gamestate {
  Choose_Pnumber_State(const Choose_Pnumber_State &);
  Choose_Pnumber_State operator=(const Choose_Pnumber_State &);

  class Start_Button : public Text_Button {
  public:
    Start_Button(int* player_num_ptr_)
      : Text_Button(Point2f(20.0f, 20.0f), Point2f(220.0f, 80.0f), "system_36_800x600", "Start Game!"),
        player_num_ptr(player_num_ptr_)
    {
    }

  private:
    int* player_num_ptr;
    void on_accept() {
      Model_state::get_instance()->set_initial_player_num(*player_num_ptr);
      get_Game().pop_state();
      get_Game().push_state(new Preparation_State());
    }
  };

  class Plus_Player_Button : public Text_Button {
  public:
    Plus_Player_Button(int* player_num_ptr_)
      : Text_Button(Point2f(520.0f, 320.0f), Point2f(720.0f, 380.0f), "system_36_800x600", "+1"),
        player_num_ptr(player_num_ptr_)
    {
    }

  private:
    int* player_num_ptr;
    void on_accept() {
      if (*player_num_ptr < 4)
        (*player_num_ptr)++;
    }
  };
  class Minus_Player_Button: public Text_Button {
  public:
    Minus_Player_Button(int* player_num_ptr_)
      : Text_Button(Point2f(120.0f, 320.0f), Point2f(320.0f, 380.0f), "system_36_800x600", "-1"),
        player_num_ptr(player_num_ptr_)
    {
    }

  private:
    int* player_num_ptr;
    void on_accept() {
      if (*player_num_ptr > 1)
        (*player_num_ptr)--;
    }
  };

public:
  Choose_Pnumber_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
      player_num(1)
  {
    Start_Button* start_button = new Start_Button(&player_num);
    Plus_Player_Button* plus_player_button = new Plus_Player_Button(&player_num);
    Minus_Player_Button* minus_player_button = new Minus_Player_Button(&player_num);
    m_widgets.lend_Widget(*start_button);
    m_widgets.lend_Widget(*plus_player_button);
    m_widgets.lend_Widget(*minus_player_button);
  }

private:
  int player_num;
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
      get_Game().push_state(new Popup_Menu_State);
    }
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["shop_ft"];
    char* str = new char[10];
    sprintf(str, "%d", player_num);//speed_lvl);
    String text_buf = "Player num: ";
    text_buf += str;
    fr.render_text(text_buf,
                   Point2f(400.0f, 300.0f),
                   get_Colors()["white"],
                   ZENI_CENTER);
    delete[] str;
  }
};

class Title_state : public Gamestate_II{
  Title_state(const Title_state &);
  Title_state operator=(const Title_state &);
private:
  Chronometer<Time> m_set;
public:
  Title_state(){
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), CONFIRM1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), CONFIRM2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), CONFIRM3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), CONFIRM4);
    get_Sound().set_BGM("sfx/bgm");
    get_Sound().play_BGM();
    get_Sound().set_BGM_looping(true);
    m_set.start();
  };

  void on_push() {
    get_Window().mouse_hide(true);
    get_Game().joy_mouse.enabled = false;
  };

  void on_pop() {
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

  void on_event(const Zeni_Input_ID &, const float &confidence, const int &action);
  
  void render(){
    //Gamestate_II::render();
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
    if(m_set.seconds() < 0.5f)
      render_image("title0", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    else if (m_set.seconds()<1.f)
      render_image("title1", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    else{
      m_set.reset();
      m_set.start();
      render_image("title0", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
    }
  }
};


class Instructions_State_I : public Widget_Gamestate {
  Instructions_State_I(const Instructions_State &);
  Instructions_State_I operator=(const Instructions_State &);

public:
  Instructions_State_I()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event);

  void render() {
    Widget_Gamestate::render();
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
    /*
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
                   */
   Zeni::render_image("instr", Point2f(0.f, 0.f), Point2f(1024.f, 1024.f));
  }
};


class Pre_Play_State : public Widget_Gamestate {
    Pre_Play_State(const Pre_Play_State &);
    Pre_Play_State & operator=(const Pre_Play_State &);

  public:
      class Difficulty_Down_Button : public Text_Button {
          Difficulty_Down_Button(const Difficulty_Down_Button &);
          Difficulty_Down_Button & operator=(const Difficulty_Down_Button &);
          
      public:
          Difficulty_Down_Button()
          : Text_Button(Point2f(50.0f, 370.0f), Point2f(150.0f, 420.0f),
                        "system_36_800x600", "<<")
          {
            give_Renderer(new Widget_Renderer_Tricolor(Color(1.f, 1.f, 1.f, 0.0f), 
                  Color(1.f, 0.66f, 0.f, 0.0f),
                  Color(0.5f, 0.66f, 0.f, 0.0f),
                                                        Color(1.f, 0.f, 0.f, 0.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f)));
          }
          
          void on_accept() {
            Model_state::get_instance()->diff_minus();
          }
      } difficulty_down_button;

      class Difficulty_Up_Button : public Text_Button {
          Difficulty_Up_Button(const Difficulty_Up_Button &);
          Difficulty_Up_Button & operator=(const Difficulty_Up_Button &);
          
      public:
          Difficulty_Up_Button()
          : Text_Button(Point2f(390.0f, 370.0f), Point2f(490.0f, 420.0f),
                        "system_36_800x600", ">>")
          {
            give_Renderer(new Widget_Renderer_Tricolor(Color(1.f, 1.f, 1.f, 0.0f), 
                  Color(1.f, 0.66f, 0.f, 0.0f),
                  Color(0.5f, 0.66f, 0.f, 0.0f),
                                                        Color(1.f, 0.f, 0.f, 0.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f)));
          }
          
          void on_accept() {
            Model_state::get_instance()->diff_add();
          }
      } difficulty_up_button;

      class Play_Single_Button : public Text_Button {
          Play_Single_Button(const Play_Single_Button &);
          Play_Single_Button & operator=(const Play_Single_Button &);
          
      public:
          Play_Single_Button()
          : Text_Button(Point2f(50.0f, 440.0f), Point2f(260.0f, 490.0f),
                        "system_36_800x600", "Play")
          {
            give_Renderer(new Widget_Renderer_Tricolor(Color(1.f, 1.f, 1.f, 0.0f), 
                  Color(1.f, 0.66f, 0.f, 0.0f),
                  Color(0.5f, 0.66f, 0.f, 0.0f),
                                                        Color(1.f, 0.f, 0.f, 0.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f)));
          }
          
          void on_accept() {
              get_Game().push_state(new Preparation_State());

          }
      } play_single_button;
      
  
      class Instructions_Button : public Text_Button {
          Instructions_Button(const Instructions_Button &);
          Instructions_Button & operator=(const Instructions_Button &);
          
      public:
          Instructions_Button()
          : Text_Button(Point2f(280.0f, 440.0f), Point2f(490.0f, 490.0f),
                        "system_36_800x600", "Instructions")
          {
            give_Renderer(new Widget_Renderer_Tricolor(Color(1.f, 1.f, 1.f, 0.0f), 
                  Color(1.f, 0.66f, 0.f, 0.0f),
                  Color(0.5f, 0.66f, 0.f, 0.0f),
                                                        Color(1.f, 0.f, 0.f, 0.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f)));
          }
          
          void on_accept() {
             get_Game().push_state(new Instructions_State_I());
          }
      } instructions_button;

      //Popup_Menu_State::Sound_Check_Box sound_check_box;
#ifndef ANDROID
      Popup_Menu_State::Configure_Video_Button configure_video_button;
#endif
      Popup_Menu_State::Sound_Check_Box sound_check_box;
      Popup_Menu_State::Quit_Button quit_button;
    Pre_Play_State()
      : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
      configure_video_button(Point2f(50.0f, 510.0f), Point2f(260.0f, 560.0f)),
      sound_check_box(Point2f(510.0f, 510.0f), Point2f(560.0f, 560.0f)),
      quit_button(Point2f(280.0f, 510.0f), Point2f(490.0f, 560.0f))
    {
    /*  m_widgets.lend_Widget(level_1_button);
        m_widgets.lend_Widget(level_2_button);
        m_widgets.lend_Widget(level_3_button);
        m_widgets.lend_Widget(level_4_button);*/
        m_widgets.lend_Widget(difficulty_down_button);
        m_widgets.lend_Widget(difficulty_up_button);
        m_widgets.lend_Widget(play_single_button);
        m_widgets.lend_Widget(sound_check_box);
        m_set.start();
#ifndef ANDROID
        configure_video_button.give_Renderer(new Widget_Renderer_Tricolor(Color(1.f, 1.f, 1.f, 0.0f), 
                  Color(1.f, 0.66f, 0.f, 0.0f),
                  Color(0.5f, 0.66f, 0.f, 0.0f),
                                                        Color(1.f, 0.f, 0.f, 0.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f)));
        m_widgets.lend_Widget(configure_video_button);

#endif
        m_widgets.lend_Widget(instructions_button);
     // m_widgets.lend_Widget(back_button);
        quit_button.give_Renderer(new Widget_Renderer_Tricolor(Color(1.f, 1.f, 1.f, 0.0f), 
                  Color(1.f, 0.66f, 0.f, 0.0f),
                  Color(0.5f, 0.66f, 0.f, 0.0f),
                                                        Color(1.f, 0.f, 0.f, 0.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f),
                                                        Color(1.f, 1.f, 1.f, 1.0f)));
       m_widgets.lend_Widget(quit_button);
       get_Video().set_clear_Color(get_Colors()["title_bg"]);
     }

    ~Pre_Play_State() {
      get_Video().set_clear_Color(Color(1.0f, 0.0f, 0.0f, 0.0f));
    }

      void on_key(const SDL_KeyboardEvent &event)
      {
          if (event.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN){
              get_Game().pop_state();
              get_Game().push_state( new Title_state());
              
          }
          else
              Widget_Gamestate::on_key(event);
      }

      void perform_logic(){
          Widget_Gamestate::perform_logic();
      const float time_passed = m_set.seconds();
      const float time_step = time_passed - m_time_passed;
      m_time_passed = time_passed;
      m_step += time_step;
      if (m_step > 0.5){
          change = !change;
          m_step = 0;
      }
      }
     


      void render(){
          get_Video().set_2d(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
          if(m_set.seconds()<0.5f)
            render_image("title0", Point2f(0.f, 0.0f), Point2f(1024.0f, 1024.0f));
          else if (m_set.seconds()<1.f)
            render_image("title2", Point2f(0.f, 0.0f), Point2f(1024.0f, 1024.0f));
          else{
            render_image("title0", Point2f(0.f, 0.0f), Point2f(1024.0f, 1024.0f));
            m_set.reset();
            m_set.start();
          }

          get_Video().set_2d(get_virtual_window(), fix_aspect_ratio());

          m_widgets.render();
          Zeni::String diff;
          switch(Model_state::get_instance()->get_diff()) {
              case A:
                diff = "Easy";
                break;
              case B:
                diff = "Comfortable";
                break;
              case C:
                diff = "Normal";
                break;
              case D:
                diff = "Hard";
                break;
              case E:
                diff = "Crazy";
                break;
              case GOD:
                diff = "God Mode";
                break;
              default:
                break;
          }
          Zeni::Font &l_ft = get_Fonts()["victory_ft"];
          l_ft.render_text(diff,
                             Point2f(270.0f, 395.0f - 0.5f*l_ft.get_text_height()),
                             get_Colors()["yellow"],
                             ZENI_CENTER);
      };
  private:
      Chronometer<Time> m_set;
      float m_time_passed, m_step;
      bool change;
      
  };

  
void Instructions_State_I::on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED){
      get_Game().pop_state();
      get_Game().push_state(new Pre_Play_State());
      //get_Game().push_state(new Victory_State());
    }
  }

 void Title_state::on_event(const Zeni_Input_ID &, const float &, const int &action) {
    switch(action) {
      case CONFIRM1:
      case CONFIRM2:
      case CONFIRM3:
      case CONFIRM4:
        get_Game().pop_state();
        //get_Game().push_state(new Title_State<Choose_Pnumber_State, Instructions_State>("Zenipex Library\nApplication"));
        get_Game().push_state(new Pre_Play_State());
      default:
        break;
    }
  }
class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("Flaming Mountains");

      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;
      return new Title_state();
      //return new Title_State<Choose_Pnumber_State, Instructions_State>("Zenipex Library\nApplication");
      //return new Title_State<Play_level_one, Instructions_State>("Zenipex Library\nApplication");
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
