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
  normal_attack(false),
  running_status(false),
  ptype(player_type_),
  attack_buff(kInit_buff),
  game_time(game_t_),
  spell1_active(false),
  spell2_active(false),
  spell3_active(false),
  last_htime(0.0f),
  last_spell1(0.0f),
  last_spell2(0.0f),
  last_spell3(0.0f),
  last_charge_attack(0.0f),
  charge_attacking(false),
  render_clock(0.0f),
  render_player(true)
{
  switch (ptype) {
    case SANZANG:
      spell1_CD = kConfusing_CD;
      spell2_CD = kHealing_CD;
      spell3_CD = kAll_healing_CD;
      break;
    case WUKONG:
      spell1_CD = kCudge_fury_CD;
      spell2_CD = kCharge_CD;
      spell3_CD = kBerserk_CD;
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
  set_moving(true);
  set_orientation(Vector2f());
  monster_list_ptr = Model_state::get_instance()->get_monster_list_ptr();
}

void Player::static_move(float time) {
  Point2f backup_position;
  Zeni::Vector2f backup_ori = get_current_orientation();
  float speed = get_current_speed();

  bool move_x = true;
  bool move_y = true;
  
  backup_position = get_location();
  set_speed(speed * backup_ori.get_i().magnitude() / backup_ori.magnitude());
  set_orientation(backup_ori.get_i());
  update_location(time);
  update_body();
  if (!Model_state::get_instance()->can_move(get_body())) {
      set_position(backup_position);
      update_body();
      move_x = false;
  }

  backup_position = get_location();
  set_speed(speed * backup_ori.get_j().magnitude() / backup_ori.magnitude());
  set_orientation(backup_ori.get_j());
  update_location(time);
  update_body();
  if (!Model_state::get_instance()->can_move(get_body())) {
      set_position(backup_position);
      update_body();
      move_y = false;
  }

  set_orientation(backup_ori);
  set_speed(speed);
}

void Player::update(float time) {
  Agent::update(time);

  Point2f backup_position;
  Point2f new_position;
  float scale = Model_state::get_instance()->get_scale();

  if (!is_hitback() && !is_charging() && abs(ctrl.move_hori) + abs(ctrl.move_vert) > 0.3f) {
    bool move_x = true;
    bool move_y = true;
    if (!ctrl.l) {
      backup_position = get_location();
      set_speed(abs(ctrl.move_hori) * kSpeed_player);
      set_orientation(Vector2f(ctrl.move_hori, 0.0f));
      update_location(time);
      update_body();
      if (!Model_state::get_instance()->can_move(get_body())) {
          set_position(backup_position);
          update_body();
          move_x = false;
      }

      backup_position = get_location();
      set_speed(abs(ctrl.move_vert) * kSpeed_player);
      set_orientation(Vector2f(0.0f, ctrl.move_vert));
      update_location(time);
      update_body();
      if (!Model_state::get_instance()->can_move(get_body())) {
          set_position(backup_position);
          update_body();
          move_y = false;
      }
    }
    Vector2f dir(ctrl.move_hori, ctrl.move_vert);
    set_orientation(dir);
    set_speed(sqrt(pow(ctrl.move_hori * (int)move_x, 2) + pow(ctrl.move_vert * int(move_y), 2)));
  }

  if (is_hitback() && is_charging()) {
    charge_end();
  }
  if (is_hitback()) {
    static_move(time);
  }
  if (is_charging()) {
    charge_update(time);
  }


  rel_loc = (get_location() - Model_state::get_instance()->get_center_location()) * scale + Point2f(400.0f, 300.0f);

  // local spell removing
  float current_time = game_time->seconds();
  if (is_charge_attacking() && (current_time - last_charge_attack) > kCharge_attack_last) {
    charge_attacking = false;
  }

  if (normal_attack && (current_time - last_htime) > kAttack_show_time) {
    normal_attack = false;
  }

  if (ptype == BAJIE && spell1_active && (current_time - last_spell1) > kShield_last) {
    spell1_active = false;
    set_armor(backup_armor);
  }
  if (ptype == BAJIE && spell3_active && (current_time - last_spell3) > kBloodsuck_last) {
    spell3_active = false;
  }
  if (ptype == WUKONG && spell2_active && (current_time - last_spell2) > kCharge_last) {
    charge_end();
  }
  if (ptype == WUKONG && spell3_active && (current_time - last_spell3) > kBerserk_last) {
    berserk_end();
  }

  //

  float render_passed_time = current_time - render_clock;
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

  float size = get_radius();
  if (ptype == WUKONG && spell3_active) {
    size *= kBerserk_enlarge;
  }
  if (render_player) {
    render_image(player_texture,
                 Point2f(rel_loc.x - size * scale, rel_loc.y - size * scale),
                 Point2f(rel_loc.x + size * scale, rel_loc.y + size * scale));
  }
  if (ptype == BAJIE) {
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
  } else if (ptype == WUKONG) {
    if (spell2_active) {
      render_image("charge_mode",
                   Point2f(rel_loc.x - size, (rel_loc.y - size - 50.0f)),
                   Point2f(rel_loc.x + size, (rel_loc.y + size)),
                   rad,
                   scale,
                   rel_loc,
                   false);
      if (is_charge_attacking()) {
        render_image("charge_attack",
             Point2f(rel_loc.x - size, rel_loc.y - size),
             Point2f(rel_loc.x + size, rel_loc.y + size),
             rad,
             scale,
             rel_loc,
             false);
      }
    }
    if (spell3_active) {
      render_image("berserk_mode",
             Point2f(rel_loc.x - size * scale, rel_loc.y - size * scale),
             Point2f(rel_loc.x + size * scale, rel_loc.y + size * scale));
    }
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
  float current_time = game_time->seconds();
  float passed_time = current_time - last_htime;
  if (passed_time > PLAYER_ATTACK_INTERVAL) {
    last_htime = current_time;
    switch (type) {
      case A1:
      case A2:
      case A3:
      case A4:
        try_normal_attack();

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

void Player::berserk() {
  spell3_active = true;
  //set_radius(get_radius() * kBerserk_enlarge);
  attack_buff = 2.0f;
}

void Player::berserk_end() {
  //set_radius(get_radius() / kBerserk_enlarge);
  attack_buff = kInit_buff;
  spell3_active = false;
}

void Player::cudgel_fury_begin() {
  if (render_player) {
    render_player = false;
    Spell* new_spell = new Cudgel_fury(get_location(),
                                       get_current_orientation(),
                                       get_radius(),
                                       this,
                                       game_time);
    Model_state::get_instance()->add_spell(new_spell);
  }
}
void Player::cudgel_fury_end() {
  render_player = true;
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

void Player::charge() {
  spell2_active = true;
  set_speed(kCharge_speed);
}

void Player::charge_update(float time) {
  static_move(time);
  float current_time = game_time->seconds();
  float passed_time = current_time - last_charge_attack;

  if (passed_time > kCharge_attack_CD) {
    last_charge_attack = current_time;
    for (std::vector<Monster *>::iterator it = monster_list_ptr->begin(); it != monster_list_ptr->end(); ++it) {
      if (get_body().intersects((*it)->get_body()) && (*it)->is_alive()) {
        (*it)->dec_health(kCharge_attack_damage);
        charge_attacking = true;
        break;
      }
    }
  }
}

void Player::charge_end() {
  //set_speed(get_current_speed() / kCharge_speed_multiplier);
  //charge_attacking = false;
  spell2_active = false;
}

void Player::try_normal_attack() {
  Spell* new_spell;
  if (ptype == SHASENG) {
    new_spell = new Arrow_attack(get_location(), get_current_orientation(), this);
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
  float current_time = game_time->seconds();
  float passed_time = current_time - last_spell1;
  Spell* new_spell;
  if (passed_time > spell1_CD) {
    last_spell1 = current_time;
    //create spell based on character type
    switch (ptype) {
      case SANZANG: //stopping
        break;
      case WUKONG: //Cudgel Fury
        cudgel_fury_begin();
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
        new_spell = new Healing_spell(get_location(), get_current_orientation());
        //new_spell = new Arrow_attack(get_location(), get_current_orientation());
        //new_spell = new Fire_ball(get_location(), get_current_orientation());
        Model_state::get_instance()->add_spell(new_spell);
        break;
      case WUKONG: //Charge
        charge();
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
        berserk();
        break;
      case SHASENG: //Magic Arrow
        new_spell = new Magic_arrow_ice(get_location(), get_current_orientation(), this);
        Model_state::get_instance()->add_spell(new_spell);
        break;
      default: // case BAJIE:  Life Trap
        bloodsuck();
        break;
    }
  }
}
