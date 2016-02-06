#include "Actor.hpp"
#include <math/transform/transform.hpp>
#include <core/interface/entity.hpp>
#include <renderer/renderer.hpp>
#include <systems/transform/transform.hpp>
#include "../common/ids_object_tags.hpp"
#include <iostream>


namespace
{
  inline math::transform
  bullet_to_gl(const btTransform &transform)
  {
    const auto rotation         = transform.getRotation();
    const math::quat rb_quat    = math::quat_init(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    const math::mat3 rot_mat    = math::quat_get_rotation_matrix(rb_quat);
    const math::mat3 rot_mat_tr = math::mat3_get_transpose(rot_mat);
    const math::quat rb_quat2   = math::quat_init_with_mat3(rot_mat_tr);
    
    const auto pos_data         = transform.getOrigin();
    const math::vec3 rb_pos     = math::vec3_init(pos_data.x(), pos_data.y(), pos_data.z());
    
    math::transform adjusted_transform;
    adjusted_transform.rotation = rb_quat2;
    adjusted_transform.position = rb_pos;
    adjusted_transform.scale    = math::vec3_one(); // This might not be correct, Assuming that no scale from bullet.
    
    return adjusted_transform;
  }
  
  inline btTransform
  gl_to_bullet(const math::transform &transform)
  {
    math::quat bt_friendly_rot;
    {
      const math::mat3 rot_mat    = math::quat_get_rotation_matrix(transform.rotation);
      const math::mat3 rot_mat_tr = math::mat3_get_transpose(rot_mat);
      
      bt_friendly_rot = math::quat_init_with_mat3(rot_mat_tr);
    }
    
    btTransform adjusted_transform;
    adjusted_transform.setOrigin(btVector3(math::vec3_get_x(transform.position),
                                           math::vec3_get_y(transform.position),
                                           math::vec3_get_z(transform.position)));

    adjusted_transform.setRotation(btQuaternion(math::quat_get_x(bt_friendly_rot),
                                                math::quat_get_y(bt_friendly_rot),
                                                math::quat_get_z(bt_friendly_rot),
                                                math::quat_get_w(bt_friendly_rot)));

    return adjusted_transform;
  }
}


Actor::Actor()
{
}


void
Actor::on_start()
{
  m_ghost_obj.reset(new btPairCachingGhostObject());
  m_ghost_obj->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
  m_ghost_obj->setUserPointer(Core::Entity_id_util::convert_entity_to_ptr(get_entity().get_id()));
  
  m_pair_cb.reset(new btGhostPairCallback());
  
  m_collisionshape.reset(new btCapsuleShape(0.35,0.75));
  m_ghost_obj->setCollisionShape(m_collisionshape.get());
  
  m_world_data->physics_world->dynamics_world.addCollisionObject(m_ghost_obj.get(), btBroadphaseProxy::AllFilter, btBroadphaseProxy::AllFilter);
  m_world_data->physics_world->dynamics_world.getPairCache()->setInternalGhostPairCallback(m_pair_cb.get());
  
  get_entity().add_tag(Tag::actor);
}


void
Actor::on_update(const float dt)
{
  math::transform curr_trans = get_entity().get_transform();
  
  // Apply gravity
  {
    const auto actor_length = math::vec3_get_y(m_size);
    
    // Cast ray downwards from feet
    const btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - actor_length + 0.2, math::vec3_get_z(curr_trans.position));
    const btVector3 btTo(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - actor_length, math::vec3_get_z(curr_trans.position));
    btCollisionWorld::ClosestRayResultCallback feet_test(btFrom, btTo);
    
    m_ghost_obj->setWorldTransform(gl_to_bullet(curr_trans)); // move outta the way.
    
    m_world_data->physics_world->dynamics_world.rayTest(btFrom, btTo, feet_test);

    Renderer::debug_line(btFrom, btTo, btVector3(1, 1, 0));
    
    // If hit something apply any adjustments.
    if(feet_test.hasHit())
    //if(false)
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
    const auto move_fwd  = math::vec3_get_z(m_pending_move);
    const auto move_left = math::vec3_get_x(m_pending_move);
    
    // TODO:
    // We will want to normalise or at least clamp this.
    // 1 forward and 1 left makes movement 1.4. it should be 0.7
  
    if(move_fwd != 0)
    {
      math::vec3 left;
      Transform::get_left_vec(&curr_trans, &left);
      
      // TODO:
      // Can the player's 'left' ever point directly up?
      // If it does we'll get a zero cross product.
      
      const math::vec3 fwd = math::vec3_cross(Transform::world_up(), left);
      const math::vec3 fwd_norm = math::vec3_normalize(fwd);
      const math::vec3 scaled_fwd = math::vec3_scale(fwd_norm, move_fwd * dt * 4);
      
      curr_trans.position = math::vec3_add(curr_trans.position, scaled_fwd);
    }
    
    if(move_left != 0)
    {
      math::vec3 fwd;
      Transform::get_fwd_vec(&curr_trans, &fwd);
      
      // TODO
      // If player looks up we get a 0 cross product.
      // Don't use world up.
      
      const math::vec3 left = math::vec3_cross(fwd, Transform::world_up());
      const math::vec3 left_norm = math::vec3_normalize(left);
      const math::vec3 scaled_left = math::vec3_scale(left_norm, move_left * dt * 4);
      
      curr_trans.position = math::vec3_add(curr_trans.position, scaled_left);
    }
    
    m_pending_move = math::vec3_zero();
  }
  
