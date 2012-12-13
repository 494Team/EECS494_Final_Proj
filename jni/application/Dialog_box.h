#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H
#include "Model_state.h"
#include "Collision.h"
#include "Sim_object.h"

namespace Flame {
  const int dialog_max[6] = {6, 5, 2, 1, 5, 1};
  const Zeni::String kBoss_name[2] = {"Red Boy", "Bull King"};

  class Dialog_box{
  private:
    int episode;
    int dialog_n;
    bool in_conversation;
    Chronometer<Time>* game_time;
    Time_HQ* entering_time_ptr;
  public:
    Dialog_box(Chronometer<Time>* game_t, Time_HQ* entering_time_ptr_)
    : in_conversation(false),
      dialog_n(0),
      game_time(game_t),
      entering_time_ptr(entering_time_ptr_)
    {
    }

    bool is_goingon() {
      return in_conversation;
    }

    void start(const int episode_) {
      episode = episode_;
      dialog_n = 0;
      in_conversation = true;
      //game_time->pause_all();
      *entering_time_ptr = Zeni::get_Timer_HQ().get_time();
    }

    void proceed() {
      if (in_conversation) {
        if (dialog_n < dialog_max[episode]-1) {
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
        Point2f(0.0f, 0.0f),
        Point2f(1024.0f, 1024.0f));
     
      Point2f name_loc(20.0f, 420.0f);
      Zeni::Font &fr = get_Fonts()["dialog_ft"];

      Zeni::String name = "Someone";
      Zeni::String text;
      Point2f text_loc(80.0f, 450.0f);
      
      if (episode == 0) {
        switch (dialog_n) {
          case 0:
            name = "Background";
            text = "Dismayed that \"the land of the South knows only greed, \nhedonism, promiscuity, and sins\", the Buddha instructs \nTang China to find someone to take the Buddhist sutras \nof \"transcendence and persuasion for good will\" back \nto the East";
            break;
          case 1:
            name = "Background";
            text = "Sanzang and his three disciples Wukong, Bajie and \nShaseng set out to bring back Buddhist scriptures \nfrom India, but encounters various evils along the";
            break;
          case 2:
            name = "Background";
            text = "way. And here is an episode of their adventure story \nwhen they try to get through the impassably flaming \nmountains.";
            break;
          case 3:
            name = "Bajie";
            text = "It is so hot here!";
            break;
          case 4:
            name = "Sanzang The Master";
            text = "The flame with evil power is blocking our way. The fire \ncannot be put out by water! How can we have to \nget through this mountain? We have to do it.";
            break;
          case 5:
            name = "Bajie";
            text = "I know the Bull King has a magical palm leaf fan which \ncan put out this fire. He's my friend and I think he would \nagree to borrow it to us. Let's find him!";
            break;
          default:
            break;
        }
      } else if (episode == 1) {        switch (dialog_n) {
          case 0:
            name = "Red Boy";
            text = "Who are you, Strangers?";
            break;
          case 1:
            name = "Sanzang The Master";
            text = "We came for borrowing the palm leaf fan from your \nfather, the Bull King. Wukong is his friend.";
            break;
          case 2:
            name = "Red Boy";
            text = "I don't know Wukong. You must be coveting our treasure.";
            break;
          case 3:
            name = "Wukong";
            text = "Let me talk to your father.";
            break;
          case 4:
            name = "Red Boy";
            text = "I'll believe you if you come to find me in the third stage. \nMy father's friends must be able to defeat enemies in \nthe first and second stages.";
            break;
          default:
            break;
        }
      } else if (episode == 2) {
        switch (dialog_n) {
          case 0:
            name = "Sanzang The Master";
            text = "We are here, kid. Let's see your father.";
            break;
          case 1:
            name = "Red Boy";
            text = "Wait. Are you Sanzang? I heard that if anyone eat you, \nhe would live forever. You are dying and I'm gonna be \nimmortal!";
            break;
          default:
            break;
        }
      } else if (episode == 3) {
        switch (dialog_n) {
          case 0:
            name = "Sanzang The Mastser";
            text = "Let's find Bull King by ourself.";
            break;
          default:
            break;
        }
      } else if (episode == 4) {
        switch (dialog_n) {
          case 0:
            name = "Wukong";
            text = "Hey, Bull King.";
            break;
          case 1:
            name = "Bull King";
            text = "Wukong, you were my friend but you betrayed me and \nbecame a Buddhist. My son Red Boy just told me that \nyou are trying to steal my treasure. How dare you!";
            break;
          case 2:
            name = "Wukong";
            text = "Let me explain.";
            break;
          case 3:
            name = "Bull King";
            text = "Don't! I can only forgive you if you let me eat Sanzang \nand become immortal.";
            break;
          case 4:
            name = "Wukong";
            text = "Are you crazy? He's the master!";
            break;
          default:
            break;
        }
      } else if (episode == 5) {
        switch (dialog_n) {
          case 0:
            text = "I admit I were tempted by Sanzang's meat. Here's the \nmagical palm leaf fan. Everyone is dreaming about \nbeing immortal. Be careful.";
            break;
          default:
            break;
        }
      }
      if (name != "Background")
        name += ":";

      fr.render_text(name, name_loc, get_Colors()["title_text"], ZENI_LEFT);
      fr.render_text(text, text_loc, get_Colors()["title_text"], ZENI_LEFT);
    }


  };
}

#endif
