#ifndef PLAYER_H
#define PLAYER_H

#include "Agent.h"
#include <vector>
//#include <zenilib.h>
using namespace Zeni;

namespace Flame {
  #define PLAYER_ATTACK_INTERVAL 0.5f
  const float kAttack_show_time = 0.2f;
  #define PLAYER_SPELL1_CD 1.0f
  #define PLAYER_SPELL2_CD 1.0f
  #define PLAYER_SPELL3_CD 1.0f
  #define HURT_SHOWING_INTERVAL 0.2f
  const float kMove_speed = 500.0f;

  class Player: public Agent {
  public:
    Player(
      const float &health_ = 0.0f,
      const float &speed_ = 0.0f,
      const float &radius_ = 0.0f,
      const Zeni::Point2f &location_ = Zeni::Point2f())
    : Agent(health_, speed_, radius_, location_),
      wpinuse(false),
      damaged(false)
    {
    }
    void update(float time)  {
      Point2f backup_position = get_location();
      
      if (abs(ctrl.move_hori) + abs(ctrl.move_vert) > 0.3f) {
        Vector2f dir(ctrl.move_hori, ctrl.move_vert);
        set_orientation(dir);

        if (!ctrl.l) {
          Point2f new_position = backup_position + Point2f(ctrl.move_hori * time * get_current_speed(), 0.0f);
          set_position(new_position);
          //if (m_monkey_king.collide(m_collision_rectangle))
          //  monkey_king_position = backup_position;
          backup_position = get_location();
          new_position = backup_position + Point2f(0.0f, ctrl.move_vert * time * get_current_speed());
          set_position(new_position);
          //if (m_monkey_king.collide(m_collision_rectangle))
          //  monkey_king_position = backup_position;
          //m_monkey_king.set_position(monkey_king_position);
        }
      }

      const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
      float passed_time = float(current_time.get_seconds_since(last_htime));
      if (wpinuse && passed_time > kAttack_show_time) {
        wpinuse = false;
      }

      //hit
      if (wpinuse && !damaged) {
        //create damage
        damaged = true;
        //search the enemy list
      }
    }

    void render() {
      Point2f loc = get_location();
      Vector2f orient = get_current_orientation();
      float rad = orient_vec_to_radians(orient);

      if (rad <= 0.25f * Global::pi && rad > -0.25f * Global::pi) {
        render_image("monkey_king_front",
                     loc,
                     loc + 5.0f * Vector2f(10.0f, 10.0f));
      } else if (rad <= 0.75f * Global::pi && rad > 0.25f * Global::pi) {
        render_image("monkey_king_right",
                     loc,
                     loc + 5.0f * Vector2f(10.0f, 10.0f));
      } else if (rad <= Global::pi && rad > 0.75f * Global::pi ||
                rad >= -Global::pi && rad < -0.75f * Global::pi) {
        render_image("monkey_king_back",
                     loc,
                     loc + 5.0f * Vector2f(10.0f, 10.0f));
      } else {
        render_image("monkey_king_left",
                     loc,
                     loc + 5.0f * Vector2f(10.0f, 10.0f));
      }

      //render the orientation arrow
      render_image(
        "rarrow3", // which texture to use
        loc + Point2f(0.0f, 50.0f), // upper-left corner
        loc + Point2f(50.0f, 70.0f),//m_size, // lower-right corner
        rad,//0.0f * Global::pi, // rotation in radians
        1.0f, // scaling factor
        loc + Point2f(25.0f, 25.0f), // point to rotate & scale about
        false);//, // whether or not to horizontally flip the texture
        //filter); // what Color to "paint" the texture
      //render weapon attacking
      if (wpinuse)
        render_image(
          "sword_attack", // which texture to use
          loc + Point2f(0.0f, 50.0f), // upper-left corner
          loc + Point2f(50.0f, 90.0f),//m_size, // lower-right corner
          orient_vec_to_radians(get_current_orientation()),//0.0f * Global::pi, // rotation in radians
          1.0f, // scaling factor
          loc + Point2f(25.0f, 25.0f), // point to rotate & scale about
          false);//, // whether or not to horizontally flip the texture
          //filter); // what Color to "paint" the texture
    }

    //control
    Control ctrl;

    void fire(kKey_type type) {
      const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
      float passed_time = float(current_time.get_seconds_since(last_htime));
      if (passed_time > PLAYER_ATTACK_INTERVAL) {
        last_htime = current_time;
        switch (type) {
          case A1:
          case A2:
          case A3:
          case A4:
            wpinuse = true;
            damaged = false;
            break;
          case B1:
          case B2:
          case B3:
          case B4:
            //spell 1
            break;
          case X1:
          case X2:
          case X3:
          case X4:
            //spell 2
            break;
          case Y1:
          case Y2:
          case Y3:
          case Y4:
            //spell 3
            break;
          default:
            break;
        }
      } else {
        //in PLAYER_ATTACK_INTERVAL
      }
    }
  private:
    float orient_vec_to_radians(Vector2f vec) {
      float radians = atan2(vec.i, vec.j);
      return radians;
    }

    //attack
    bool wpinuse;
    bool damaged; //whether the normal attack has created a damage
    Zeni::Time_HQ last_htime;

    //vector<Agent*>* Monsters;
    int attack_strength;
    double attack_range;
  };



}

#endif