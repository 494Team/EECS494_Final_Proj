//
//  Game_state_sample.cpp
//  game
//
//  Created by Kyle Kong on 11/16/12.
//
//

#include "Game_state_sample.h"
#include "Control.h"
using namespace Flame;
using namespace Zeni;
using namespace std;

Game_state_sample::Game_state_sample()
    :True_loc_player1(400.0f, 300.0f),
    True_loc_player2(400.0f, 300.0f),
    Key_up(false),
    Key_down(false),
    Key_left(false),
    Key_right(false)
{
    m_set.start();
    set_pausable(true);
    
    // Open joystick;
    if (SDL_NumJoysticks() > 0)
        joy = SDL_JoystickOpen(0);
    
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
    
    if (Frac < 4.0f / 3.0f){
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

void Game_state_sample::on_joy_axis(const SDL_JoyAxisEvent &event){
    if (event.axis == 0)
        Move_dir1.x = event.value;
    else
        Move_dir1.y = event.value;
};

Point2f Game_state_sample::get_rel_loc(Zeni::Point2f &True_loc_){
    return (True_loc_ - Center_loc_player) * scale + Point2f(400.0f, 300.0f);
}

void Game_state_sample::perform_logic(){
    const float time_passed = m_set.seconds();
    const float time_step = time_passed - m_time_passed;
    m_time_passed = time_passed;

    /*This does not work;
    
     True_loc_player1 += Move_dir1 * time_step / 320;
    
     */
    
    True_loc_player1.x+=SDL_JoystickGetAxis(joy, 0)*time_step/320;
    True_loc_player1.y+=SDL_JoystickGetAxis(joy, 1)*time_step/320;
    
    True_loc_player2.x += (Key_right - Key_left) * time_step * 150;
    True_loc_player2.y += (Key_down - Key_up) * time_step * 150;
    
    Center_loc_player = Point2f((True_loc_player1.x + True_loc_player2.x) / 2, (True_loc_player1.y + True_loc_player2.y) / 2);
    
    scale = get_scale(True_loc_player1, True_loc_player2);
    
    Rel_loc_player1 = get_rel_loc(True_loc_player1);
    Rel_loc_player2 = get_rel_loc(True_loc_player2);
    
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
    
    render_image("one", Point2f(Rel_loc_player1.x-10.0f*scale,Rel_loc_player1.y-10.0f*scale), Point2f(Rel_loc_player1.x+10.0f*scale,Rel_loc_player1.y +10.0f*scale));
    
    render_image("two", Point2f(Rel_loc_player2.x-10.0f*scale,Rel_loc_player2.y-10.0f*scale), Point2f(Rel_loc_player2.x+10.0f*scale,Rel_loc_player2.y +10.0f*scale));
};