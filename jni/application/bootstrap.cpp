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
#include <iostream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;
using namespace Flame;

class Upgrade_state :public Gamestate_II { //public Widget_Gamestate,
  Upgrade_state(const Upgrade_state &);
  Upgrade_state operator=(const Upgrade_state &);
private:
  int skill_point_tmp[4];
  int attack_tmp[4];
  int defense_tmp[4];
  int strength_tmp[4];
  int speed_tmp[4];
  bool confirmed[4];
  std::vector<Player *> * player_list_ptr;
  int player_number;
  Chronometer<Time>* game_time;
  Time_HQ last_highlight_move;
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
    last_highlight_move = Zeni::get_Timer_HQ().get_time();
    //set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), CONFIRM1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 0), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 0), HORI1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 0), VERT1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 0), B1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 0), X1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 0), Y1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 0), L1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), CONFIRM2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 1), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 1), HORI2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 1), VERT2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 1), B2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 1), X2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 1), Y2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 1), L2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), CONFIRM3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 2), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 2), HORI3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 2), VERT3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 2), B3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 2), X3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 2), Y3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 2), L3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), CONFIRM4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 3), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 3), HORI4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 3), VERT4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 3), B4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 3), X4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 3), Y4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 3), L4);

    player_list_ptr = Model_state::get_instance()->get_player_list_ptr();
    player_number = player_list_ptr->size();
    load_abilities();
    for (int i=0; i < Model_state::get_instance()->get_player_num(); i++) {
      confirmed[i] = false;
      
      if ((*player_list_ptr)[i]->ptype == SHASENG)
        cursor_pos[i] = kCursor_min;
      else
        cursor_pos[i] = kCursor_min+1;
      p_confirmed[i] = false;
      p_color[i] = Color();
    }

  }