  // Turn actor
  {
    const float rot_rad   = static_cast<float>(math::vec3_get_y(m_acuumulated_rotations));
    math::quat turn_actor = math::quat_init_with_axis_angle(0, 1, 0, rot_rad);
    
    curr_trans.rotation = turn_actor;
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
  
  const float ghost_offset = -0.55;
  
  if(m_ghost_obj)
  {
    //curr_trans.rotation = math::quat_init();
    math::transform collider_trans = curr_trans;
    collider_trans.rotation = math::quat_init();
    collider_trans.position = math::vec3_add(curr_trans.position, math::vec3_init(0, ghost_offset, 0));
    m_ghost_obj->setWorldTransform(gl_to_bullet(collider_trans));
  
    //** testing ghost stuff **//
    
    for(size_t i = 0; i < m_ghost_obj->getNumOverlappingObjects(); i++)
    {
      
    }
    
    auto resolve_overlap = [&]()
    {
      btVector3 offset(0,0,0);
      
      btManifoldArray manifoldArray;
      auto pairArray = m_ghost_obj->getOverlappingPairCache()->getOverlappingPairArray();
      
      int numPairs = pairArray.size();

      btScalar maxPen = btScalar(0.0);
      for (int i = 0; i < m_ghost_obj->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
      {
        const btBroadphasePair& pair = pairArray[i];
        btBroadphasePair* collisionPair = m_world_data->physics_world->dynamics_world.getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);

        manifoldArray.resize(0);

        //btBroadphasePair* collisionPair = &m_ghost_obj->getOverlappingPairCache()->getOverlappingPairArray()[i];

        btCollisionObject* obj0 = static_cast<btCollisionObject*>(collisionPair->m_pProxy0->m_clientObject);
        btCollisionObject* obj1 = static_cast<btCollisionObject*>(collisionPair->m_pProxy1->m_clientObject);

        if ((obj0 && !obj0->hasContactResponse()) || (obj1 && !obj1->hasContactResponse()))
        {
         // continue;
        }
        
        if (collisionPair->m_algorithm)
        {
          collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);
        }
        
        for (int j=0;j<manifoldArray.size();j++)
        {
          btPersistentManifold* manifold = manifoldArray[j];
          btScalar directionSign = manifold->getBody0() == m_ghost_obj.get() ? btScalar(-1.0) : btScalar(1.0);
          for (int p=0;p<manifold->getNumContacts();p++)
          {
            const btManifoldPoint&pt = manifold->getContactPoint(p);

            btScalar dist = pt.getDistance();

            if (dist < 0.0)
            {
              if (dist < maxPen)
              {
                maxPen = dist;
                //m_touchingNormal = pt.m_normalWorldOnB * directionSign;//??
              }
              offset += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);
              //penetration = true;
            }
            else {
              //printf("touching %f\n", dist);
            }
          }
          
          manifold->clearManifold();
        }
      }
      
      // Ghost object needs to be updated.
      math::vec3 final_offset = math::vec3_init(offset.x(), offset.y(), offset.z());
      collider_trans.position = math::vec3_add(curr_trans.position, final_offset);
      
      curr_trans.position = collider_trans.position;
      
      collider_trans.position = math::vec3_add(curr_trans.position, math::vec3_init(0, ghost_offset, 0));
      
      m_ghost_obj->setWorldTransform(gl_to_bullet(collider_trans));
      get_entity().set_transform(curr_trans);
      
      return false;
    
    //  return true;
    };
    
    
    for(int i = 0; i < 5; ++i)
    {
      if(resolve_overlap())
      {
        break;
      }
    }
  }
}


void
Actor::on_event(const uint32_t id, const void *data, const size_t size_of_data)
{
  std::cout << "evt yo!" << std::endl;
}


void
Actor::move_forward(const float fwd)
{
  const float accum_fwd = math::vec3_get_z(m_pending_move) + fwd;
  m_pending_move        = math::vec3_init(math::vec3_get_x(m_pending_move), math::vec3_get_y(m_pending_move), accum_fwd);
}


void
Actor::move_left(const float left)
{
  const float accum_left = math::vec3_get_x(m_pending_move) + left;
  m_pending_move         = math::vec3_init(accum_left, math::vec3_get_y(m_pending_move), math::vec3_get_z(m_pending_move));
}


void
Actor::look_up(const float pitch)
{
  const float accum_up      = math::vec3_get_x(m_acuumulated_rotations) + pitch;
  const float clamped_accum = math::clamp(accum_up, +math::quart_tau(), -math::quart_tau());
  m_acuumulated_rotations   = math::vec3_init(clamped_accum, math::vec3_get_y(m_acuumulated_rotations), math::vec3_get_z(m_acuumulated_rotations));
}


void
Actor::turn_left(const float turn)
{
  const float accum_right = math::vec3_get_y(m_acuumulated_rotations) + turn;
  m_acuumulated_rotations = math::vec3_init(math::vec3_get_x(m_acuumulated_rotations), accum_right, math::vec3_get_z(m_acuumulated_rotations));
}


void
Actor::take_damage()
{
  size_t index;
  Core::Entity_id_util::find_index_linearly(&index,
                                            get_entity().get_id(),
                                            m_world_data->entity_pool->entity_id,
                                            m_world_data->entity_pool->size);

  m_world_data->entity_pool->texture[index] = Resource::Texture::dev_green;
}