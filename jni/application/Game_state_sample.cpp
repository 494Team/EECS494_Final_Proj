//
//  Game_state_sample.cpp
//  game
//
//  Created by Kyle Kong on 11/16/12.
//
//

#include "Game_state_sample.h"

#include "Utility.h"
using namespace Zeni;
using namespace std;

namespace Flame {
    
Game_state_sample::Game_state_sample()
    :True_loc_player1(300.0f, 200.0f),
    True_loc_player2(500.0f, 300.0f),
	True_loc_player3(500.0f, 200.0f),
    True_loc_player4(300.0f, 300.0f),
    Key_up(false),
    Key_down(false),
    Key_left(false),
    Key_right(false),
    brick1(Point2f(0.0f, 610.0f), Vector2f(800.0f,10.0f), String("brick")),
	player1(Point2f(290.0f, 190.0f), Vector2f(20.0f, 20.0f)),
	rec1(Point2f(200.0f, 200.0f), Vector2f(50.0f, 50.0f), Point2f(200.0f, 200.0f), Vector2f(50.0f, 50.0f), String("monkey_king_front"))
{
    m_set.start();
    set_pausable(true);
	
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), 1);
    set_action(Zeni_Input_ID(SDL_JOYBUTTONDOWN, 7), 1);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X, 0), 2);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y, 0), 3);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X, 1), 4);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y, 1), 5);    
   
	set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X, 2), 6);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y, 2), 7);    
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_X, 3), 8);
    set_action(Zeni_Input_ID(SDL_JOYAXISMOTION, Joysticks::AXIS_LEFT_THUMB_Y, 3), 9);    
    
};

void Game_state_sample::update(Point2f &True_loc_player1,
											 Point2f &True_loc_player2,
											 Point2f &True_loc_player3,
											 Point2f &True_loc_player4){

	vector<Point2f> Player_list;
	map<float, pair<int,int> > dis_list;

	Player_list.push_back(True_loc_player1);
	Player_list.push_back(True_loc_player2);
	Player_list.push_back(True_loc_player3);
	Player_list.push_back(True_loc_player4);


	for(int i = 0; i < 4; ++i){
		for(int j = 1; j < 4; ++j)
		dis_list.insert(pair<float, pair<int,int>>(get_scale(Player_list[(i+j)%4], Player_list[i]), pair<int,int>(i,(j+i)%4)));
	}
	map<float,pair<int,int> >::iterator it = dis_list.begin();
	Center_loc_player = Point2f((True_loc_player1.x + True_loc_player2.x + True_loc_player3.x + True_loc_player4.x) / 4, 
								(True_loc_player1.y + True_loc_player2.y + True_loc_player3.y + True_loc_player4.y) / 4);

	scale = it->first;
}

void Game_state_sample::on_event(const Zeni_Input_ID &, const float &confidence, const int &action) {
   switch(action) {
	case 1:
        if(confidence == 1.0f) {
          Game &game = get_Game();
          game.push_state(new Popup_Menu_State);
        }
        break;

    case 2:
        Move_dir1.x = confidence;
        break;

    case 3:
        Move_dir1.y = confidence;
        break;

    case 4:
        Move_dir2.x = confidence;
		break;

    case 5:
        Move_dir2.y = confidence;
		break;
    
	case 6:
        Move_dir3.x = confidence;
		break;

    case 7:
        Move_dir3.y = confidence;
		break;
	
	case 8:
        Move_dir4.x = confidence;
		break;

    case 9:
        Move_dir4.y = confidence;
		break;
    
    default:
        break;
    }
};



void Game_state_sample::on_push(){
    Zeni::get_Game().joy_mouse.enabled = false;
    Zeni::get_Window().mouse_hide(true);
};

void Game_state_sample::on_pop(){
    Zeni::get_Game().joy_mouse.enabled = true;
    Zeni::get_Window().mouse_hide(false);
};

void Game_state_sample::on_cover(){
    Zeni::get_Game().joy_mouse.enabled = true;
    Zeni::get_Window().mouse_hide(false);
};

void Game_state_sample::on_uncover(){
    Zeni::get_Game().joy_mouse.enabled = false;
    Zeni::get_Window().mouse_hide(true);
};


float Game_state_sample::get_scale(Zeni::Point2f &True_loc_player1_, Zeni::Point2f &True_loc_player2_){
    float Dis_x = abs(True_loc_player1_.x - True_loc_player2_.x);
    float Dis_y = abs(True_loc_player1_.y - True_loc_player2_.y);
    float Frac = Dis_x / Dis_y;
    
    if (Dis_y != 0 &&Frac < 4.0f / 3.0f){
        if (200.0f / Dis_y < 1)
            return 1;
        else if (200.0f / Dis_y > 5)
            return 5;
        else
            return 200.0f / Dis_y;
    }
    else{
        if (800.0f/3/Dis_x < 1)
            return 1;
        else if (800.0f/3/Dis_x > 5)
            return 5;
        else
            return 800.0f/3/Dis_x;
    }
};