private:
  void load_abilities() {
    for (int i=0; i < Model_state::get_instance()->get_player_num(); i++) {
      skill_point_tmp[i] = (*player_list_ptr)[i]->get_skill_point();
      attack_tmp[i] = (*player_list_ptr)[i]->attack;
      defense_tmp[i] = (*player_list_ptr)[i]->defense;
      strength_tmp[i] = (*player_list_ptr)[i]->strength;
      speed_tmp[i] = (*player_list_ptr)[i]->speed;
    }
  }
  void store_abilities() {
    for (int i=0; i < Model_state::get_instance()->get_player_num(); i++) {
      (*player_list_ptr)[i]->set_skill_point(skill_point_tmp[i]);
      (*player_list_ptr)[i]->attack = attack_tmp[i];
      (*player_list_ptr)[i]->defense = defense_tmp[i];
      (*player_list_ptr)[i]->strength = strength_tmp[i];
      (*player_list_ptr)[i]->speed = speed_tmp[i];
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
  
  void highlight_move(const int player_n, const bool is_down) {
    const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
    if (float(current_time.get_seconds_since(last_highlight_move) > kHighlight_move_CD)) {
       last_highlight_move = current_time;
        if (!p_confirmed[player_n]) {
          if (is_down && cursor_pos[player_n] < kShop_cursor_max-1) {
            cursor_pos[player_n]++;
          } else if (!is_down && cursor_pos[player_n] > 1) {
            cursor_pos[player_n]--;
          } else if (!is_down && (*player_list_ptr)[player_n]->ptype == SHASENG && cursor_pos[player_n] > 0) {
            cursor_pos[player_n]--;
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

  void confirm(const int player_n) {
    if (player_n < Model_state::get_instance()->get_player_num()) {
      cursor_pos[player_n] = 5;
      click_a(player_n);
    }
  }
  void click_a(const int player_n) {
    if (Model_state::get_instance()->get_player_num() && !p_confirmed[player_n]) {
      int pos = cursor_pos[player_n];
      switch (pos) {
        case 0: //switch arrow
          if ((*player_list_ptr)[player_n]->ptype == SHASENG) {
            //switch_magic_arrow();
            (*player_list_ptr)[player_n]->switch_magic_arrow();
          }
          break;
        case 1:
          if (attack_tmp[player_n] < kAttack_max) {
            attack_tmp[player_n]++;
          }
          break;
        case 2:
          if (defense_tmp[player_n] < kDefense_max) {
            defense_tmp[player_n]++;
          }
          break;
        case 3:
          if (strength_tmp[player_n] < kStrength_max) {
            strength_tmp[player_n]++;
          }
          break;
        case 4:
          if (speed_tmp[player_n] < kSpeed_max) {
            speed_tmp[player_n]++;
          }
          break;
        case 5: //confirm
          p_confirmed[player_n] = true;
          chosen_num++;
          break;
        case 6: //cancel
          load_abilities();
          break;
        default:
          break;
      }
      if (chosen_num == 1) {//player_number) {
        get_Game().pop_state();
        //get_Game().push_state(new Play_State());
      }
    }
  }

  void on_event(const Zeni_Input_ID &, const float &confidence, const int &action) {
    float high_conf = 0.9f;
    switch(action) {
      case VERT1:
        if (confidence >= high_conf)
          highlight_move(0, true);
        else if (confidence <= -high_conf)
          highlight_move(0, false);
        break;
      case VERT2:
        if (confidence >= high_conf)
          highlight_move(1, true);
        else if (confidence <= -high_conf)
          highlight_move(1, false);
        break;
      case VERT3:
        if (confidence >= high_conf)
          highlight_move(2, true);
        else if (confidence <= -high_conf)
          highlight_move(2, false);
        break;
      case VERT4:
        if (confidence >= high_conf)
          highlight_move(3, true);
        else if (confidence <= -high_conf)
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
          confirm(0);
          break;
        case CONFIRM2: 
          confirm(1);
          break;
        case CONFIRM3: 
          confirm(2);
          break;
        case CONFIRM4: 
          confirm(3);
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
  void render_player_helper(int p_x, Player* p_ptr) {
    Zeni::String player_texture;
    Point2f loc;
    float margin = 20.0f;
    switch (p_x) {
      case 0:
        loc = Point2f(margin, margin * 2 + 40.0f);
        break;
      case 1:
        loc = Point2f(margin + 400.0f, margin * 2+ 40.0f);
        break;
      case 2:
        loc = Point2f(margin, margin * 2 + 300.0f);
        break;
      default: //case 3:
        loc = Point2f(margin + 400.0f, margin * 2 + 300.0f);
        break;
    }

    Point2f size(140.0f, 200.0f);
    switch (p_ptr->get_player_type()) {
      case SANZANG:
        player_texture = "tripitaka_front0";
        break;
      case WUKONG:
        player_texture = "monkey_king_front0";
        break;
      case SHASENG:
        player_texture = "friar_sand_front0";
        break;
      case BAJIE:
        player_texture = "pigsy_front0";
        break;
      default:
        break;
    }
    render_image(player_texture, loc, loc + size);


    //render status
    Zeni::Font &fr = get_Fonts()["shop_ft"];
    float button_size = fr.get_text_height() * 0.8f;    

    char* str = new char[10];
    sprintf(str, "%d", p_x+1);//speed_lvl);
    String text_buf = "Player ";
    text_buf += str;
    fr.render_text(text_buf,
                   loc - Point2f(0.0f, 30.0f),
                   get_Colors()["white"],
                   ZENI_LEFT);

    Point2f Bar_loc[7];
    Bar_loc[0] = loc + Point2f(140.0f, 40.0f) - Point2f(button_size * 1.1f, 2.5f*fr.get_text_height());
    Bar_loc[1] = loc + Point2f(140.0f, 40.0f);
    Bar_loc[2] = Bar_loc[1] + Point2f(0.0f, fr.get_text_height());
    Bar_loc[3] = Bar_loc[2] + Point2f(0.0f, fr.get_text_height());
    Bar_loc[4] = Bar_loc[3] + Point2f(0.0f, fr.get_text_height());
    Bar_loc[5] = Bar_loc[4] + Point2f(0.0f, 2*fr.get_text_height());
    Bar_loc[6] = Bar_loc[5] + Point2f(0.0f, fr.get_text_height());

    if (p_ptr->get_player_type() == SHASENG) {
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

    sprintf(str, "%d", attack_tmp[p_x]);//speed_lvl);
    text_buf = "Attack: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[1],
                   get_Colors()["white"],
                   ZENI_LEFT);

    
    //loc += Point2f(0.0f, fr.get_text_height());
    sprintf(str, "%d", defense_tmp[p_x]);//speed_lvl);
    text_buf = "Defense: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[2],
                   get_Colors()["white"],
                   ZENI_LEFT);
    

    //loc += Point2f(0.0f, fr.get_text_height());
    sprintf(str, "%d", strength_tmp[p_x]);//speed_lvl);
    text_buf = "MAX HP: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[3],
                   get_Colors()["white"],
                   ZENI_LEFT);

    //loc += Point2f(0.0f, fr.get_text_height());
    sprintf(str, "%d", speed_tmp[p_x]);//speed_lvl);
    text_buf = "Speed: lvl ";
    text_buf += str;
    text_buf += "/5";
    fr.render_text(text_buf,
                   Bar_loc[4],
                   get_Colors()["white"],
                   ZENI_LEFT);
    Color filter;
    Color disable_button = Color(1.0f, 0.5f, 0.5f, 0.5f);
    filter = cursor_pos[p_x] == 1 ? Color() : disable_button;
    render_image("plus_button", Bar_loc[1] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[1] + Point2f(-button_size*0.1f, button_size), false, filter); 
    filter = cursor_pos[p_x] == 2 ? Color() : disable_button;
    render_image("plus_button", Bar_loc[2] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[2] + Point2f(-button_size*0.1f, button_size), false, filter);
    filter = cursor_pos[p_x] == 3 ? Color() : disable_button;
    render_image("plus_button", Bar_loc[3] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[3] + Point2f(-button_size*0.1f, button_size), false, filter); 
    filter = cursor_pos[p_x] == 4 ? Color() : disable_button;
    render_image("plus_button", Bar_loc[4] - Point2f(button_size * 1.1f, 0.0f), Bar_loc[4] + Point2f(-button_size*0.1f, button_size), false, filter); 

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
    float Shaseng_indent = (*player_list_ptr)[p_x]->ptype == SHASENG ? button_size*1.1f : 0.0f;
    Point2f highlight_size(150.0f + 2*Shaseng_indent, fr.get_text_height());
    render_image("highlight", Bar_loc[cursor_pos[p_x]] - Point2f(button_size * 1.1f - Shaseng_indent, 0.0f), Bar_loc[cursor_pos[p_x]] + highlight_size);

    Zeni::Font &fr2 = get_Fonts()["shop_title"];
    fr2.render_text("Upgrade Abilities",
                   Point2f(400.0f, 10.0f),
                   get_Colors()["white"],
                   ZENI_CENTER);

  }

  void render() {
    int p_x = 0;
    for (vector<Player *>::iterator it = player_list_ptr->begin(); it != player_list_ptr->end(); it++) {
      render_player_helper(p_x++, *it);
    }
  }
};

class Play_State : public Gamestate_II {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);
  Dialog_box dialog;
  int lvl;
public:
  Play_State() :
    m_time_passed(0.f),
    lvl(0),
    dialog(&m_set)
  {
    set_pausable(true);

    m_set.start();

    set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 0), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 0), HORI1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 0), VERT1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 0), B1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 0), X1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 0), Y1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 0), L1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 1), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 1), HORI2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 1), VERT2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 1), B2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 1), X2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 1), Y2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 1), L2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 2), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 2), HORI3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 2), VERT3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 2), B3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 2), X3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 2), Y3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 2), L3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), MENU);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_BACK, 3), BACK);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 3), HORI4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 3), VERT4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 3), B4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_X, 3), X4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_Y, 3), Y4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_LEFT_SHOULDER, 3), L4);


    Model_state::get_instance()->init(lvl, &m_set);
    set_level(lvl);
    //!!! test
    //dialog.start(lvl);
    begin_dialog(&dialog, lvl);
  }

