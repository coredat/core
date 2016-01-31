#include "actor_local_player.hpp"
#include <renderer/renderer.hpp>
#include <btBulletCollisionCommon.h>
#include <systems/transform/transform.hpp>
#include <application/entity_factory.hpp>
#include <data/data.hpp>
#include <core/interface/entity.hpp>


Actor_local_player::Actor_local_player()
{
}


void
Actor_local_player::on_start()
{
 cube_id = Entity_factory::create_placement_cube(m_world_data).get_id();
}


void
Actor_local_player::on_update(const float dt)
{
  const math::transform curr_trans = get_entity().get_transform();

  math::vec3 fwd;
  Transform::get_fwd_vec(&curr_trans, &fwd);
  fwd = math::vec3_scale(fwd, 3);

  const btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));

  const btVector3 fwdVec(math::vec3_get_x(fwd), math::vec3_get_y(fwd), math::vec3_get_z(fwd));
  const btVector3 btFaceStart(btFrom);
  const btVector3 btFwd = btFaceStart + fwdVec;
  
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
      
      if(m_place_node)
      {
        auto ent = Entity_factory::create_random_cube(m_world_data);
        
        auto trans = ent.get_transform();
        trans.position = cube_entity.get_transform().position;
        ent.set_transform(trans);
        
        // Join rbs.
        void *user_ptr = face_ray.m_collisionObject->getUserPointer();
        const std::size_t ent_id = (std::size_t)user_ptr;
        const Core::Entity_id collided_id = Core::uint_as_entity(static_cast<uint32_t>(ent_id));
        
        ent.set_parent(collided_id);
      }
    }
  }
  
  m_place_node = false;
  
  Actor_base::on_update(dt); // Super to apply new transform
}


void
Actor_local_player::action()
{
  m_place_node = true;
}