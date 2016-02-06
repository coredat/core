#include "../common/ids_component_types.hpp"
#include "../common/ids_game_events.hpp"
#include "../common/ids_object_tags.hpp"
#include "../entity_factory.hpp"
#include "local_player_controller.hpp"
#include "actor.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
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
  // Input
  // Movement axis
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
      math::transform curr_trans = get_entity().get_transform();
      
      math::vec3 fwd;
      Transform::get_fwd_vec(&curr_trans, &fwd);
      fwd = math::vec3_scale(fwd, 3);

      const btVector3 fwdVec(math::vec3_get_x(fwd), math::vec3_get_y(fwd), math::vec3_get_z(fwd));

      const btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));
      
      const btVector3 btFaceStart(btFrom);
      const btVector3 btFwd = btFaceStart + fwdVec * 3;
      
      btCollisionWorld::ClosestRayResultCallback face_ray(btFaceStart, btFwd);
      m_world_data->physics_world->dynamics_world.rayTest(btFaceStart, btFwd, face_ray);
      
      Renderer::debug_line(btFaceStart, btFwd, btVector3(1, 0, 0));

      if(face_ray.hasHit())
      {
        // Draw cube at that point.
        Core::Entity cube_entity;
        Data::world_find_entity(m_world_data, &cube_entity, cube_id);
        
        if(cube_entity.is_valid())
        {
          auto trans = cube_entity.get_transform();
          const math::vec3 pos = math::vec3_init(face_ray.m_hitPointWorld.x(),
                                                 face_ray.m_hitPointWorld.y(),
                                                 face_ray.m_hitPointWorld.z());
          trans.position = pos;
          cube_entity.set_transform(trans);
          
          // See if we can get the entity of the object
          const auto hit_obj = face_ray.m_collisionObject;
          const Core::Entity_id id = Core::Entity_id_util::convert_ptr_to_entity(hit_obj->getUserPointer());

          if(id != Core::Entity_id_util::invalid_id())
          {
            Core::Entity hit_ent;
          
            Data::world_find_entity(m_world_data, &hit_ent, id);
            
            if(hit_ent.is_valid())
            {
              std::cout << "is_valid" << std::endl;
              
              if(hit_ent.has_tag(Tag::actor))
              {
                std::cout << "has_actor_tag" << std::endl;
                Actor *actor = hit_ent.get_component<Actor>(Component_type::actor);
              
                if(actor)
                {
                  hit_ent.send_event(Game_event_id::got_shot, nullptr, 0);
                }
                else
                {
                  std::cout << "noo" << std::endl;
                }
              }
              else
              {
                std::cout << "no actor tag" << std::endl;
              }
            }
            else
            {
              std::cout << "is_not_valid" << std::endl;
              std::cout << "" << std::endl;
            }
          }
        }
      }
    }
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
      // Get model apply damage.
      Actor *base = get_entity().get_component<Actor>(Component_type::actor);
      base->take_damage();
  }
}