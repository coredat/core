#include "actor_local_player.hpp"
#include <renderer/renderer.hpp>
#include <btBulletCollisionCommon.h>
#include <systems/transform/transform.hpp>


Actor_local_player::Actor_local_player()
{
}


void
Actor_local_player::on_update(const float dt)
{
  auto apply_gravity = [&](const Entity::Entity_id ent)
  {
    struct Kine_actor
    {
      bool is_grounded = false;
    } act;
  
    // Get transform of controller.

    const math::transform curr_trans = get_transform();
    
    // Cast ray downwards
    const btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));
    const btVector3 btTo(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - 2, math::vec3_get_z(curr_trans.position));
    btCollisionWorld::ClosestRayResultCallback feet_test(btFrom, btTo);
    
    m_world->dynamics_world.rayTest(btFrom, btTo, feet_test);

    Renderer::debug_line(btFrom, btTo, btVector3(1, 1, 0));
    
    // If not hit anything then go downwards.
    // Gravity
    if(!feet_test.hasHit())
    {
      const math::vec3 down = math::vec3_init(0, -0.01, 0);
      
      math::transform new_trans = curr_trans;
      new_trans.position = math::vec3_add(curr_trans.position, down);
      
      set_transform(new_trans);
    }
    else
    {
      const auto norm = feet_test.m_hitPointWorld;

      math::transform new_trans = curr_trans;
      math::vec3 pos = math::vec3_init(math::vec3_get_x(curr_trans.position), norm.y() + 2, math::vec3_get_z(curr_trans.position));
      new_trans.position = pos;
      
     set_transform(new_trans);
      
      act.is_grounded = true;
    }
    
    math::vec3 fwd;
    Transform::get_fwd_vec(&curr_trans, &fwd);
    fwd = math::vec3_scale(fwd, 2);
    
    const btVector3 fwdVec(math::vec3_get_x(fwd), math::vec3_get_y(fwd), math::vec3_get_z(fwd));
    const btVector3 btFaceStart(btFrom);
    const btVector3 btFwd = btFaceStart + fwdVec;
    
    btCollisionWorld::ClosestRayResultCallback face_ray(btFaceStart, btFwd);
    m_world->dynamics_world.rayTest(btFaceStart, btFwd, face_ray);
    
    Renderer::debug_line(btFaceStart, btFwd, btVector3(1, 0, 0));
    
    if(face_ray.hasHit())
    {
      // Draw cube at that point.
      
    }
  };
  
  apply_gravity(get_entity());
}