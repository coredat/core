#include "../common/ids_component_types.hpp"
#include "../common/ids_game_events.hpp"
#include "../common/ids_object_tags.hpp"
#include "../entity_factory.hpp"
#include "local_player_controller.hpp"
#include "actor.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
#include <core/physics/ray.hpp>
#include <systems/transform/transform.hpp>
#include <renderer/renderer.hpp>
#include <iostream>


Local_player_controller::Local_player_controller()
{
  
}


void
Local_player_controller::on_start()
{
  get_entity().add_tag(Tag::player);
  cube_id = Entity_factory::create_placement_cube(m_world_data).get_id();
}


void
Local_player_controller::on_update(const float dt)
{
  const auto controller = Core::Input::get_controller(Core::Input::Player::one);
  
  // Actor Movement
  {
    Actor *actor = get_entity().get_component<Actor>(Component_type::actor);
    assert(actor);
    
    // Position
    actor->move_forward(controller.get_axis(0).x);
    actor->move_left(controller.get_axis(0).y);
    
    // Head
    const float scale(0.5f);
    actor->turn_left(controller.get_axis(1).x * scale * dt);
    actor->look_up(controller.get_axis(1).y * scale * dt);
  }
  
  // Player Actions
  if(controller.is_button_down(Core::Input::Button::button_0))
  {
    const math::transform curr_trans = get_entity().get_transform();
    const math::vec3 curr_pos = curr_trans.position;
    
    math::vec3 fwd_vec;
    Transform::get_fwd_vec(&curr_trans, &fwd_vec);
    const math::vec3 scaled_fwd_vec = math::vec3_scale(fwd_vec, 3);
    
    Core::Ray shoot_ray(curr_pos, scaled_fwd_vec, Core::Ray::Search::first);
    
    if(shoot_ray.has_hit())
    {
      Core::Entity hit_ent = shoot_ray.get_entity(0);

      if(hit_ent.is_valid() && hit_ent.has_tag(Tag::actor))
      {
          hit_ent.send_event(Game_event_id::got_shot, nullptr, 0);
      }
    } // if face_ray hit
  } // If player input action
}


void
Local_player_controller::on_event(const uint32_t id,
                                  const void *data,
                                  const size_t size_of_data)
{
  switch(id)
  {
    case(Game_event_id::got_shot):
      // Get model apply damage.
      Actor *base = get_entity().get_component<Actor>(Component_type::actor);
      base->take_damage();
  }
}