private:
    void set_level(const int lvl){
        if (lvl == 0){
            float x = 350;
            for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
                it != Model_state::get_instance()->get_player_list_ptr()->end();
                ++it){
                (*it)->set_position(Point2f(x,3000));
                x += 100.f;
            }
            Redboy* redboy_inst = new Redboy(Zeni::Point2f(100, 2300));
            Model_state::get_instance()->add_monster(redboy_inst);
            Wanderer* wanderer = new Wanderer(Zeni::Point2f(100, 2300));
            Model_state::get_instance()->add_monster(wanderer);
            wanderer = new Wanderer(Zeni::Point2f(100, 2300));
            Model_state::get_instance()->add_monster(wanderer);
            wanderer = new Wanderer(Zeni::Point2f(150, 2300));
            Model_state::get_instance()->add_monster(wanderer);
            wanderer = new Wanderer(Zeni::Point2f(250, 2300));
            Model_state::get_instance()->add_monster(wanderer);
            
            Whisper* whisper_1 = new Whisper(Zeni::Point2f(400, 2800));
            Model_state::get_instance()->add_monster(whisper_1);
            whisper_1 = new Whisper(Zeni::Point2f(450, 2800));
            Model_state::get_instance()->add_monster(whisper_1);
            whisper_1 = new Whisper(Zeni::Point2f(500, 2800));
            Model_state::get_instance()->add_monster(whisper_1);
            whisper_1 = new Whisper(Zeni::Point2f(550, 2800));
            Model_state::get_instance()->add_monster(whisper_1);
        }
        else if (lvl == 1){
            float x = 350;
            for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
                it != Model_state::get_instance()->get_player_list_ptr()->end();
                ++it){
                (*it)->set_position(Point2f(x,1900));
                x += 100.f;
            }
            Wanderer* wanderer = new Wanderer(Zeni::Point2f(100, 1500));
            Model_state::get_instance()->add_monster(wanderer);
            wanderer = new Wanderer(Zeni::Point2f(150, 1500));
            Model_state::get_instance()->add_monster(wanderer);
            wanderer = new Wanderer(Zeni::Point2f(200, 1500));
            Model_state::get_instance()->add_monster(wanderer);
            wanderer = new Wanderer(Zeni::Point2f(250, 1500));
            Model_state::get_instance()->add_monster(wanderer);
            
            Whisper* whisper_1 = new Whisper(Zeni::Point2f(400, 1500));
            Model_state::get_instance()->add_monster(whisper_1);
            whisper_1 = new Whisper(Zeni::Point2f(450, 1500));
            Model_state::get_instance()->add_monster(whisper_1);
            whisper_1 = new Whisper(Zeni::Point2f(500, 1500));
            Model_state::get_instance()->add_monster(whisper_1);
            whisper_1 = new Whisper(Zeni::Point2f(550, 1500));
            Model_state::get_instance()->add_monster(whisper_1);
        }else if (lvl == 2){
            float x = 350;
            for(auto it = Model_state::get_instance()->get_player_list_ptr()->begin();
                it != Model_state::get_instance()->get_player_list_ptr()->end();
                ++it){
                (*it)->set_position(Point2f(x,900));
                x += 100.f;
            }
            Redboy* redboy_inst = new Redboy(Zeni::Point2f(300, 300));
            Model_state::get_instance()->add_monster(redboy_inst);
            Whisper* whisper_1 = new Whisper(Zeni::Point2f(400, 500));
            Model_state::get_instance()->add_monster(whisper_1);
        }
        
    }
    
  void begin_dialog(Dialog_box* dialog_ptr, int lvl) {
    //m_set.pause_all();
    dialog_ptr->start(lvl);
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
    bool control_enable[4]; // = {true, true, true, true};
    Player* p_ptr;
    for (int i=0; i < 4; i++) {
      if (i < Model_state::get_instance()->get_player_num())
          p_ptr = (*Model_state::get_instance()->get_player_list_ptr())[i];
      control_enable[i] = i < Model_state::get_instance()->get_player_num() &&
                          !p_ptr->is_hitback() &&
                          !p_ptr->is_charging();
    }
    switch(action) {
      case BACK: {
        Game &game = get_Game();
        game.push_state(new Popup_Menu_State);
        break;
      }
      case HORI1:
        if (control_enable[0])
          (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.move_hori = confidence / 2;
        break;
      case VERT1:
        if (control_enable[0])
          (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.move_vert = confidence / 2;
        break;
      case L1:
        if (control_enable[0])
          (*Model_state::get_instance()->get_player_list_ptr())[0]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      case HORI2:
        if (control_enable[1])
          (*Model_state::get_instance()->get_player_list_ptr())[1]->ctrl.move_hori = confidence / 2;
        break;
      case VERT2:
        if (control_enable[1])
          (*Model_state::get_instance()->get_player_list_ptr())[1]->ctrl.move_vert = confidence / 2;
        break;
      case L2:
        if (control_enable[1])
          (*Model_state::get_instance()->get_player_list_ptr())[1]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      case HORI3:
        if (control_enable[2])
          (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.move_hori = confidence / 2;
        break;
      case VERT3:
        if (control_enable[2])
          (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.move_vert = confidence / 2;
        break;
      case L3:
        if (control_enable[2])
          (*Model_state::get_instance()->get_player_list_ptr())[2]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      case HORI4:
        if (control_enable[3])
          (*Model_state::get_instance()->get_player_list_ptr())[3]->ctrl.move_hori = confidence / 2;
        break;
      case VERT4:
        if (control_enable[3])
          (*Model_state::get_instance()->get_player_list_ptr())[3]->ctrl.move_vert = confidence / 2;
        break;
      case L4:
        if (control_enable[3])
          (*Model_state::get_instance()->get_player_list_ptr())[3]->ctrl.l = confidence > 0.5 ? true : false;
        break;

      default:
        break;
    }

    if (action == MENU && confidence == 1.0f) {
      get_into_upgradeshop();
    }
    
    switch(action) {
      case B1:
        if (confidence < 0.1f) {
          (*Model_state::get_instance()->get_player_list_ptr())[0]->button_b_release();
        }
        break;
      default:
        break;
    }

    if(confidence >= 0.9f) {
      switch(action) {
        case A1:
          if (control_enable[0])
            if (dialog.is_goingon()) {
              dialog.proceed();
            } else {
              (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(A1);
            }
          break;
        case B1:
          if (control_enable[0])
            (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(B1);
          break;
        case X1:
          if (control_enable[0])
            (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(X1);
          break;
        case Y1: {
          if (control_enable[0])
            (*Model_state::get_instance()->get_player_list_ptr())[0]->fire(Y1);
          break;
        }

        case A2:
          if (control_enable[1])
            if (dialog.is_goingon()) {
              dialog.proceed();
            } else {
              (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(A2);
            }
          break;
        case B2:
          if (control_enable[1])
            (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(B2);
          break;
        case X2:
          if (control_enable[1])
            (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(X2);
          break;
        case Y2:
          if (control_enable[1])
            (*Model_state::get_instance()->get_player_list_ptr())[1]->fire(Y2);
          break;

        case A3:
          if (control_enable[2])
            if (dialog.is_goingon()) {
              dialog.proceed();
            } else {
              (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(A3);
            }
          break;
        case B3:
          if (control_enable[2])
            (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(B3);
          break;
        case X3:
          if (control_enable[2])
            (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(X3);
          break;
        case Y3:
          if (control_enable[2])
            (*Model_state::get_instance()->get_player_list_ptr())[2]->fire(Y3);
          break;

        case A4:
          if (control_enable[3])
            if (dialog.is_goingon()) {
              dialog.proceed();
            } else {
              (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(A4);
            }
          break;
        case B4:
          if (control_enable[3])
            (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(B4);
          break;
        case X4:
          if (control_enable[3])
            (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(X4);
          break;
        case Y4:
          if (control_enable[3])
            (*Model_state::get_instance()->get_player_list_ptr())[3]->fire(Y4);
          break;

        default:
          break;
      }
    }
  }

  void perform_logic() {
    const float time_passed = m_set.seconds();
    float processing_time = time_passed - m_time_passed;
    m_time_passed = time_passed;

    if (Model_state::get_instance()->get_monster_list_ptr()->empty() && lvl < 2)
      set_level(++lvl);

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

  void render_status_helper(int p_x, Player* p_ptr) {
    Zeni::String player_texture, player_skill_texture;
    Point2f loc;
    switch (p_x) {
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
    float hp_percent = p_ptr->get_hp()/kHp_max;
    float mp_percent = p_ptr->get_mp()/kMp_max;
    float kCDbar_length = 30.0f;
    float kCDbar_width = 10.0f;
    Zeni::String kCDbar_color = "blue";

    
    Zeni::Video &vr = Zeni::get_Video();
    Zeni::Colors &cr = Zeni::get_Colors();

    Point2f Hpbar_loc(loc.x + 80.0f, loc.y);
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

    //CDbar
    
    Point2f CDbar_loc(loc.x, loc.y + head_size.y + 5.0f);
    /*
    Zeni::Vertex2f_Color p00(CDbar_loc, cr[kCDbar_color]);
    Zeni::Vertex2f_Color p01(CDbar_loc + Zeni::Point2f(0.0f, kCDbar_width), cr[kCDbar_color]);
    Zeni::Vertex2f_Color p02(CDbar_loc + Zeni::Point2f(CD1percent * kCDbar_length, kCDbar_width), cr[kCDbar_color]);
    Zeni::Vertex2f_Color p03(CDbar_loc + Zeni::Point2f(CD1percent * kCDbar_length, 0.0f), cr[kCDbar_color]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> bar1(p00, p01, p02, p03);
    */
    Zeni::render_image(player_skill_texture + "1",CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    Zeni::render_image("cd_mask", CDbar_loc + Zeni::Vector2f(0, kCDbar_length *(CD1percent)) ,CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));

    CDbar_loc += Point2f(kCDbar_length + 5.0f, 0.0f);
    /*
    Zeni::Vertex2f_Color p10(CDbar_loc, cr[kCDbar_color]);
    Zeni::Vertex2f_Color p11(CDbar_loc + Zeni::Point2f(0.0f, kCDbar_width), cr[kCDbar_color]);
    Zeni::Vertex2f_Color p12(CDbar_loc + Zeni::Point2f(CD2percent * kCDbar_length, kCDbar_width), cr[kCDbar_color]);
    Zeni::Vertex2f_Color p13(CDbar_loc + Zeni::Point2f(CD2percent * kCDbar_length, 0.0f), cr[kCDbar_color]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> bar2(p10, p11, p12, p13);
    */
    Zeni::render_image(player_skill_texture + "2",CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    Zeni::render_image("cd_mask", CDbar_loc + Zeni::Vector2f(0, kCDbar_length *(CD2percent)) ,CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));

    CDbar_loc += Point2f(kCDbar_length + 5.0f, 0.0f);
    /*
    Zeni::Vertex2f_Color p20(CDbar_loc, cr[kCDbar_color]);
    Zeni::Vertex2f_Color p21(CDbar_loc + Zeni::Point2f(0.0f, kCDbar_width), cr[kCDbar_color]);
    Zeni::Vertex2f_Color p22(CDbar_loc + Zeni::Point2f(CD3percent * kCDbar_length, kCDbar_width), cr[kCDbar_color]);
    Zeni::Vertex2f_Color p23(CDbar_loc + Zeni::Point2f(CD3percent * kCDbar_length, 0.0f), cr[kCDbar_color]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> bar3(p20, p21, p22, p23);
    */
    Zeni::render_image(player_skill_texture + "3",CDbar_loc, CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));
    Zeni::render_image("cd_mask", CDbar_loc + Zeni::Vector2f(0, kCDbar_length *(CD3percent)) ,CDbar_loc+Zeni::Vector2f(kCDbar_length, kCDbar_length));

//    vr.render(bar1);
  //  vr.render(bar2);
   // vr.render(bar3);
    vr.render(hpbar);
    vr.render(mpbar);
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
    Point2f Map_p0 = (Point2f(-20.0f, -20.0f) - center_location) * scale + Map_center;
    Point2f Map_p1 = (Point2f(-20.0f, 3100.0f) - center_location) * scale + Map_center;
    Point2f Map_p2 = (Point2f(1020.0f, 3100.0f) - center_location) * scale + Map_center;
    Point2f Map_p3 = (Point2f(1020.0f, -20.0f) - center_location) * scale+ Map_center;
    /*
    Vertex2f_Texture text_p0(Map_p0, Point2f(0.0f,0.0f));
    Vertex2f_Texture text_p1(Map_p1, Point2f(0.0f, 154.0f));
    Vertex2f_Texture text_p2(Map_p2, Point2f(50.0f, 154.0f));
    Vertex2f_Texture text_p3(Map_p3, Point2f(50.0f, 0.0f));
    */
    Vertex2f_Texture text_p0(Map_p0, Point2f(0.0f,0.0f));
    Vertex2f_Texture text_p1(Map_p1, Point2f(0.0f, 30.0f));
    Vertex2f_Texture text_p2(Map_p2, Point2f(10.0f, 30.0f));
    Vertex2f_Texture text_p3(Map_p3, Point2f(10.0f, 0.0f));
    map[0] = text_p0;
    map[1] = text_p1;
    map[2] = text_p2;
    map[3] = text_p3;
    Material a("floor");
    map.fax_Material(&a);
    vr.render(map);

    Model_state::get_instance()->render();

    /* render top panel */
    Zeni::Colors &cr = Zeni::get_Colors();
    const Zeni::String kToppanel_color = "white_light";
    Zeni::Vertex2f_Color p00(Zeni::Point2f(0.0f, 0.0f), cr[kToppanel_color]);
    Zeni::Vertex2f_Color p01(Zeni::Point2f(0.0f, 75.0f), cr[kToppanel_color]);
    Zeni::Vertex2f_Color p02(Zeni::Point2f(800.0f, 75.0f), cr[kToppanel_color]);
    Zeni::Vertex2f_Color p03(Zeni::Point2f(800.0f, 0.0f), cr[kToppanel_color]);
    Zeni::Quadrilateral<Zeni::Vertex2f_Color> toppanel(p00, p01, p02, p03);
    vr.render(toppanel);

    /* render the PLAYER STATUS */
    std::vector<Player *> * plist = Model_state::get_instance()->get_player_list_ptr();
    int p_x = 0;
    for (vector<Player *>::iterator it = plist->begin(); it != plist->end(); it++) {
      render_status_helper(p_x++, *it);
    }

    /* render level status */
    char* str = new char[10];
    sprintf(str, "%d", lvl+1);
    Zeni::String text_buf = "Level ";
    text_buf += str;
    Zeni::Font &l_ft = get_Fonts()["lvl_ft"];
    l_ft.render_text(text_buf,
                   Point2f(400.0f, 25.0f - 0.5f * l_ft.get_text_height()),
                   get_Colors()["black"],
                   ZENI_CENTER);

    /* render money amount */
    sprintf(str, "%d", Model_state::get_instance()->get_money_amount());
    text_buf = "$ ";
    text_buf += str;
    Zeni::Font &m_ft = get_Fonts()["money_ft"];
    m_ft.render_text(text_buf,
                   Point2f(400.0f, 60.0f - 0.5f * m_ft.get_text_height()),
                   get_Colors()["orange"],
                   ZENI_CENTER);
    delete [] str;


    render_minimap();
    dialog.render();
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

class Preparation_State :public Gamestate_II { //public Widget_Gamestate,
  Preparation_State(const Preparation_State &);
  Preparation_State operator=(const Preparation_State &);
public:
  Preparation_State()
  : chosen_num(0)
    //: Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
    set_pausable(true);
    Model_state::get_instance()->set_player_num(1);
    //m_widgets.lend_Widget(play_button);
    //m_widgets.lend_Widget(menu_button);

    set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), MENU);
    //p1
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 0), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 0), HORI1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 0), VERT1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 0), A1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 0), B1);
    //p2
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 1), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 1), HORI2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 1), VERT2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 1), A2);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 1), B2);
    //p3
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 2), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 2), HORI3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 2), VERT3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 2), A3);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 2), B3);
    //p4
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_START, 3), MENU);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X /* x-axis */, 3), HORI4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y /* y-axis */, 3), VERT4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_A, 3), A4);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, Joysticks::BUTTON_B, 3), B4);


    
    for (int i=0; i < Model_state::get_instance()->get_player_num(); i++) {
      cursor_pos[i] = kCursor_min;
      p_decided[i] = false;
      p_color[i] = Color();
      char_available[i] = true;
    }

  }

