#include "Player.h"

using namespace std;
using namespace Flame;

Player::Player(
  Chronometer<Time>* game_t_,
  const float &health_,
  const float &speed_,
  const float &radius_,
  const Zeni::Point2f &location_,
  const kPlayer_type player_type_)
: Agent(health_, speed_, radius_, location_),
  crazy(false),
  normal_attack(false),
  running_status(false),
  ptype(player_type_),
//size(Zeni::Vector2f(radius_ * 2, radius_ * 2))
  size(radius_),
  attack_buff(kInit_buff),
  game_time(game_t_),
  spell1_active(false),
  spell2_active(false),
  spell3_active(false),
  last_spell1(0.0f),
  last_spell2(0.0f),
  last_spell3(0.0f)
{
  const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
  render_clock = current_time;
  last_htime = current_time;

  switch (ptype) {
    case SANZANG:
      spell1_CD = kSpell1_CD;
      spell2_CD = kSpell2_CD;
      spell3_CD = kSpell3_CD;
      break;
    case WUKONG:
      spell1_CD = kSpell1_CD;
      spell2_CD = kSpell2_CD;
      spell3_CD = kSpell3_CD;
      break;
    case SHASENG:
      spell1_CD = kStrafe_CD;
      spell2_CD = kTrap_CD;
      spell3_CD = kMagicarrow_CD;
      break;
    default: // case BAJIE:
      spell1_CD = kShield_CD;
      spell2_CD = kTaunt_CD;
      spell3_CD = kBloodsuck_CD;
      break;
  }
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

  // local spell removing
  const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
  float passed_time = float(current_time.get_seconds_since(last_htime));
  if (normal_attack && passed_time > kAttack_show_time) {
    normal_attack = false;
  }

  if (ptype == BAJIE && spell1_active && float(current_time.get_seconds_since(last_spell1)) > kShield_last) {
    spell1_active = false;
    set_armor(backup_armor);
  }
  if (ptype == BAJIE && spell3_active && float(current_time.get_seconds_since(last_spell3)) > kBloodsuck_last) {
    spell3_active = false;
  }

  //

  float render_passed_time = float(current_time.get_seconds_since(render_clock));
  if (render_passed_time > kRun_render_gap) {
    running_status = !running_status;
    render_clock = current_time;
  }

  Model_state::get_instance()->can_move_player(get_body());
}

