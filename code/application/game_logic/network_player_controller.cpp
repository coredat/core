#include "network_player_controller.hpp"
#include "../common/ids_game_events.hpp"
#include "actor_model.hpp"
#include <core/interface/entity.hpp>


Network_player_controller::Network_player_controller()
{
  
}


Network_player_controller::~Network_player_controller()
{

}


void
Network_player_controller::on_start()
{
}


void
Network_player_controller::on_update(const float dt)
{
  
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