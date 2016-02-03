#include "actor_local_player.hpp"
#include <renderer/renderer.hpp>
#include <btBulletCollisionCommon.h>
#include <systems/transform/transform.hpp>
#include <application/entity_factory.hpp>
#include <data/data.hpp>
#include <core/interface/entity.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "../common/ids_object_tags.hpp"
#include <iostream>


namespace
{
  btPairCachingGhostObject* obj = new btPairCachingGhostObject();
  btGhostPairCallback *cb = new btGhostPairCallback();
  
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


Actor_local_player::Actor_local_player()
{
  /* Not to be used to access the game world state. */
}


void
Actor_local_player::on_start()
{
  Actor_base::on_start();
  get_entity().add_tag(Tag::player);
  
  cube_id = Entity_factory::create_placement_cube(m_world_data).get_id();
}

void
Actor_local_player::on_update(const float dt)
{
  Actor_base::on_update(dt); // Super to apply new transform

  // ** TODO ** // Make this part of the base actor might get rid of the wibble.
  math::transform curr_trans = get_entity().get_transform();
  
//  obj->setWorldTransform(gl_to_bullet(curr_trans));
//  
//  for(size_t i = 0; i < obj->getNumOverlappingObjects(); i++)
//  {
//    
//  }
//  
//  auto resolve_overlap = [&]()
//  {
//    btManifoldArray manifoldArray;
//    auto pairArray = obj->getOverlappingPairCache()->getOverlappingPairArray();
//    
//    int numPairs = pairArray.size();
//
//    for (int i = 0; i < numPairs; ++i)
//    {
//      manifoldArray.clear();
//
//      const btBroadphasePair& pair = pairArray[i];
//
//      btBroadphasePair* collisionPair = m_world_data->physics_world->dynamics_world.getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
//
//      if (!collisionPair)
//      {
//        continue;
//      }
//
//      if (collisionPair->m_algorithm)
//      {
//        collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);
//      }
//
//      for(int j=0;j<manifoldArray.size();j++)
//      {
//        btPersistentManifold* manifold = manifoldArray[j];
//
//        //bool isFirstBody = manifold->getBody0() == obj;
//
//        //btScalar direction = isFirstBody ? btScalar(-1.0) : btScalar(1.0);
//
//        for(int p = 0; p < manifold->getNumContacts(); ++p)
//        {
//          const btManifoldPoint&pt = manifold->getContactPoint(p);
//
//          if(pt.getDistance() < 0.f)
//          {
//            // Get current position and move back on normal.
//            const float scale_of_full_normal = pt.m_normalWorldOnB.length();
//            const float pen_leng = pt.getDistance();
//            
//            math::vec3 norm = math::vec3_init(pt.m_normalWorldOnB.x(), 0, pt.m_normalWorldOnB.z()); // I think this normal needs corrected.
//            math::vec3 corrected_norm = math::vec3_normalize(norm);
//        
//            math::vec3 scaled_offset = math::vec3_scale(corrected_norm, (pt.getDistance()));
//          
//            math::vec3 new_pos = math::vec3_add(curr_trans.position, scaled_offset);
//            curr_trans.position = new_pos;
//          
//            // Ghost object needs to be updated.
//            
//            obj->setWorldTransform(gl_to_bullet(curr_trans));
//            get_entity().set_transform(curr_trans);
//            
//            return false;
//          }
//        }
//      }
//    }
//  
//    return true;
//  };
//  
//  
//  for(int i = 0; i < 1; ++i)
//  {
//    if(resolve_overlap())
//    {
//      break;
//    }
//  }
//  

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
      
//      if(m_place_node)
//      {
//        auto ent = Entity_factory::create_random_cube(m_world_data);
//        
//        auto trans = ent.get_transform();
//        trans.position = cube_entity.get_transform().position;
//        ent.set_transform(trans);
//        
//        // Join rbs.
//        void *user_ptr = face_ray.m_collisionObject->getUserPointer();
//        const std::size_t ent_id = (std::size_t)user_ptr;
//        const Core::Entity_id collided_id = Core::uint_as_entity(static_cast<uint32_t>(ent_id));
//        
//        ent.set_parent(collided_id);
//      }
    }
  }
  
  m_place_node = false;
  
}


void
Actor_local_player::action()
{
  m_place_node = true;
}