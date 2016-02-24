#include "../common/ids_component_types.hpp"
#include "../common/ids_game_events.hpp"
#include "npc_actor_controller.hpp"
#include "actor_model.hpp"
#include <core/interface/entity.hpp>


void
Npc_actor_controller::on_update(const float dt)
{
}


void
Npc_actor_controller::on_event(const uint32_t id,
                                  const void *data,
                                  const uint32_t size_of_data)
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
    
    case(Game_event_id::actor_died):
    {
      get_entity().destroy();
      break;
    }
  }
}