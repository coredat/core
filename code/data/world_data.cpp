#include <data/world_data.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/world/pending_entity_removal_data.hpp>
#include <data/world/camera_data.hpp>
#include <data/world/transform_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/transform_data.hpp>
#include <data/world/trigger_data.hpp>
#include <data/world/collision_data.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <utilities/logging.hpp>
#include <atomic>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


namespace Data {


World::World(const util::generic_id instance_id, const uint32_t entity_hint)
: world_instance_id(instance_id)
{
  Data::Pending_entity_removal_data *graph_changes = new Data::Pending_entity_removal_data();
  Data::pending_entity_removal_create(graph_changes, entity_hint);
  
  Data::Camera_data *camera_data = new Data::Camera_data;
  Data::camera_create(camera_data, 32);
    
  Data::Rigidbody_data* rb_data = new Data::Rigidbody_data;
  Data::rigidbody_create(rb_data, entity_hint);
  
  Data::Trigger_data *trigger_data = new Data::Trigger_data;
  Data::trigger_create(trigger_data, entity_hint);
  
  Data::Mesh_draw_call_data *mesh_data = new Data::Mesh_draw_call_data;
  Data::mesh_draw_call_create(mesh_data, entity_hint);
  
  Data::Transform_data *transform_data = new Data::Transform_data;
  Data::transform_create(transform_data, entity_hint);
  
  Data::Entity_data *entity_data = new Data::Entity_data;
  Data::entity_create(entity_data, entity_hint);
  
  Data::Text_draw_call_data *text_draw_calls = new Data::Text_draw_call_data;
  Data::text_draw_call_create(text_draw_calls, entity_hint);
  
  Data::Collision_data *collision_data = new Data::Collision_data;
  Data::collision_create(collision_data, entity_hint);
  
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  dynamicsWorld->setGravity(btVector3(0, -10, 0));  
  
  this->entity_removal = graph_changes;
  this->rigidbody_data = rb_data;
  this->mesh_data      = mesh_data;
  this->camera_data    = camera_data;
  this->transform      = transform_data;
  this->entity         = entity_data;
  this->text_data      = text_draw_calls;
  this->trigger_data   = trigger_data;
  this->collision_data = collision_data;
}

World::~World()
{
  if(this->entity_removal)
  {
    Data::pending_entity_removal_destroy(entity_removal);
    delete this->entity_removal;
  }
  
  if(this->rigidbody_data)
  {
    Data::rigidbody_destroy(rigidbody_data);
    delete this->rigidbody_data;
  }
  
  if(this->transform)
  {
    Data::transform_destroy(transform);
    delete this->transform;
  }
  
  if(this->mesh_data)
  {
    Data::mesh_draw_call_destroy(mesh_data);
    delete this->mesh_data;
  }
  
  if(this->camera_data)
  {
    Data::camera_destroy(camera_data);
    delete this->camera_data;
  }
  
  if(this->entity)
  {
    Data::entity_destroy(entity);
    delete this->entity;
  }
  
  if(this->text_data)
  {
    Data::text_draw_call_destroy(text_data);
    delete this->text_data;
  }
  
  if(this->trigger_data)
  {
    Data::trigger_destroy(trigger_data);
    delete this->trigger_data;
  }
}


void
world_update_scene_graph_changes(Data::World *world_data,
                                 const Data::Pending_entity_removal_data *graph_changes)
{
  for(uint32_t i = 0; i < graph_changes->size; ++i)
  {
    const util::generic_id id = graph_changes->field_deleted_entity[i];
    
    entity_remove(world_data->entity, id);
    transform_remove(world_data->transform, id);
    
    if(Data::mesh_draw_call_exists(world_data->mesh_data, id))
    {
      Data::mesh_draw_call_remove(world_data->mesh_data, id);
    }
    
    {
      if(text_draw_call_exists(world_data->text_data, id))
      {
        text_draw_call_exists(world_data->text_data, id);
      }
    }

    // Remove RB
    {
      Data::Rigidbody_data *rb_data = world_data->rigidbody_data;
      Data::data_lock(rb_data);
      
      if(Data::rigidbody_exists(rb_data, id))
      {
        uintptr_t body_ptr = 0;
        Data::rigidbody_get_rigidbody(rb_data, id, &body_ptr);
        
        btRigidBody *body = reinterpret_cast<btRigidBody*>(body_ptr);
        
        if(body)
        {
          world_data->dynamicsWorld->removeRigidBody(body);
          
          delete body->getMotionState();
          delete body->getCollisionShape();
          delete body;
        }
        
        Data::rigidbody_remove(rb_data, id);
      }
      
      Data::data_unlock(rb_data);
    }
    
    // Remove Trigger
    {
      Data::Trigger_data *trigger_data = world_data->trigger_data;
      Data::data_lock(trigger_data);
      
      if(Data::trigger_exists(trigger_data, id))
      {
        uintptr_t trigger_ptr = 0;
        Data::trigger_get_trigger(trigger_data, id, &trigger_ptr);
        
        btGhostObject *ghost = reinterpret_cast<btGhostObject*>(trigger_ptr);
        
        if(ghost)
        {
          world_data->dynamicsWorld->removeCollisionObject(ghost);
        
          delete ghost->getCollisionShape();
          delete ghost;
        }
      }
      
      Data::data_unlock(trigger_data);
    }
  }
}


} // ns