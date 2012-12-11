#include "level.h"

using namespace Flame;

Level_1::Level_1() {
  int stage;
  std::vector<Monster*> monster_list;
  std::vector<Map*> map_list;
  // stage 1
  stage = 1;

  // monster
  monster_list.push_back(new Wanderer(Zeni::Point2f(100, 2300)));
  monster_list.push_back(new Wanderer(Zeni::Point2f(150, 2300)));
  monster_list.push_back(new Wanderer(Zeni::Point2f(250, 2300)));
  monster_list.push_back(new Whisper(Zeni::Point2f(400, 2800)));
  monster_list.push_back(new Whisper(Zeni::Point2f(450, 2800)));
  monster_list.push_back(new Whisper(Zeni::Point2f(500, 2800)));
  monster_list.push_back(new Whisper(Zeni::Point2f(550, 2800)));
  // wall vertical
  map_list.push_back(new Map_brick(Point2f(-40.0f, 2040.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(1000.0f, 2040.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
  // wall horizontal
  map_list.push_back(new Map_brick(Point2f(0.0f, 2040.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
  map_list.push_back(new Map_brick(Point2f(0.0f, 3080.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
  // lava
  map_list.push_back(new Map_brick(Point2f(260.0f, 2440.0f), Vector2f(40.f, 280.0f), 40.0f, 40.f, String("rock")));
  map_list.push_back(new Map_brick(Point2f(300.0f, 2440.0f), Vector2f(400.f, 40.0f), 40.0f, 40.f, String("rock")));
  map_list.push_back(new Map_lava(Point2f(280.f, 2460.f), Vector2f(440.f, 240.f), 80.f, 80.f));
  map_list.push_back(new Map_brick(Point2f(700.0f, 2440.0f), Vector2f(40.f, 280.0f), 40.0f, 40.f, String("rock")));
  map_list.push_back(new Map_brick(Point2f(300.0f, 2680.0f), Vector2f(400.f, 40.0f), 40.0f, 40.f, String("rock")));
  // laser
  map_list.push_back(new Map_laser(Point2f(3.f, 2500.f), Vector2f(1.f, 0.f), "laser"));
  
  Level::set_monster_list(stage, monster_list);
  Level::set_map_list(stage, map_list);
  Level::set_upper_left(stage, Zeni::Point2f(-20.0f, 2060.0f));
  Level::set_lower_right(stage, Zeni::Point2f(1020.0f, 3100.0f));
  
  // stage 2
  stage = 2;
  monster_list.clear();
  map_list.clear();

  // monster
  monster_list.push_back(new Wanderer(Zeni::Point2f(100, 1500)));
  monster_list.push_back(new Wanderer(Zeni::Point2f(150, 1500)));
  monster_list.push_back(new Wanderer(Zeni::Point2f(200, 1500)));
  monster_list.push_back(new Wanderer(Zeni::Point2f(250, 1500)));
  monster_list.push_back(new Whisper(Zeni::Point2f(400, 1500)));
  monster_list.push_back(new Whisper(Zeni::Point2f(450, 1500)));
  monster_list.push_back(new Whisper(Zeni::Point2f(500, 1500)));
  monster_list.push_back(new Whisper(Zeni::Point2f(550, 1500)));

  // map
  // vertical
  map_list.push_back(new Map_brick(Point2f(-40.0f, 1000.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(1000.0f, 1000.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
  // horizontal
  map_list.push_back(new Map_brick(Point2f(0.0f, 1000.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
  map_list.push_back(new Map_brick(Point2f(0.0f, 2040.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
  // laser
  map_list.push_back(new Map_laser(Point2f(997.f, 1500.f), Vector2f(-1.f, 0.f), "laser"));

  Level::set_monster_list(stage, monster_list);
  Level::set_map_list(stage, map_list);
  Level::set_upper_left(stage, Zeni::Point2f(-20.0f, 1020.0f));
  Level::set_lower_right(stage, Zeni::Point2f(1020.0f, 2060.0f));
  
  // stage 3
  stage = 3;
  monster_list.clear();
  map_list.clear();

  // monster
  monster_list.push_back(new Redboy(Zeni::Point2f(300, 300)));
  monster_list.push_back(new Whisper(Zeni::Point2f(400, 500)));

  // map
  // vertical
  map_list.push_back(new Map_brick(Point2f(-40.f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(1000.f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
  // horizontal
  map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
  map_list.push_back(new Map_brick(Point2f(0.0f, 1000.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
  // transmission gate
  map_list.push_back(new Map_transmission_gate(Point2f(800.0f,200.0f), Point2f(200.f, 500.f), Vector2f(40.f, 40.f), "door"));
  map_list.push_back(new Map_transmission_gate(Point2f(200.0f,550.0f), Point2f(800.f, 350.f), Vector2f(40.f, 40.f), "door"));
  // laser
  map_list.push_back(new Map_light_beam(Point2f(3.0f, 100.0f), Vector2f(1.0f, 0.0f), 0, "light_beam"));
  map_list.push_back(new Map_laser(Point2f(3.f, 500.f), Vector2f(1.f, 0.f), "laser"));
  map_list.push_back(new Map_laser(Point2f(200.f, 3.f), Vector2f(0.f, 1.f), "laser"));
  map_list.push_back(new Map_laser(Point2f(800.f, 3.f), Vector2f(0.f, 1.f), "laser"));

  Level::set_monster_list(stage, monster_list);
  Level::set_map_list(stage, map_list);
  Level::set_upper_left(stage, Zeni::Point2f(-20.0f, -20.0f));
  Level::set_lower_right(stage, Zeni::Point2f(1020.0f, 1020.0f));
}

Level_2::Level_2() {
  int stage;
  std::vector<Monster*> monster_list;
  std::vector<Map*> map_list;

  // stage 1
  stage = 1;
  // map
  map_list.push_back(new Map_brick(Point2f(0.0f, 0.0f), Vector2f(1200.0f, 1200.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(240.0f, 1200.0f), Vector2f(960.0f, 1560.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(1200.0f, 240.0f), Vector2f(1560.0f, 960.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(1440.0f, 1440.0f), Vector2f(1320.0f, 1320.0f), 40.0f, 40.0f, "rock"));

  map_list.push_back(new Map_brick(Point2f(4800.0f, 0.0f), Vector2f(1200.0f, 1200.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(4800.0f, 1200.0f), Vector2f(960.0f, 1560.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(3240.0f, 240.0f), Vector2f(1560.0f, 960.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(3240.0f, 1440.0f), Vector2f(1320.0f, 1320.0f), 40.0f, 40.0f, "rock"));

  map_list.push_back(new Map_brick(Point2f(0.0f, 4800.0f), Vector2f(1200.0f, 1200.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(240.0f, 3240.0f), Vector2f(960.0f, 1560.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(1200.0f, 4800.0f), Vector2f(1560.0f, 960.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(1440.0f, 3240.0f), Vector2f(1320.0f, 1320.0f), 40.0f, 40.0f, "rock"));

  map_list.push_back(new Map_brick(Point2f(4800.0f, 4800.0f), Vector2f(1200.0f, 1200.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(4800.0f, 3240.0f), Vector2f(960.0f, 1560.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(3240.0f, 4800.0f), Vector2f(1560.0f, 960.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(3240.0f, 3240.0f), Vector2f(1320.0f, 1320.0f), 40.0f, 40.0f, "rock"));
 
  map_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(40.0f, 6080.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(6000.0f, -40.0f), Vector2f(40.0f, 6080.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(6000.0f, 40.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(0.0f, 6000.0f), Vector2f(6000.0f, 40.0f), 40.0f, 40.0f, "rock"));

  Level::set_map_list(stage, map_list);
  Level::set_monster_list(stage, monster_list);
  Level::set_upper_left(stage, Zeni::Point2f(-20.0f, -20.0f));
  Level::set_lower_right(stage, Zeni::Point2f(6020.0f, 6020.0f));

  // stage 2
  stage = 2;
  monster_list.clear();

  Level::set_map_list(stage, map_list);
  Level::set_monster_list(stage, monster_list);
  Level::set_upper_left(stage, Zeni::Point2f(-20.0f, -20.0f));
  Level::set_lower_right(stage, Zeni::Point2f(6020.0f, 6020.0f));

  // stage 3
  stage = 3;
  monster_list.clear();

  Level::set_map_list(stage, map_list);
  Level::set_monster_list(stage, monster_list);
  Level::set_upper_left(stage, Zeni::Point2f(-20.0f, -20.0f));
  Level::set_lower_right(stage, Zeni::Point2f(6020.0f, 6020.0f));

  // stage 4: bullking's room
  stage = 4;
  map_list.clear();
  monster_list.clear();
  Point2f room_ul(0.0f, 0.0f), room_lr(600.0f, 600.0f);
  // map
  map_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(40.0f, 680.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(600.0f, -40.0f), Vector2f(40.0f, 680.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(600.0f, 40.0f), 40.0f, 40.0f, "rock"));
  map_list.push_back(new Map_brick(Point2f(0.0f, 600.0f), Vector2f(600.0f, 40.0f), 40.0f, 40.0f, "rock"));

  Level::set_map_list(stage, map_list);
  Level::set_monster_list(stage, monster_list);
  Level::set_upper_left(stage, Point2f(-20.0f, -20.0f));
  Level::set_lower_right(stage, Point2f(620.0f, 620.0f));
}