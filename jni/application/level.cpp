#include "level.h"

using namespace Flame;

void Level_1::init_map(int stage) {
  std::vector<Monster*> monster_list;
  std::vector<Map*> map_list;

  if (stage == 1) {
    // stage 1
    // monster
    get_Sound().set_BGM("sfx/stage_bgm");
    monster_list.push_back(new Wanderer(Zeni::Point2f(75, 150)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 150)));
    monster_list.push_back(new Whisper(Zeni::Point2f(100, 100)));

    monster_list.push_back(new Wanderer(Zeni::Point2f(875, 150)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(925, 150)));
    monster_list.push_back(new Whisper(Zeni::Point2f(900, 100)));

    monster_list.push_back(new Wanderer(Zeni::Point2f(475, 850)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(525, 850)));
    monster_list.push_back(new Whisper(Zeni::Point2f(500, 800)));
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
    map_list.push_back(new Map_laser(Point2f(997.f, 500.f), Vector2f(-1.f, 0.f), "laser"));
    // transmission gates
    map_list.push_back(new Map_gate(Point2f(440.f, 60.f), Vector2f(120.f, 120.f), 2, "blue", true));
  }
  else if (stage == 2) {
    // stage 2
    // monster
    get_Sound().set_BGM("sfx/stage_bgm");
    monster_list.push_back(new Wanderer(Zeni::Point2f(475, 500)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(525, 500)));
    monster_list.push_back(new Whisper(Zeni::Point2f(500, 450)));

    monster_list.push_back(new Wanderer(Zeni::Point2f(75, 150)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 150)));
    monster_list.push_back(new Whisper(Zeni::Point2f(75, 100)));
    monster_list.push_back(new Whisper(Zeni::Point2f(125, 100)));

    monster_list.push_back(new Wanderer(Zeni::Point2f(875, 150)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(925, 150)));
    monster_list.push_back(new Whisper(Zeni::Point2f(875, 100)));
    monster_list.push_back(new Whisper(Zeni::Point2f(925, 100)));

    monster_list.push_back(new Whisper(Zeni::Point2f(75, 900)));
    monster_list.push_back(new Whisper(Zeni::Point2f(75, 850)));

    monster_list.push_back(new Whisper(Zeni::Point2f(925, 900)));
    monster_list.push_back(new Whisper(Zeni::Point2f(925, 850)));

    // map
    // vertical
    map_list.push_back(new Map_brick(Point2f(-40.0f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    map_list.push_back(new Map_brick(Point2f(1000.0f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    // horizontal
    map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    map_list.push_back(new Map_brick(Point2f(0.0f, 1000.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    // laser
    map_list.push_back(new Map_laser(Point2f(3.f, 300.f), Vector2f(1.f, 0.f), "laser"));
    map_list.push_back(new Map_laser(Point2f(997.f, 600.f), Vector2f(-1.f, 0.f), "laser"));
    // transmission gates
    map_list.push_back(new Map_gate(Point2f(440.f, 60.f), Vector2f(120.f, 120.f), 3, "red"));
  }
  else {
    // stage 3
    // monster
    get_Sound().set_BGM("sfx/boss_bgm");
    monster_list.push_back(new Redboy(Zeni::Point2f(500.f, 500.f)));

    // map
    // vertical
    map_list.push_back(new Map_brick(Point2f(-40.f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    map_list.push_back(new Map_brick(Point2f(1000.f, -40.0f), Vector2f(40.0f, 1080.0f), 40.0f, 40.0f, "rock"));
    // horizontal
    map_list.push_back(new Map_brick(Point2f(0.0f, -40.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    map_list.push_back(new Map_brick(Point2f(0.0f, 1000.0f), Vector2f(1000.f, 40.0f), 40.0f, 40.f, String("rock")));
    // transmission gate
    //map_list.push_back(new Map_transmission_gate(Point2f(800.0f,200.0f), Point2f(200.f, 500.f), Vector2f(40.f, 40.f), "door"));
    //map_list.push_back(new Map_transmission_gate(Point2f(200.0f,550.0f), Point2f(800.f, 350.f), Vector2f(40.f, 40.f), "door"));
    // laser
    //map_list.push_back(new Map_light_beam(Point2f(3.0f, 100.0f), Vector2f(1.0f, 0.0f), 0, "light_beam"));
    map_list.push_back(new Map_laser(Point2f(3.f, 250.f), Vector2f(1.f, 0.f), "laser"));
    map_list.push_back(new Map_laser(Point2f(997.f, 750.f), Vector2f(-1.f, 0.f), "laser"));
    map_list.push_back(new Map_laser(Point2f(250.f, 3.f), Vector2f(0.f, 1.f), "laser"));
    map_list.push_back(new Map_laser(Point2f(750.f, 997.f), Vector2f(0.f, -1.f), "laser"));
  }

  Level::set_monster_list(monster_list);
  Level::set_map_list(map_list);
}

void Level_2::init_map(int stage)
{
  std::vector<Monster*> monster_list;
  std::vector<Map*> map_list;

  if (stage == 1) {
    get_Sound().set_BGM("sfx/stage_bgm");
    // stage 1
    // monster
    // left up
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 1300)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 1250)));
    // left up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 2100)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 2050)));
    // left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 2925)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(95, 2925)));
    // left down
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 4700)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 4750)));
    // left down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 3900)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 3950)));

    // middle left up
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 1300)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 1250)));
    // middle left up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 2100)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 2050)));
    // middle left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1325, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1295, 2925)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1295, 3075)));
    // middle left down
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 4700)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 4750)));
    // middle left down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 3900)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 3950)));

    // middle up
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 1300)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 1250)));
    // middle up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 2100)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 2050)));
    // middle middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3005, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3005, 2925)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2975, 2925)));
    // middle down
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 4700)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 4750)));
    // middle down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 3900)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 3950)));

    // middle right up
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 1300)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 1250)));
    // middle right up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 2100)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 2050)));
    // middle right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4655, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4655, 2925)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4685, 2925)));
    // middle right down
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 4700)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 4750)));
    // middle right down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 3900)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 3950)));

    // right up
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 1300)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 1250)));
    // right up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 2100)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 2050)));
    // right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5855, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5855, 2925)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5885, 2925)));
    // right down
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 4700)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 4750)));
    // right down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 3900)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 3950)));

    // up left
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 195)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1250, 195)));
    // up left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 195)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 195)));
    // up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 125)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 125)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2925, 95)));
    // up right
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 195)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4750, 195)));
    // up right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 195)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 195)));

    // down left
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 5805)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1250, 5805)));
    // down left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 5805)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 5805)));
    // down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 5875)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 5875)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2925, 5905)));
    // down right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 5805)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4750, 5805)));
    // down right
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 5805)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 5805)));

    // left middle middle left - middle up
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 1250)));
    // left middle middle left - middle down
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 4750)));

    // right middle middle right - middle up
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 1250)));
    // right middle middle right - middle down
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 4750)));

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

    // laser
    map_list.push_back(new Map_laser(Point2f(1200.f, 2763.f), Vector2f(0.f, 1.f), "laser"));
    map_list.push_back(new Map_laser(Point2f(2763.f, 4800.f), Vector2f(1.f, 0.f), "laser"));
    map_list.push_back(new Map_laser(Point2f(4800.f, 3237.f), Vector2f(0.f, -1.f), "laser"));
    map_list.push_back(new Map_laser(Point2f(3237.f, 1200.f), Vector2f(-1.f, 0.f), "laser"));
  }
  else if (stage == 2) {
    get_Sound().set_BGM("sfx/stage_bgm");
    // stage 2
    // monster
    // left up
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 1250)));
    // left up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 2050)));
    // left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(95, 3075)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(95, 2925)));
    // left down
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 4750)));
    // left down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 3950)));

    // middle left up
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 1250)));
    // middle left up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 2050)));
    // middle left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1325, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1295, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1325, 2925)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1295, 3075)));
    // middle left down
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 4750)));
    // middle left down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 3950)));

    // middle up
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2925, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 1250)));
    // middle up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2935, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 2050)));
    // middle middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3005, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3005, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2975, 3075)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2975, 2925)));
    // middle down
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2925, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 4750)));
    // middle down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2925, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 3950)));

    // middle right up
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 1250)));
    // middle right up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 2050)));
    // middle right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4655, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4655, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4685, 3075)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4685, 2925)));
    // middle right down
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 4750)));
    // middle right down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 3950)));

    // right up
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 1250)));
    // right up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 2050)));
    // right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5855, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5855, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5885, 3075)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5885, 2925)));
    // right down
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 4750)));
    // right down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 3950)));

    // up left
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1250, 45)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1250, 195)));
    // up left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2050, 45)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 195)));
    // up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 125)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 125)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3075, 95)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2925, 95)));
    // up right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4750, 45)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4750, 195)));
    // up right
    /*
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 45)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 195)));
    */

    // down left
    /*
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1250, 5955)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1250, 5805)));
    */
    // down left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2050, 5955)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 5805)));
    // down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 5875)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 5875)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3075, 5905)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2925, 5905)));
    // down right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4750, 5955)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4750, 5805)));
    // down right
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 5955)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 5805)));

    // right middle middle right - middle up
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 1250)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 1300)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 1250)));
    // right middle middle right - middle down
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 4750)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 4700)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 4750)));

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
    get_Sound().set_BGM("sfx/stage_bgm");
    // stage 3
    // monster
    // left up
    /*
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 1250)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(120, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 1250)));
    */
    // left up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 2050)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(120, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 2050)));
    // left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(125, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(95, 3075)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(95, 3000)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(95, 2925)));
    // left down
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 4750)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(120, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 4750)));
    // left down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(45, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(195, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(45, 3950)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(120, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(195, 3950)));

    // middle left up
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 1250)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1320, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 1250)));
    // middle left up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 2050)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1320, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 2050)));
    // middle left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1325, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1325, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1295, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1295, 3000)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1295, 3075)));
    // middle left down
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 4750)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1320, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 4750)));
    // middle left down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(1245, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1395, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1245, 3950)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1320, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1395, 3950)));

    // middle up
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2925, 1250)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3000, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 1250)));
    // middle up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2925, 2050)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3000, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 2050)));
    // middle middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3005, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3005, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2975, 3075)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3000, 3000)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2975, 2925)));
    // middle down
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2925, 4750)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3000, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 4750)));
    // middle down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2925, 3950)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3000, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3075, 3950)));

    // middle right up
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 1250)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4680, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 1250)));
    // middle right up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 2050)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4680, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 2050)));
    // middle right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4655, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4655, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4685, 3075)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4685, 3000)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4685, 2925)));
    // middle right down
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 4750)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4680, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 4750)));
    // middle right down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4605, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4755, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4605, 3950)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4680, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4755, 3950)));

    // right up
    /*
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 1250)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5880, 1250)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 1250)));
    */
    // right up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 2100)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 2100)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 2050)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5880, 2050)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 2050)));
    // right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5855, 3075)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5855, 2925)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5885, 3075)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5885, 3000)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5885, 2925)));
    // right down
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 4750)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5880, 4750)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 4750)));
    // right down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(5805, 3900)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(5955, 3900)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5805, 3950)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(5880, 3950)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(5955, 3950)));

    // up left
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1250, 45)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1250, 120)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1250, 195)));
    // up left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2050, 45)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2050, 120)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 195)));
    // up middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 125)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 125)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3075, 95)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3000, 95)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2925, 95)));
    // up right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4750, 45)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4750, 120)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4750, 195)));
    // up right
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 45)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 195)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 45)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 120)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 195)));

    // down left
    /*
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(1300, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1250, 5955)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(1250, 5880)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(1250, 5805)));
    */
    // down left middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2100, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2050, 5955)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(2050, 5880)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2050, 5805)));
    // down middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(3075, 5875)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(2925, 5875)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3075, 5905)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3000, 5905)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(2925, 5905)));
    // down right middle
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(4700, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4750, 5955)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(4750, 5880)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(4750, 5805)));
    // down right
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 5955)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 5805)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 5955)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 5880)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 5805)));

    // right middle middle right - middle up
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 1300)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 1250)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 1300)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 1275)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 1250)));
    // right middle middle right - middle down
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 4700)));
    monster_list.push_back(new Wanderer(Zeni::Point2f(3900, 4750)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 4700)));
    monster_list.push_back(new Whisper_green(Zeni::Point2f(3950, 4725)));
    monster_list.push_back(new Whisper_violet(Zeni::Point2f(3950, 4750)));

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
    map_list.push_back(new Map_gate(Point2f(1260.f, 5820.f), Vector2f(120.f, 120.f), 4, "boss"));
  }
  else {
    get_Sound().set_BGM("sfx/boss_bgm");
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
