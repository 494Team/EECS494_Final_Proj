#ifndef MINIMAP_H
#define MINIMAP_H

#include "Boss.h"
#include <zenilib.h>

namespace Flame {
  const Zeni::Point2f mini_ul = Zeni::Point2f(620.0f, 420.0f);
  const Zeni::Point2f mini_lr = Zeni::Point2f(770.0f, 570.0f);
  float mini_scale = 0.1f;

  bool get_mini_pos(const Zeni::Point2f &original_pos, Zeni::Point2f &mini_pos) {
    Zeni::Point2f mini_center = Zeni::Point2f((mini_ul.x + mini_lr.x) * 0.5f, (mini_ul.y + mini_lr.y) * 0.5f);
    Zeni::Point2f screen_center = Model_state::get_instance()->get_center_location();
    mini_pos = mini_center + (original_pos - screen_center) * mini_scale;
    if (mini_pos.x > mini_ul.x && mini_pos.x < mini_lr.x
        && mini_pos.y > mini_ul.y && mini_pos.y < mini_lr.y) {
      return true;
    } else {
      return false;
    }
  }

  void render_minimap() {
    // background
    render_image(
      "minimap",
      mini_ul,
      mini_lr,
      false,
      Color(0.5f, 1.0f, 1.0f, 1.0f)
    );

    // mini walls
    std::vector<Map *> maps = *(Model_state::get_instance()->get_map_obj_list_ptr());
    Zeni::Colors &cr = get_Colors();
    Video &vr = get_Video();
    for (int i = 0; i < (int)maps.size(); ++i) {
      Zeni::Point2f wall_pos;
      get_mini_pos(maps[i]->get_location(), wall_pos);
      Zeni::Point2f brick_ul = wall_pos;
      Zeni::Point2f brick_lr = brick_ul + maps[i]->get_size() * mini_scale;
      // clip
      if (brick_ul.x < mini_ul.x)
        brick_ul.x = mini_ul.x;
      if (brick_ul.y < mini_ul.y)
        brick_ul.y = mini_ul.y;
      if (brick_lr.x > mini_lr.x)
        brick_lr.x = mini_lr.x;
      if (brick_lr.y > mini_lr.y)
        brick_lr.y = mini_lr.y;
      if (brick_lr.x > brick_ul.x && brick_lr.y > brick_ul.y) {
        Zeni::Point2f brick_ur = Zeni::Point2f(brick_lr.x, brick_ul.y);
        Zeni::Point2f brick_ll = Zeni::Point2f(brick_ul.x, brick_lr.y);
        if (!maps[i]->walk_thru()) {
          Zeni::Vertex2f_Color ul_vertex = Zeni::Vertex2f_Color(brick_ul, cr["blue"]);
          Zeni::Vertex2f_Color lr_vertex = Zeni::Vertex2f_Color(brick_lr, cr["blue"]);
          Zeni::Vertex2f_Color ur_vertex = Zeni::Vertex2f_Color(brick_ur, cr["blue"]);
          Zeni::Vertex2f_Color ll_vertex = Zeni::Vertex2f_Color(brick_ll, cr["blue"]);
          Zeni::Quadrilateral<Zeni::Vertex2f_Color> quad(ul_vertex, ll_vertex, lr_vertex, ur_vertex);
          vr.render(quad);
        }
      }
    }

    // mini players
    std::vector<Player*> players = *(Model_state::get_instance()->get_player_list_ptr());
    Zeni::Point2f player0_pos, player1_pos, player2_pos, player3_pos;
    if (get_mini_pos(players[0]->get_location(), player0_pos)) {
      render_image(
        "mini_player0",
        player0_pos - Zeni::Vector2f(4.0f, 4.0f),
        player0_pos + Zeni::Vector2f(4.0f, 4.0f)
      );
    }
    if (Model_state::get_instance()->get_player_num() >= 2) {
        if (get_mini_pos(players[1]->get_location(), player1_pos)) {
          render_image(
            "mini_player1",
            player1_pos - Zeni::Vector2f(4.0f, 4.0f),
            player1_pos + Zeni::Vector2f(4.0f, 4.0f)
          );
        }
    }
    if (Model_state::get_instance()->get_player_num() >= 3) {
        if (get_mini_pos(players[2]->get_location(), player2_pos)) {
          render_image(
            "mini_player2",
            player2_pos - Zeni::Vector2f(4.0f, 4.0f),
            player2_pos + Zeni::Vector2f(4.0f, 4.0f)
          );
        }
    }
    if (Model_state::get_instance()->get_player_num() >= 4) {
        if (get_mini_pos(players[3]->get_location(), player3_pos)) {
          render_image(
            "mini_player3",
            player3_pos - Zeni::Vector2f(4.0f, 4.0f),
            player3_pos + Zeni::Vector2f(4.0f, 4.0f)
          );
        }
    }

    // mini_monsters
    std::vector<Monster *> monsters = *(Model_state::get_instance()->get_monster_list_ptr());
    for (int i = 0; i < (int)monsters.size(); ++i) {
      Zeni::Point2f monster_pos;
      if (get_mini_pos(monsters[i]->get_location(), monster_pos)) {
        Boss* boss_ptr = dynamic_cast<Boss*>(monsters[i]);
        if (boss_ptr == 0) {
          // normal monster
          render_image(
            "mini_monster",
            monster_pos - Zeni::Vector2f(2.0f, 2.0f),
            monster_pos + Zeni::Vector2f(2.0f, 2.0f)
          );
        } else {
          // boss
          render_image(
            "mini_boss",
            monster_pos - Zeni::Vector2f(6.0f, 6.0f),
            monster_pos + Zeni::Vector2f(6.0f, 6.0f)
          );
        }
      }
    }
  }

}

#endif