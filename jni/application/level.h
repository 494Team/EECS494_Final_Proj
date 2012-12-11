#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Player.h"
#include "Wanderer.h"
#include "Whisper.h"
#include "Redboy.h"
#include "Bullking.h"
#include "Map.h"

namespace Flame {
  class Level {
  public:
    std::vector<Monster*> get_monster_list(int stage) { return monster_list[stage]; }
    std::vector<Map*> get_map_list(int stage) { return map_list[stage]; }
    Zeni::Point2f get_upper_left(int stage) { return ul[stage]; }
    Zeni::Point2f get_lower_right(int stage) { return lr[stage]; }
  protected:
    void set_monster_list(int stage, const std::vector<Monster*> &monster_list_) {
      monster_list[stage] = monster_list_;
    }
    void set_map_list(int stage, const std::vector<Map*> &map_list_) {
      map_list[stage] = map_list_;
    }
    void set_upper_left(int stage, const Zeni::Point2f &ul_) { ul[stage] = ul_; }
    void set_lower_right(int stage, const Zeni::Point2f &lr_) { lr[stage] = lr_; }
  private:
    std::map<int, std::vector<Monster*>> monster_list;
    std::map<int, std::vector<Map*>> map_list;
    std::map<int, Zeni::Point2f> ul;
    std::map<int, Zeni::Point2f> lr;
  };

  class Level_1 : public Level {
  public:
    Level_1();
  };

}

#endif LEVEL_H