#include "actor_base.hpp"
#include <math/transform/transform.hpp>
#include <core/interface/entity.hpp>
#include <renderer/renderer.hpp>
#include <systems/transform/transform.hpp>


Actor_base::Actor_base()
{
}


Actor_base::~Actor_base()
{
}


void
Actor_base::on_update(const float dt)
{
  math::transform curr_trans = get_entity().get_transform();
  
  // Apply gravity
  {
    const auto actor_length = math::vec3_get_y(m_size);
  
    // Cast ray downwards
    const btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));
    const btVector3 btTo(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - actor_length, math::vec3_get_z(curr_trans.position));
    btCollisionWorld::ClosestRayResultCallback feet_test(btFrom, btTo);
    
    m_world_data->physics_world->dynamics_world.rayTest(btFrom, btTo, feet_test);

    Renderer::debug_line(btFrom, btTo, btVector3(1, 1, 0));
    
    // If hit something apply any adjustments.
    if(feet_test.hasHit())
    {
      const auto hit_point          = math::vec3_init(feet_test.m_hitPointWorld.x(), feet_test.m_hitPointWorld.y(), feet_test.m_hitPointWorld.z());
      const auto corrected_position = math::vec3_init(math::vec3_get_x(hit_point), math::vec3_get_y(hit_point) + actor_length, math::vec3_get_z(hit_point));
      
      curr_trans.position = corrected_position;
    }
    // Else apply gravity.
    else
    {
      const auto gravity = math::vec3_init(0, m_gravity * dt, 0);
      
      curr_trans.position = math::vec3_add(curr_trans.position, gravity);
    }
  }
  
  // Apply positional movements
  {
    const auto move_fwd = math::vec3_get_z(m_pending_move);
  
    if(move_fwd != 0)
    {
      math::vec3 left;
      Transform::get_left_vec(&curr_trans, &left);

      const math::vec3 fwd                      = math::vec3_cross(Transform::world_up(), left);
      const math::vec3 norm_fwd                 = math::vec3_normalize(fwd);
      const math::vec3 scaled_fwd               = math::vec3_scale(norm_fwd, move_fwd * dt);
      const math::vec3 norm_corrected_rotation  = math::vec3_normalize(scaled_fwd);
      const math::vec3 positional_movement      = math::vec3_scale(norm_corrected_rotation, dt);
      
      curr_trans.position = math::vec3_add(positional_movement, curr_trans.position);
    }
    
    m_pending_move = math::vec3_zero();
  }
  
  // Turn actor
  {
    const float rot_rad   = static_cast<float>(math::vec3_get_y(m_acuumulated_rotations));
    math::quat turn_actor = math::quat_init_with_axis_angle(0, 1, 0, rot_rad);
    
    curr_trans.rotation   = turn_actor;
  }
  
  // Apply head movements
  {
    const float rot_rad            = static_cast<float>(math::vec3_get_x(m_acuumulated_rotations));
    const math::vec3 axis          = math::vec3_init(1,0,0);
    const math::vec3 adjusted_axis = math::quat_rotate_point(curr_trans.rotation, axis);
    const math::quat pitch_head    = math::quat_init_with_axis_angle(adjusted_axis, rot_rad);
    
    curr_trans.rotation = math::quat_multiply(curr_trans.rotation, pitch_head);
  }
  
  get_entity().set_transform(curr_trans);
}


void
Actor_base::move_forward(const float fwd)
{
  const float accum_fwd = math::vec3_get_z(m_pending_move) + fwd;
  m_pending_move = math::vec3_init(math::vec3_get_x(m_pending_move), math::vec3_get_y(m_pending_move), accum_fwd);
}


void
Actor_base::move_left(const float left)
{
  const float accum_right = math::vec3_get_x(m_pending_move) + left;
  m_pending_move = math::vec3_init(math::vec3_get_x(m_pending_move), accum_right, math::vec3_get_z(m_pending_move));
}


void
Actor_base::look_up(const float pitch)
{
  const float accum_up      = math::vec3_get_x(m_acuumulated_rotations) + pitch;
  const float clamped_accum = math::clamp(accum_up, +math::quart_tau(), -math::quart_tau());
  
  m_acuumulated_rotations = math::vec3_init(clamped_accum, math::vec3_get_y(m_acuumulated_rotations), math::vec3_get_z(m_acuumulated_rotations));
}


void
Actor_base::turn_left(const float turn)
{
  const float accum_right = math::vec3_get_y(m_acuumulated_rotations) + turn;
  m_acuumulated_rotations = math::vec3_init(math::vec3_get_x(m_acuumulated_rotations), accum_right, math::vec3_get_z(m_acuumulated_rotations));
}