void Player::render() {
  Vector2f orient = get_current_orientation();
  float rad = orient_vec_to_radians(orient);
  float scale = Model_state::get_instance()->get_scale();
  Zeni::String player_texture;
  string ttype;
  switch (ptype) {
    case SANZANG:
      ttype = "tripitaka";
      break;
    case WUKONG:
      ttype = "monkey_king";
      break;
    case SHASENG:
      ttype = "friar_sand";
      break;
    default: // case BAJIE:
      ttype = "pigsy";
      break;
  }
  if (rad <= 0.25f * Global::pi && rad > -0.25f * Global::pi) {
    ttype += "_front";
  } else if (rad <= 0.75f * Global::pi && rad > 0.25f * Global::pi) {
    ttype += "_right";
  } else if ((rad <= Global::pi && rad > 0.75f * Global::pi) ||
             (rad >= -Global::pi && rad < -0.75f * Global::pi)) {
    ttype += "_back";
  } else {
    ttype += "_left";
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
  if (spell1_active) {
    render_image("shield",
           Point2f(rel_loc.x - size * scale, rel_loc.y - size * scale),
           Point2f(rel_loc.x + size * scale, rel_loc.y + size * scale));
  }
  if (spell3_active) {
    render_image("bloodsuck",
           Point2f(rel_loc.x - size * scale, rel_loc.y - size * scale),
           Point2f(rel_loc.x + size * scale, rel_loc.y + size * scale));
  }

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
  if (normal_attack)

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
      
  Agent::render();
}

void Player::fire(kKey_type type) {
  if (!game_time->is_running()) {
    return;
  }

  const Zeni::Time_HQ current_time = Zeni::get_Timer_HQ().get_time();
  float passed_time = float(current_time.get_seconds_since(last_htime));
  if (passed_time > PLAYER_ATTACK_INTERVAL) {
    last_htime = current_time;

    switch (type) {
      case A1:
      case A2:
      case A3:
      case A4:
        try_normal_attack(current_time);

        /*
            Attack_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                 float radius_ = 0.f,
                 float attack_strength_ = 0.f,
                 bool is_player_ = false,
                 bool heal_self_ = false,
                 Player * player_ptr_ = nullptr);
                 */
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

void Player::get_crazy() {
  crazy = true;
  attack_buff = 2.0f;
}



void Player::shield() {
  spell1_active = true;
  backup_armor = get_armor();
  set_armor(backup_armor * kShield_effect);
}

void Player::taunt() {
  spell2_active = true;
  Taunt* new_spell = new Taunt(get_location(), this);
  Model_state::get_instance()->add_spell(new_spell);
}

void Player::bloodsuck() {
  spell3_active = true;
}

void Player::try_normal_attack(const Zeni::Time_HQ current_time) {
  Spell* new_spell;
  if (ptype == SHASENG) {
    new_spell = new Arrow_attack(get_location(), get_current_orientation());
    Model_state::get_instance()->add_spell(new_spell);
  } else {
    bool heal_self = (ptype == BAJIE) && spell3_active;
    new_spell = new Attack_spell(get_location(),
                                          get_current_orientation(),
                                          kPlayer_attack_range,
                                          kPlayer_attack_strengh * attack_buff,
                                          true,
                                          heal_self,
                                          this);
    normal_attack = true;
    Model_state::get_instance()->add_spell(new_spell);
  }
  
}
void Player::try_spell1() {
  //float passed_time = float(current_time.get_seconds_since(last_spell1));
  float current_time = game_time->seconds();
  float passed_time = current_time - last_spell1;
  Spell* new_spell;
  if (passed_time > spell1_CD) {
    last_spell1 = current_time;
    //create spell based on character type
    switch (ptype) {
      case SANZANG: //stopping
        //new_spell = new Healing_spell(get_location(), get_current_orientation());
        //new_spell = new Arrow_attack(get_location(), get_current_orientation());
        new_spell = new Fire_ball(get_location(), get_current_orientation());
        Model_state::get_instance()->add_spell(new_spell);
        break;
      case WUKONG: //Cudgel Fury
        break;
      case SHASENG: //Strafe
        new_spell = new Strafe(get_location(), get_current_orientation());
        Model_state::get_instance()->add_spell(new_spell);
        break;
      default: // case BAJIE: Shield
        shield();
        break;
    }
  }
}

void Player::try_spell2() {
  //float passed_time = float(current_time.get_seconds_since(last_spell2));
  float current_time = game_time->seconds();
  float passed_time = current_time - last_spell2;
  Spell* new_spell;
  if (passed_time > spell2_CD) {
    last_spell2 = current_time;
    //create spell based on character type
    switch (ptype) {
      case SANZANG: //healing_spell
        break;
      case WUKONG: //Charge
        break;
      case SHASENG: //Trap
        new_spell = new Trap(get_location());
        Model_state::get_instance()->add_spell(new_spell);
        break;
      default: // case BAJIE: Taunt
        taunt();
        break;
    }
  }
}

void Player::try_spell3() {
  //float passed_time = float(current_time.get_seconds_since(last_spell3));
  float current_time = game_time->seconds();
  float passed_time = current_time - last_spell3;
  Spell* new_spell;
  if (passed_time > spell3_CD) {
    last_spell3 = current_time;
    //create spell based on character type
    switch (ptype) {
      case SANZANG: //all_healing
        break;
      case WUKONG: //Berserk
        break;
      case SHASENG: //Magic Arrow
        new_spell = new Magic_arrow(get_location(), get_current_orientation());
        Model_state::get_instance()->add_spell(new_spell);
        break;
      default: // case BAJIE:  Life Trap
        bloodsuck();
        break;
    }
  }
}
