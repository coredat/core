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
          else if(key == SDLK_LSHIFT || key == SDLK_RSHIFT)
          {
            player_one->buttons[Core::Input::Button::button_2] = Core::Input::Button_state::down;
          }
          else if(key == SDLK_SPACE)
          {
            player_one->buttons[Core::Input::Button::button_3] = Core::Input::Button_state::down;
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
          else if(key == SDLK_LSHIFT || key == SDLK_RSHIFT)
          {
            player_one->buttons[Core::Input::Button::button_2] = Core::Input::Button_state::up;
          }
          else if(key == SDLK_SPACE)
          {
            player_one->buttons[Core::Input::Button::button_3] = Core::Input::Button_state::up;
          }
          
          break;
        }
        
        case(SDL_MOUSEMOTION):
          player_one->axis[1].x = static_cast<float>(evt.motion.xrel);
          player_one->axis[1].y = static_cast<float>(evt.motion.yrel);
          break;
          
        case(SDL_MOUSEBUTTONDOWN):
        {
          player_one->buttons[Core::Input::Button::button_0] = Core::Input::Button_state::down;
          break;
        }
        
        case(SDL_MOUSEBUTTONUP):
        {
          player_one->buttons[Core::Input::Button::button_0] = Core::Input::Button_state::up;
          break;
        }
      }
      
    } // While event
    
  }
}



} // ns