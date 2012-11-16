//
//  Game_state_sample.h
//  game
//
//  Created by Kyle Kong on 11/16/12.
//
//

#ifndef __game__Game_state_sample__
#define __game__Game_state_sample__

#include <iostream>
#include <zenilib.h>

namespace Flame {

class Game_state_sample : public Zeni::Gamestate_Base {

    Zeni::Point2f True_loc_player1, True_loc_player2,
    Rel_loc_player1, Rel_loc_player2;
    Zeni::Point2f Center_loc_player;
    bool Key_up, Key_down, Key_left, Key_right;
    Zeni::Chronometer<Zeni::Time> m_set;
    float m_time_passed;
    SDL_Joystick *joy;
    float scale;
    Zeni::Vector2f Move_dir1;
    
public:
    Game_state_sample();
private:
    void on_push();
    void on_pop();
    void on_cover();
    void on_uncover();
    void on_key(const SDL_KeyboardEvent &event);
    void on_joy_axis(const SDL_JoyAxisEvent &event);
    
    float get_scale(Zeni::Point2f &True_loc_layer1_, Zeni::Point2f &True_loc_player2_);
    Zeni::Point2f get_rel_loc(Zeni::Point2f &True_loc_);
    
    void perform_logic();
    void render();
};
    
}


#endif /* defined(__game__Game_state_sample__) */
