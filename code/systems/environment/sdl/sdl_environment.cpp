#include "environment.hpp"
#include "sdl_types.hpp"
#include "sdl_common.hpp"


namespace Environment {


void
think(const Window *window,
      Input *input,
      const std::function<void(Environment::Event_id id)> &callback)
{
  Environment::Event_id id[128]; // This will need to hold data I think.
  std::size_t current_id(0);

  // Controller states
  {
    input->controllers[0].axis_1[0] = 0;
    input->controllers[0].axis_1[1] = 0;
    
    if(input->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_down)
    {
      input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::held;
    }
    
    if(input->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_up)
    {
      input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::resting;
    }
  }

  // Process events
  {
    SDL_Event evt;
    
    while (SDL_PollEvent(&evt))
    {
      switch(evt.type)
      {
        //** Application and Window **//
        case(SDL_QUIT):
          id[current_id++] = Environment::Event_id::window_close;
          break;
          
          
        //** Input **//
        case(SDL_KEYDOWN):
        {
          const SDL_Keycode key = evt.key.keysym.sym;
          
          if(key == SDLK_w)
          {
            input->controllers[0].axis_2[1] += 1;
          }
          else if(key == SDLK_s)
          {
            input->controllers[0].axis_2[1] -= 1;
          }
          else if(key == SDLK_a)
          {
            input->controllers[0].axis_2[0] += 1;
          }
          else if(key == SDLK_d)
          {
            input->controllers[0].axis_2[0] -= 1;
          }
          
          break;
        }
        case(SDL_KEYUP):
        {
          const SDL_Keycode key = evt.key.keysym.sym;
          
          if(key == SDLK_w)
          {
            input->controllers[0].axis_2[1] = 0;
          }
          else if(key == SDLK_s)
          {
            input->controllers[0].axis_2[1] = 0;
          }
          else if(key == SDLK_a)
          {
            input->controllers[0].axis_2[0] = 0;
          }
          else if(key == SDLK_d)
          {
            input->controllers[0].axis_2[0] = 0;
          }
          
          break;
        }
        
        case(SDL_MOUSEMOTION):
          input->controllers[0].axis_1[0] = static_cast<float>(evt.motion.xrel);
          input->controllers[0].axis_1[1] = static_cast<float>(evt.motion.yrel);
          break;
          
        case(SDL_MOUSEBUTTONDOWN):
        {
          if(input->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_down)
          {
            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::held;
          }
          else
          {
            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::on_down;
          }
          break;
        }
        
        case(SDL_MOUSEBUTTONUP):
        {
          if(input->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_up)
          {
            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::resting;
          }
          else
          {
            input->controllers[0].buttons[Environment::Button::action_button] = (uint8_t)Environment::Button_action::on_up;
          }
          break;
        }
      }
      
    }
  }
  
  // Normalize input
  {
    if(input->controllers[0].axis_2[0] != 0 && input->controllers[0].axis_2[1] != 0)
    {
      input->controllers[0].axis_2[0] *= 0.7071f;
      input->controllers[0].axis_2[1] *= 0.7071f;
    }
  }
  
  
  // Send out callbacks
  for(std::size_t i = 0; i < current_id; ++i)
  {
    callback(id[i]);
  }
}



} // ns