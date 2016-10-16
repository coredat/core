#include "world_pools.hpp"
#include "renderer_mesh_data.hpp"
#include "rigidbody_data.hpp"
#include "pending_entity_removal.hpp"
#include "camera_data.hpp"
#include "transform_data.hpp"
#include "renderer_mesh_data.hpp"
#include "renderer_text_draw_calls_data.hpp"
#include "entity_data.hpp"
#include "transform.hpp"
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <utilities/logging.hpp>
#include <atomic>


namespace World_data {


World::World(const uint32_t entity_hint)
{
  this->scene = new q3Scene(1.f / 60.f);

  Data::Pending_entity_removal_data *graph_changes = new Data::Pending_entity_removal_data();
  Data::pending_entity_removal_create(graph_changes, entity_hint);
  
  Camera_data *camera_data = new Camera_data;
  camera_data_init(camera_data, 32);
    
  Data::Rigidbody_data* rb_data = new Data::Rigidbody_data;
  Data::rigidbody_create(rb_data, entity_hint);
  
  Renderer_mesh_data *mesh_data = new Renderer_mesh_data;
  renderer_mesh_data_init(mesh_data, entity_hint);
  
  Data::Transform_data *transform_data = new Data::Transform_data;
  Data::transform_create(transform_data, entity_hint);
  
  Entity_data *entity_data = new Entity_data;
  World_data::entity_data_init(entity_data, entity_hint);
  
  Renderer_text_draw_calls_data *text_draw_calls = new Renderer_text_draw_calls_data;
  renderer_text_draw_calls_data_init(text_draw_calls, entity_hint);
  
  this->entity_removal       = graph_changes;
  this->rigidbody_data       = rb_data;
  this->mesh_data            = mesh_data;
  this->camera_data          = camera_data;
  this->transform            = transform_data;
  this->entity               = entity_data;
  this->text_data            = text_draw_calls;
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
  
  delete this->mesh_data;
  delete this->camera_data;
  delete this->entity;
  delete this->text_data;
}


void
world_find_entities_with_tag(World *world_data,
                             const uint32_t tag,
                             uint32_t *out_entities_for_tag,
                             util::generic_id out_ids[],
                             const uint32_t size_of_out)
{
  assert(world_data);
  
  auto entity_data = world_data->entity;
  
  uint32_t number_found(0);
  
  for(uint32_t i = 0; i < entity_data->size; ++i)
  {
    auto tags = entity_data->property_tag[i];
    
    if(tags & tag)
    {
      if(size_of_out > number_found)
      {
        out_ids[number_found++] = entity_data->entity_id[i];
      }
      else
      {
        break;
      }
    }
  }
  
  (*out_entities_for_tag) = number_found;
}


void
world_update_scene_graph_changes(World_data::World *world_data,
                                 const Data::Pending_entity_removal_data *graph_changes)
{
  for(uint32_t i = 0; i < graph_changes->size; ++i)
  {
    const util::generic_id id = graph_changes->field_deleted_entity[i];
    
    entity_data_erase(world_data->entity, id);
    transform_remove(world_data->transform, id);
    
    if(renderer_mesh_data_exists(world_data->mesh_data, id))
    {
      renderer_mesh_data_erase(world_data->mesh_data, id);
    }
    
    {
      if(renderer_text_draw_calls_data_exists(world_data->text_data, id))
      {
        renderer_text_draw_calls_data_erase(world_data->text_data, id);
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
        
        q3Body *body = reinterpret_cast<q3Body*>(body_ptr);
        
        if(body)
        {
          world_data->scene->RemoveBody(body);
        }
        
        Data::rigidbody_remove(rb_data, id);
      }
      
      Data::data_unlock(rb_data);
    }
  }
}


} // ns