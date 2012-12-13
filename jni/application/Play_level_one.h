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
#include "Dialog_box.h"
#include "Redboy.h"
#include <iostream>

class Play_level_one : public Zeni::Gamestate_II{
  Play_level_one(const Play_level_one &);
  Play_level_one operator = (const Play_level_one&);
  Flame::Dialog_box dialog;
  int lvl;
  Time_HQ tmp;
public:
  Play_level_one();

  void begin_dialog(Flame::Dialog_box* dialog_ptr, int lvl){
    dialog_ptr->start(lvl);
  }

  void monster_init(const int &lvl_);

  void on_push(){
    Zeni::get_Window().mouse_hide(true);
    Zeni::get_Game().joy_mouse.enabled = false;
  }

  void on_pop(){
    Zeni::get_Window().mouse_hide(false);
    Zeni::get_Game().joy_mouse.enabled =true;
  }

  void on_cover(){
    Zeni::get_Window().mouse_hide(false);
    Zeni::get_Game().joy_mouse.enabled = true;
  }

  void on_uncover(){
    Zeni::get_Window().mouse_hide(true);
    Zeni::get_Game().joy_mouse.enabled = false;
  }

  void on_event(const Zeni::Zeni_Input_ID&, const float &confidence, const int &action);
  void perform_logic();
  void render_status_helper(int p_x, Flame::Player* p_ptr);
  void render();

private:
  float m_time_passed;
  Chronometer<Time> m_set;

};