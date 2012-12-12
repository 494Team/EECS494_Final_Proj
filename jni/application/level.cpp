#include "level.h"

using namespace Flame;

void Level_1::init_map(int stage) {
  std::vector<Monster*> monster_list;
  std::vector<Map*> map_list;

  if (stage == 1) {
    // stage 1
    // monster
    monster_list.push_back(new Wanderer(Zeni::Point2f(100, 300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(150, 300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(150, 200)));
    monster_list.push_back(new Whisper(Zeni::Point2f(400, 800)));
    monster_list.push_back(new Whisper(Zeni::Point2f(450, 800)));
    monster_list.push_back(new Whisper(Zeni::Point2f(500, 800)));
    monster_list.push_back(new Whisper(Zeni::Point2f(550, 800)));
    // wall vertical
    map_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    map_list.push_back(new Map_brick(Point2f(1000.0f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    // wall horizontal
    map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    map_list.push_back(new Map_brick(Point2f(0.0f, 1000.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    // lava
    map_list.push_back(new Map_brick(Point2f(240.0f, 240.0f), Vector2f(40.f, 520.0f), 40.0f, 40.f, String("rock")));
    map_list.push_back(new Map_brick(Point2f(280.0f, 240.0f), Vector2f(440.f, 40.0f), 40.0f, 40.f, String("rock")));
    map_list.push_back(new Map_lava(Point2f(260.f, 260.f), Vector2f(480.f, 480.f), 80.f, 80.f));
    map_list.push_back(new Map_brick(Point2f(720.0f, 240.0f), Vector2f(40.f, 520.0f), 40.0f, 40.f, String("rock")));
    map_list.push_back(new Map_brick(Point2f(280.0f, 720.0f), Vector2f(440.f, 40.0f), 40.0f, 40.f, String("rock")));
    // laser
    map_list.push_back(new Map_laser(Point2f(3.f, 500.f), Vector2f(1.f, 0.f), "laser"));
    // transmission gates
    map_list.push_back(new Map_gate(Point2f(440.f, 60.f), Vector2f(120.f, 120.f), 2, "blue", true));
  }
  else if (stage == 2) {
    // stage 2
    // monster
    monster_list.push_back(new Wanderer(Zeni::Point2f(100, 500)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(150, 500)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(200, 500)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(250, 500)));
    monster_list.push_back(new Whisper(Zeni::Point2f(400, 500)));
    monster_list.push_back(new Whisper(Zeni::Point2f(450, 500)));
    monster_list.push_back(new Whisper(Zeni::Point2f(500, 500)));
    monster_list.push_back(new Whisper(Zeni::Point2f(550, 500)));

    // map
    // vertical
    map_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    map_list.push_back(new Map_brick(Point2f(1000.0f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    // horizontal
    map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    map_list.push_back(new Map_brick(Point2f(0.0f, 1000.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    // laser
    map_list.push_back(new Map_laser(Point2f(3.f, 500.f), Vector2f(-1.f, 0.f), "laser"));
    // transmission gates
    map_list.push_back(new Map_gate(Point2f(440.f, 60.f), Vector2f(120.f, 120.f), 3, "blue", true));
  }
  else {
    // stage 3
    // monster
    monster_list.push_back(new Redboy(Zeni::Point2f(500.f, 500.f)));

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
  }

  Level::set_monster_list(monster_list);
  Level::set_map_list(map_list);
}

void Level_2::init_map(int stage)
{
  std::vector<Monster*> monster_list;
  std::vector<Map*> map_list;

  if (stage == 1) {
    // stage 1
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

    // transmission gates
    map_list.push_back(new Map_gate(Point2f(1260.f, 60.f), Vector2f(120.f, 120.f), 2, "blue"));
    map_list.push_back(new Map_gate(Point2f(5820.f, 4620.f), Vector2f(120.f, 120.f), 3, "red"));
  }
  else if (stage == 2) {
    // stage 2
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

    // transmission gates
    map_list.push_back(new Map_gate(Point2f(4620.f, 60.f), Vector2f(120.f, 120.f), 3, "green"));
    map_list.push_back(new Map_gate(Point2f(1260.f, 5820.f), Vector2f(120.f, 120.f), 1, "blue"));
  }
  else if (stage == 3) {
    // stage 3
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

    // transmission gates
    map_list.push_back(new Map_gate(Point2f(60.f, 1260.f), Vector2f(120.f, 120.f), 1, "red"));
    map_list.push_back(new Map_gate(Point2f(5820.f, 1260.f), Vector2f(120.f, 120.f), 2, "green"));
    map_list.push_back(new Map_gate(Point2f(1260.f, 5820.f), Vector2f(120.f, 120.f), 4, "blue"));
  }
  else {
    // stage 4: bullking's room
    // monster
    monster_list.push_back(new Bullking(Zeni::Point2f(300.f, 300.f)));

    // map
    map_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(40.0f, 680.0f), 40.0f, 40.0f, "rock"));
    map_list.push_back(new Map_brick(Point2f(600.0f, -40.0f), Vector2f(40.0f, 680.0f), 40.0f, 40.0f, "rock"));
    map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(600.0f, 40.0f), 40.0f, 40.0f, "rock"));
    map_list.push_back(new Map_brick(Point2f(0.0f, 600.0f), Vector2f(600.0f, 40.0f), 40.0f, 40.0f, "rock"));
  }

  get_map_list().clear();
  get_monster_list().clear();
  Level::set_map_list(map_list);
  Level::set_monster_list(monster_list);
}
