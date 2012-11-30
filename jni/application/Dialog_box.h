#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H
#include "Model_state.h"
#include "Collision.h"
#include "Sim_object.h"

namespace Flame {
  class Dialog_box{
  private:
    Dialog_state state;
  public:
    Dialog_box(const Dialog_state start_state) {
      state = start_state;
    }
    void set_state(const Dialog_state new_state) {
      state = new_state;
    }
    void render() {
      if (state == NO_DIALOG)
        return;

      render_image("dialog_box",
        Point2f(0.0f, 400.0f),
        Point2f(800.0f, 600.0f));
      render_image("button",
        Point2f(710.0f, 500.0f),
        Point2f(760.0f, 550.0f));

      Zeni::String name;
      Point2f name_loc(20.0f, 420.0f);
      Zeni::Font &fr = get_Fonts()["dialog_ft"];
      switch (state) {
        case LVL1_1:
          name = "BOSS";
          break;
        default:
          name = "BOSS";
          break;
      }
      name += ":";
      Zeni::String text;
      Point2f text_loc(80.0f, 460.0f);
      switch (state) {
        case LVL1_1:
          text = "Come on!";
          break;
        default:
          text = "Come on!";
          break;
      }

      fr.render_text(name, name_loc, get_Colors()["title_text"], ZENI_LEFT);
      fr.render_text(text, text_loc, get_Colors()["title_text"], ZENI_LEFT);
    }


  };
}

#endif
