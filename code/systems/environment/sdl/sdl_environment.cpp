#include "../environment.hpp"
#include "sdl_types.hpp"
#include "sdl_common.hpp"


namespace Environment {


void
think(const Window *window,
      Core_data::Input_pool *input_pool)
{
  // Process events
  {
    SDL_Event evt;
    Core_data::Game_controller *player_one = Core_data::input_data_get_controller(input_pool, 0);
    player_one->axis[1].x = 0;
    player_one->axis[1].y = 0;
    
    while (SDL_PollEvent(&evt))
    {
      switch(evt.type)
      {
        //** Application and Window **//
        case(SDL_QUIT):
          break;
          
          
        //** Input **//
        case(SDL_KEYDOWN):
        {
          const SDL_Keycode key = evt.key.keysym.sym;
          
          // Input directions are based of a normal cartesian grid.
          if(key == SDLK_w)
          {
            player_one->axis[0].y = +1;
          }
          else if(key == SDLK_s)
          {
            player_one->axis[0].y = -1;
          }
          else if(key == SDLK_a)
          {
            player_one->axis[0].x = -1;
          }
          else if(key == SDLK_d)
          {
            player_one->axis[0].x = +1;
          }
          
          break;
        }
        case(SDL_KEYUP):
        {
          const SDL_Keycode key = evt.key.keysym.sym;
          
          if(key == SDLK_w)
          {
            player_one->axis[0].y = 0;
          }
          else if(key == SDLK_s)
          {
            player_one->axis[0].y = 0;
          }
          else if(key == SDLK_a)
          {
            player_one->axis[0].x = 0;
          }
          else if(key == SDLK_d)
          {
            player_one->axis[0].x = 0;
          }
          
          break;
        }
        
        case(SDL_MOUSEMOTION):
          player_one->axis[1].x = static_cast<float>(evt.motion.xrel);
          player_one->axis[1].y = static_cast<float>(evt.motion.yrel);
          break;
          
        case(SDL_MOUSEBUTTONDOWN):
        {
//          if(input->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_down)
//          {
//            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::held;
//          }
//          else
//          {
//            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::on_down;
//          }
//          break;
        }
        
        case(SDL_MOUSEBUTTONUP):
        {
//          if(input->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_up)
//          {
//            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::resting;
//          }
//          else
//          {
//            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::on_up;
//          }
//          break;
        }
      }
      
    } // While event
    
  }
  
  // Normalize input
  {
//    if(input->controllers[0].axis_2[0] != 0 && input->controllers[0].axis_2[1] != 0)
//    {
//    // TODO: Normalise this.
////      input->controllers[0].axis_2[0] *= 0.7071f;
////      input->controllers[0].axis_2[1] *= 0.7071f;
//    }
  }
}



} // ns