private:
  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    get_Game().joy_mouse.enabled = false;
  }

  void save_chosen_characters() {
    std::vector<kPlayer_type> * char_list = Model_state::get_instance()->get_character_list_ptr();
    for (int i=0; i < Model_state::get_instance()->get_player_num(); i++) {
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
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }
  
  void move_cursor(const int player_n, const bool is_to_right) {
    if (!p_decided[player_n]) {
      if (is_to_right) {
        cursor_pos[player_n]++;
      } else {
        cursor_pos[player_n]--;
      }
      if (cursor_pos[player_n] > kCursor_max) {
        cursor_pos[player_n] -= kCursor_max + 1;
      } else if (cursor_pos[player_n] < kCursor_min) {
        cursor_pos[player_n] += kCursor_max + 1;
      }
    }
  }

  /*
  SANZANG,
  WUKONG,
  SHASENG,
  BAJIE
  */
  bool char_available[4];
  bool p_decided[4];
  kPlayer_type chosen_char[4];
  int cursor_pos[4];
  Color p_color[4];
  int chosen_num;

  void choose_char(const int player_n) {
    if (!p_decided[player_n]) {
      int pos = cursor_pos[player_n];
      if (char_available[pos]) {
        switch (pos) {
          case 0:
            chosen_char[player_n] = SANZANG;
            break;
          case 1:
            chosen_char[player_n] = WUKONG;
            break;
          case 2:
            chosen_char[player_n] = SHASENG;
            break;
          case 3:
            chosen_char[player_n] = BAJIE;
            break;
          default:
            break;
        }
        chosen_num++;
        p_decided[player_n] = true;
        p_color[pos] = Color(1.0f, 0.3f, 0.3f, 0.3f);
        char_available[pos] = false;
      }
      if (chosen_num == Model_state::get_instance()->get_player_num()) {
        get_Game().pop_state();
        //get_Game().push_state(new Upgrade_state());
        get_Game().push_state(new Play_State());
      }
    }
  }

  void on_event(const Zeni_Input_ID &, const float &confidence, const int &action) {
    switch(action) {
      case HORI1:
        if (confidence >= 1.0f)
          move_cursor(0, true);
        else if (confidence <= -1.0f)
          move_cursor(0, false);
        break;
      case HORI2:
        if (confidence >= 1.0f)
          move_cursor(1, true);
        else if (confidence <= -1.0f)
          move_cursor(1, false);
        break;
      case HORI3:
        if (confidence >= 1.0f)
          move_cursor(2, true);
        else if (confidence <= -1.0f)
          move_cursor(2, false);
        break;
      case HORI4:
        if (confidence >= 1.0f)
          move_cursor(3, true);
        else if (confidence <= -1.0f)
          move_cursor(3, false);
        break;
      default:
        break;
    }
    //get_Game().push_state(new Play_State());
    if(confidence >= 1.0f) {
      switch(action) {
        case MENU: {
          Game &game = get_Game();
          //game.push_state(new Play_State());
          game.push_state(new Popup_Menu_State);
          break;
        }
        case A1:
          choose_char(0);
          break;
        case A2:
          choose_char(1);
          break;
        case A3:
          choose_char(2);
          break;
        case A4:
          choose_char(3);
          break;
        default:
          break;
      }
    }
  }

  void render() {
    //Widget_Gamestate::render();
    render_image("pigsy_front0",
       Point2f(600.0f, 150.0f),
       Point2f(800.0f, 500.0f),
       false,
       p_color[3]);
    render_image("friar_sand_front0",
       Point2f(400.0f, 150.0f),
       Point2f(650.0f, 500.0f),
       false,
       p_color[2]);
    render_image("monkey_king_front0",
       Point2f(200.0f, 150.0f),
       Point2f(450.0f, 500.0f),
       false,
       p_color[1]);
    render_image("tripitaka_front0",
       Point2f(50.0f, 150.0f),
       Point2f(250.0f, 500.0f),
       false,
       p_color[0]);
    float pos[4] = {30.0f, 180.0f, 380.0f, 580.0f};

      render_image("p1cursor",
         Point2f(pos[cursor_pos[0]], 100.0f),
         Point2f(pos[cursor_pos[0]]+50.0f, 200.0f),
         false,
         p_color[cursor_pos[0]]);
      if (Model_state::get_instance()->get_player_num() > 1) {
        render_image("p2cursor",
           Point2f(pos[cursor_pos[1]], 200.0f),
          Point2f(pos[cursor_pos[1]]+50.0f, 300.0f),
          false,
          p_color[cursor_pos[1]]);
        render_image("p3cursor",
          Point2f(pos[cursor_pos[2]], 300.0f),
          Point2f(pos[cursor_pos[2]]+50.0f, 400.0f),
          false,
          p_color[cursor_pos[2]]);
        render_image("p4cursor",
          Point2f(pos[cursor_pos[3]], 400.0f),
          Point2f(pos[cursor_pos[3]]+50.0f, 500.0f),
          false,
          p_color[cursor_pos[3]]);
      }
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