void Game_state_sample::on_key(const SDL_KeyboardEvent &event){
    
    switch (event.keysym.sym) {
        case SDLK_UP:
            Key_up = event.type == SDL_KEYDOWN;
            break;
        case SDLK_DOWN:
            Key_down = event.type == SDL_KEYDOWN;
            break;
        case SDLK_LEFT:
            Key_left = event.type == SDL_KEYDOWN;
            break;
        case SDLK_RIGHT:
            Key_right = event.type == SDL_KEYDOWN;
            break;
        default:
            Gamestate_Base::on_key(event);
            break;
            
    }
};

Point2f Game_state_sample::get_rel_loc(Zeni::Point2f &True_loc_){
    return (True_loc_ - Center_loc_player) * scale + Point2f(400.0f, 300.0f);
}

void Game_state_sample::perform_logic(){
    const float time_passed = m_set.seconds();
    const float time_step = time_passed - m_time_passed;
    m_time_passed = time_passed;
	
    
    True_loc_player1 += Move_dir1 * time_step * 20000.0f / 320;

	player1 = Collision_rectangle(Point2f(True_loc_player1 - Vector2f(10.0f, 10.0f)), Vector2f(20.0f, 20.0f));
	if (!brick1.can_move(player1) || !rec1.can_move(player1))
		True_loc_player1 -= Move_dir1 * time_step * 20000.0f /320;

    True_loc_player2 += -Move_dir1 * time_step * 20000.0f / 320;
    True_loc_player3 += Vector2f(Move_dir1.x, -Move_dir1.y) * time_step * 20000.0f / 320;
    True_loc_player4 += Move_dir4 * time_step * 20000.0f / 320;	

	update(True_loc_player1, True_loc_player2, True_loc_player3, True_loc_player4);

    Rel_loc_player1 = get_rel_loc(True_loc_player1);
    Rel_loc_player2 = get_rel_loc(True_loc_player2);
    Rel_loc_player3 = get_rel_loc(True_loc_player3);
    Rel_loc_player4 = get_rel_loc(True_loc_player4);

	brick1.update(scale, Center_loc_player);
	rec1.update(scale, Center_loc_player);
    SDL_Delay(5);
};

void Game_state_sample::render(){
    Video &vr = get_Video();
    get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
    
    vr.set_backface_culling(false);
    
    Quadrilateral<Vertex2f_Texture> map;
    Point2f Map_center(400.0f,300.0f);
	
    Point2f Map_p0 = (Point2f(0.0f, 0.0f) - Center_loc_player) * scale + Map_center;
    Point2f Map_p1 = (Point2f(0.0f, 600.0f) - Center_loc_player) * scale + Map_center;
    Point2f Map_p2 = (Point2f(800.0f, 600.0f) - Center_loc_player) * scale + Map_center;
    Point2f Map_p3 = (Point2f(800.0f, 0.0f) - Center_loc_player) * scale+ Map_center;
    Vertex2f_Texture p0(Map_p0, Point2f(0.0f,0.0f));
    Vertex2f_Texture p1(Map_p1, Point2f(0.0f, 30.0f));
    Vertex2f_Texture p2(Map_p2, Point2f(40.0f, 30.0f));
    Vertex2f_Texture p3(Map_p3, Point2f(40.0f, 0.0f));
    
    map[0] = p0;
    map[1] = p1;
    map[2] = p2;
    map[3] = p3;
    
    Material a("brick");
    map.fax_Material(&a);
    vr.render(map);
	brick1.render();
	rec1.render();
	
	//render_image("one", True_loc_player1 - Vector2f(10.f, 10.f), True_loc_player1 + Vector2f(10.f, 10.f));

    render_image("one", Point2f(Rel_loc_player1.x-10.0f*scale,Rel_loc_player1.y-10.0f*scale),
				Point2f(Rel_loc_player1.x+10.0f*scale,Rel_loc_player1.y +10.0f*scale));
    
    render_image("two", Point2f(Rel_loc_player2.x-10.0f*scale,Rel_loc_player2.y-10.0f*scale), 
				Point2f(Rel_loc_player2.x+10.0f*scale,Rel_loc_player2.y +10.0f*scale));

	render_image("three", Point2f(Rel_loc_player3.x-10.0f*scale,Rel_loc_player3.y-10.0f*scale),
				Point2f(Rel_loc_player3.x+10.0f*scale,Rel_loc_player3.y +10.0f*scale));
    
    render_image("four", Point2f(Rel_loc_player4.x-10.0f*scale,Rel_loc_player4.y-10.0f*scale), 
				Point2f(Rel_loc_player4.x+10.0f*scale,Rel_loc_player4.y +10.0f*scale));
};
    
}