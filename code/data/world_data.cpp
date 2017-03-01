#include <data/world_data.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/world/pending_entity_removal_data.hpp>
#include <data/world/camera_data.hpp>
//#include <data/world/transform_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/text_draw_call_data.hpp>
//#include <data/world/entity_data.hpp>
//#include <data/world/transform_data.hpp>
#include <data/world/trigger_data.hpp>
#include <data/world/collision_data.hpp>
#include <data/world/light_data.hpp>
#include <data/context_data.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <utilities/logging.hpp>
#include <atomic>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <data/renderers/text/text_renderer.hpp>
#include <renderer/debug_line.hpp>
#include <data/graph/graph.hpp>
#include <data/physics/physics.hpp>


namespace Data {


World::World(const uint32_t instance_id, const uint32_t entity_hint)
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
  
//  Data::Transform_data *transform_data = new Data::Transform_data;
//  Data::transform_create(transform_data, entity_hint);
//  
//  Data::Entity_data *entity_data = new Data::Entity_data;
//  Data::entity_create(entity_data, entity_hint);
  
  Data::Text_draw_call_data *text_draw_calls = new Data::Text_draw_call_data;
  Data::text_draw_call_create(text_draw_calls, entity_hint);
  
  Data::Collision_data *collision_data = new Data::Collision_data;
  Data::collision_create(collision_data, entity_hint);
  
  Data::Light_data *light_data = new Data::Light_data;
  Data::light_create(light_data, entity_hint);
  
//  Bullet_data::setup(&physics_world);
  
  this->entity_removal = graph_changes;
  this->rigidbody_data = rb_data;
  this->mesh_data      = mesh_data;
  this->camera_data    = camera_data;
//  this->transform      = transform_data;
//  this->entity         = entity_data;
  this->text_data      = text_draw_calls;
  this->trigger_data   = trigger_data;
  this->collision_data = collision_data;
  this->light_data     = light_data;
  
  auto resources = get_context_data();
  
  this->text_renderer = Data::Text_renderer::initialize(
    resources->op_context,
    resources->op_buffer
  );
  
  scene_graph = Data::Graph::initialize();
  physics = Data::Physics::initialize();
  
  Data::Physics::debug_draw_wireframes(physics, true);
  Data::Physics::debug_draw_contact_points(physics, true);
  
  Data::Physics::debug_draw_line_cb(physics, [](
    const math::vec3 start,
    const math::vec3 end,
    const math::vec3 color)
    {
      Renderer::debug_line(start, end, color);
    }
  );
  
  Data::Physics::debug_draw_point_cb(physics, [](
    const math::vec3 pos,
    const math::vec3 color)
    {
      constexpr float size = 0.05f;
    
      Renderer::debug_line(
        math::vec3_add(pos, math::vec3_init(0,size,0)),
        math::vec3_subtract(pos, math::vec3_init(0,size,0)),
        color
      );
      
      Renderer::debug_line(
        math::vec3_add(pos, math::vec3_init(size,0,0)),
        math::vec3_subtract(pos, math::vec3_init(size,0,0)),
        color
      );
      
      Renderer::debug_line(
        math::vec3_add(pos, math::vec3_init(0,0,size)),
        math::vec3_subtract(pos, math::vec3_init(0,0,size)),
        color
      );
    }
  );
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
  
  Graph::destroy(this->scene_graph);
  
//  if(this->transform)
//  {
//    Data::transform_destroy(transform);
//    delete this->transform;
//  }
  
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
  
//  if(this->entity)
//  {
//    Data::entity_destroy(entity);
//    delete this->entity;
//  }
  
//  if(this->text_data)
//  {
//    Data::text_draw_call_destroy(text_data);
//    delete this->text_data;
//  }
//  
//  if(this->trigger_data)
//  {
//    Data::trigger_destroy(trigger_data);
//    delete this->trigger_data;
//  }
  
//  Bullet_data::remove_and_clear(&physics_world);
  
//  if(this->scene_graph)
//  {
//    Data::Graph::destroy(this->scene_graph);
//  }
}


void
world_update_scene_graph_changes(Data::World *world_data,
                                 const Data::Pending_entity_removal_data *graph_changes)
{
  for(uint32_t i = 0; i < graph_changes->size; ++i)
  {
    const uint32_t id = graph_changes->field_deleted_entity[i];
    
    Data::Graph::node_remove(world_data->scene_graph, id);
    
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

    // Remove RB/Trigger
    {
      Data::Physics::rigidbody_remove(world_data->physics, id);
      Data::Physics::trigger_remove(world_data->physics, id);
    }
  }
}


} // ns
