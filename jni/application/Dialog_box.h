#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H
#include "Model_state.h"
#include "Collision.h"
#include "Sim_object.h"

namespace Flame {
  const int dialog_max[3] = {2, 2, 2};
  const Zeni::String kBoss_name[3] = {"BOSS1", "BOSS2", "BOSS3"};

  class Dialog_box{
  private:
    int lvl;
    int dialog_n;
    bool in_conversation;
    Chronometer<Time>* game_time;
  public:
    Dialog_box(Chronometer<Time>* game_t)
    : in_conversation(false),
      dialog_n(0),
      game_time(game_t)
    {
    }

    bool is_goingon() {
      return in_conversation;
    }

    void start(const int lvl_) {
      lvl = lvl_;
      dialog_n = 0;
      in_conversation = true;
      game_time->pause_all();
    }

    void proceed() {
      if (in_conversation) {
        if (dialog_n < dialog_max[lvl]) {
          dialog_n++;
        } else {
          in_conversation = false;
          game_time->unpause_all();
        }
      }
    }

    void render() {
      if (!in_conversation)
        return;

      render_image("dialog_box",
        Point2f(0.0f, 400.0f),
        Point2f(800.0f, 600.0f));
      render_image("button",
        Point2f(710.0f, 500.0f),
        Point2f(760.0f, 550.0f));

      Zeni::String name = kBoss_name[lvl];
      Point2f name_loc(20.0f, 420.0f);
      Zeni::Font &fr = get_Fonts()["dialog_ft"];

      name += ":";
      Zeni::String text;
      Point2f text_loc(80.0f, 460.0f);
      
      if (lvl == 0) {
        switch (dialog_n) {
          case 0:
            text = "Come on!";
            break;
          case 1:
            text = "Follow me!";
            break;
          case 2:
            text = "Yes!";
            break;
          default:
            break;
        }
      } else if (lvl == 1) {
      } else { //lvl == 2
      }

      fr.render_text(name, name_loc, get_Colors()["title_text"], ZENI_LEFT);
      fr.render_text(text, text_loc, get_Colors()["title_text"], ZENI_LEFT);
    }


  };
}

#endif
