#include "network_player_controller.hpp"
#include "../common/ids_game_events.hpp"
#include "actor_model.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>


Network_player_controller::Network_player_controller()
{
  
}


Network_player_controller::~Network_player_controller()
{

}


void
Network_player_controller::on_start()
{
  m_camera.set_peer_priority(1, 1);
}


void
Network_player_controller::on_update(const float dt)
{
  const auto controller = Core::Input::get_controller(Core::Input::Player::two);
  
  // Actor Movement
  {
    Actor_model *actor = get_entity().get_component<Actor_model>(Component_type::actor);
    assert(actor);
    
    if(actor)
    {
      // Position
      float move_scale = 70;
      if(controller.is_button_down(Core::Input::Button::button_2))
      {
        move_scale *= 3;
      }
      
      move_scale *= dt;
      actor->move_forward(controller.get_axis(0).y * move_scale);
      actor->move_left(controller.get_axis(0).x * move_scale);
      
      // Head
      const float rot_scale = 0.5f * dt;
      actor->turn_left(controller.get_axis(1).x * rot_scale);
      actor->look_up(controller.get_axis(1).y * rot_scale);
    }
  }
}


void
Network_player_controller::on_event(const uint32_t id,
                                  const void *data,
                                  const size_t size_of_data)
{
  switch(id)
  {
    case(Game_event_id::got_shot):
    {
      Actor_model *actor = get_entity().get_component<Actor_model>(Component_type::actor);
      assert(actor);
      
      if(actor)
      {
        actor->take_damage();
      }
      
      break;
    }
  }
}