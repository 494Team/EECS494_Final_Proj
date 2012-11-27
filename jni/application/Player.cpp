#include "Player.h"

using namespace std;
using namespace Flame;

Player::Player(
  const float &health_,
  const float &speed_,
  const float &radius_,
  const Zeni::Point2f &location_)
: Agent(health_, speed_, radius_, location_),
  wpinuse(false),
  damaged(false),
  running_status(false),
//size(Zeni::Vector2f(radius_ * 2, radius_ * 2))
  size(radius_)
{
  render_clock = Zeni::get_Timer_HQ().get_time();
}

void Player::update(float time) {
  Point2f backup_position = get_location();
  float scale = Model_state::get_instance()->get_scale();
  if (abs(ctrl.move_hori) + abs(ctrl.move_vert) > 0.3f) {
    Vector2f dir(ctrl.move_hori, ctrl.move_vert);
    set_orientation(dir);

    if (!ctrl.l) {
      Point2f new_position = backup_position + Point2f(ctrl.move_hori * time * get_current_speed(), 0.0f);
      set_position(new_position);
      update_body();
      if (!Model_state::get_instance()->can_move(get_body())) {
          set_position(backup_position);
          update_body();
      }

      backup_position = get_location();
      new_position = backup_position + Point2f(0.0f, ctrl.move_vert * time * get_current_speed());
      set_position(new_position);
      update_body();
      if (!Model_state::get_instance()->can_move(get_body())) {
          set_position(backup_position);
          update_body();
      }
    }
  }

  rel_loc = (get_location() - Model_state::get_instance()->get_center_location()) * scale + Point2f(400.0f, 300.0f);

  const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
  float passed_time = float(current_time.get_seconds_since(last_htime));
  if (wpinuse && passed_time > kAttack_show_time) {
    wpinuse = false;
  }

  float render_passed_time = float(current_time.get_seconds_since(render_clock));
  if (render_passed_time > kRun_render_gap) {
    running_status = !running_status;
    render_clock = current_time;
  }
    

  //hit
  if (wpinuse && !damaged) {
    //create damage
    damaged = true;
    //search the enemy list
  }
  Model_state::get_instance()->can_move_player(get_body());
}

void Player::render() {
  Vector2f orient = get_current_orientation();
  float rad = orient_vec_to_radians(orient);
  float scale = Model_state::get_instance()->get_scale();
  Zeni::String player_texture;
  string ttype;

  if (rad <= 0.25f * Global::pi && rad > -0.25f * Global::pi) {
    ttype = "monkey_king_front";
  } else if (rad <= 0.75f * Global::pi && rad > 0.25f * Global::pi) {
    ttype = "monkey_king_right";
  } else if ((rad <= Global::pi && rad > 0.75f * Global::pi) ||
             (rad >= -Global::pi && rad < -0.75f * Global::pi)) {
    ttype = "monkey_king_back";
  } else {
    ttype = "monkey_king_left";
  }
  if (running_status) {
    ttype += "1";
  } else {
    ttype += "0";
  }
  
  player_texture = Zeni::String(ttype);//("monkey_king_front");

render_image(player_texture,
       Point2f(rel_loc.x - size * scale, rel_loc.y - size * scale),
       Point2f(rel_loc.x + size * scale, rel_loc.y + size * scale));

  //render the orientation arrow
  /*
render_image(
    "rarrow3", // which texture to use
    loc + Point2f(0.0f, 50.0f), // upper-left corner
    loc + Point2f(50.0f, 70.0f),//m_size, // lower-right corner
    rad,//0.0f * Global::pi, // rotation in radians
    1.0f, // scaling factor
    loc + Point2f(25.0f, 25.0f), // point to rotate & scale about
    false);//, // whether or not to horizontally flip the texture
    //filter); // what Color to "paint" the texture*/
render_image(
  "rarrow3",
  Point2f(rel_loc.x - size, rel_loc.y + size ),
  Point2f(rel_loc.x + size, rel_loc.y + size + size),
  rad,
  scale,
  rel_loc,
  false);
  //render weapon attacking
  if (wpinuse)

 /*
    render_image(
      "sword_attack", // which texture to use
      loc + Point2f(0.0f, 50.0f), // upper-left corner
      loc + Point2f(50.0f, 90.0f),//m_size, // lower-right corner
      orient_vec_to_radians(get_current_orientation()),//0.0f * Global::pi, // rotation in radians
      1.0f, // scaling factor
      loc + Point2f(25.0f, 25.0f), // point to rotate & scale about
      false);//, // whether or not to horizontally flip the texture
      //filter); // what Color to "paint" the texture*/
    render_image("sword_attack_1",
                 Point2f(rel_loc.x - size, rel_loc.y + size ),
                 Point2f(rel_loc.x + size, rel_loc.y + size  + size * 2),
                 rad,
                 scale,
                 rel_loc,
                 false);
      
}

void Player::fire(kKey_type type) {
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
        try_spell1();
        break;
      case X1:
      case X2:
      case X3:
      case X4:
        //spell 2
        try_spell2();
        break;
      case Y1:
      case Y2:
      case Y3:
      case Y4:
        //spell 3
        try_spell3();
        break;
      default:
        break;
    }
  } else {
    //in PLAYER_ATTACK_INTERVAL
  }
}

void Player::try_spell1() {
}

void Player::try_spell2() {
}

void Player::try_spell3() {
}