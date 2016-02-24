#include "../common/ids_component_types.hpp"
#include "../common/ids_object_tags.hpp"
#include "../common/ids_game_events.hpp"
#include "npc_controller.hpp"
#include "actor_model.hpp"
#include <core/interface/entity.hpp>
#include <core/physics/ray.hpp>
#include <core/transform/transform.hpp>
#include <renderer/renderer.hpp>


void
Npc_controller::on_update(const float dt)
{
  switch(m_state)
  {
    case(State::guard):
    {
      const Core::Transform transform = get_entity().get_transform();
      
      const math::vec3 from = transform.get_position();
      const math::vec3 to = math::vec3_add(from,
                                           math::vec3_scale(transform.get_forward(), m_sight_length));

      Renderer::debug_line(from, to, math::vec3_init(0,1,0));

      const Core::Physics::Ray search_ray(from, to, Core::Physics::Ray::Search::first);
      
      if(search_ray.has_hit())
      {
        if(search_ray.get_entity(0).has_tag(Tag::player))
        {
          
        }
      }
    }
    
    
    case(State::alerted):
    {
      
    }
  };
}


void
Npc_controller::on_event(const uint32_t id,
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