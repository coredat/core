#include "../common/ids_component_types.hpp"
#include "../common/ids_game_events.hpp"
#include "../common/ids_object_tags.hpp"
#include "../entity_factory.hpp"
#include "local_player_controller.hpp"
#include "actor_model.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
#include <core/physics/ray.hpp>
#include <data/world_data/audio_pool.hpp>
#include <systems/transform/transformations.hpp>
#include <renderer/renderer.hpp>
#include <iostream>
#include <systems/audio/audio.hpp>


Local_player_controller::Local_player_controller()
{
  
}


Local_player_controller::~Local_player_controller()
{

}


void
Local_player_controller::on_start()
{
  get_entity().add_tag(Tag::player);
  gun = Entity_factory::create_gun(m_world_data);
}


void
Local_player_controller::on_update(const float dt)
{
  const auto controller = Core::Input::get_controller(Core::Input::Player::one);
  
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
  
  // Player Actions (This should be moved into a gun model.)
  if(controller.is_button_down(Core::Input::Button::button_0))
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
  } // If player input action
  
  // Move gun model
  {
    Core::Transform parent_trans = get_entity().get_transform();
    Core::Transform new_gun_trans = gun.get_transform();
    
    new_gun_trans.set_rotation(parent_trans.get_rotation());
    
    const math::vec3 fwd = math::vec3_add(parent_trans.get_position(), math::vec3_scale(parent_trans.get_forward(), 0.6f));
    const math::vec3 down = math::vec3_scale(parent_trans.get_up(), -0.2f);
    const math::vec3 left = math::vec3_scale(parent_trans.get_left(), 0.2f);
    
    const math::vec3 fwd_down = math::vec3_add(fwd, down);
    const math::vec3 fwd_down_left = math::vec3_add(fwd_down, left);
    
    new_gun_trans.set_position(fwd_down_left);
    
    new_gun_trans.set_scale(gun.get_transform().get_scale());

    gun.set_transform(new_gun_trans);
  }
}


void
Local_player_controller::on_event(const uint32_t id,
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
  
    case(Game_event_id::actor_died):
    {
      
      break;
    }
  }
}