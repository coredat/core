#include <application/game_logic/gun_controller.hpp>
#include <application/game_logic/gun_model.hpp>
#include <application/common/ids_game_events.hpp>
#include <application/common/ids_object_tags.hpp>
#include <core/transform/transform.hpp>
#include <core/interface/entity.hpp>
#include <core/physics/ray.hpp>
#include <data/world_data/audio_pool.hpp>
#include <systems/audio/audio.hpp>


void
Gun_controller::on_update(const float dt)
{

}


void
Gun_controller::on_event(const uint32_t id, const void *data, const uint32_t size_of_data)
{
  switch(id)
  {
    case(Game_event_id::gun_fire):
    {
      Gun_model *model = get_entity().get_component<Gun_model>(Component_type::gun);
      
      if(model && model->fire_gun())
      {
        fire_gun();
      }
      
      break;
    }
  }
}


void
Gun_controller::fire_gun()
{
  Audio::Node_sample_2d node;
  node.chunk_to_play = m_world_data->audio_pool->samples[0].chunk;
  
  Audio::play_nodes(&node, 1);

  const Core::Transform curr_trans = get_entity().get_transform();
  
  const math::vec3 scaled_fwd_vec = math::vec3_scale(curr_trans.get_forward(), 1000.f);
  
  const math::vec3 from = curr_trans.get_position();
  const math::vec3 to   = math::vec3_add(from, scaled_fwd_vec);
  
  Core::Physics::Ray shoot_ray(from, to, Core::Physics::Ray::Search::first);
  
  if(shoot_ray.has_hit())
  {
    Core::Entity hit_ent = shoot_ray.get_entity(0);

    if(hit_ent.is_valid() && hit_ent.has_tag(Tag::actor))
    {
      hit_ent.send_event(Game_event_id::got_shot, nullptr, 0);
    }
  } // if face_ray hit
}


void
Gun_controller::reload()
{

}
