#include "entity_interface.hpp"
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/collider_utils.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <core/resources/material.hpp>
#include <data/world_data/world_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <core/transform/transform.hpp>
#include <common/error_strings.hpp>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>
#include <utilities/logging.hpp>
#include <utilities/bits.hpp>
#include <data/world_data/transform_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/pending_scene_graph_change_data.hpp>


namespace Core {
namespace Entity_detail {


util::generic_id
get_id(const util::generic_id this_id, World_data::World *world)
{
  return this_id;
}


bool
is_valid(const util::generic_id this_id, World_data::World *world)
{
  // Invalid data, means invalid entity.
  if(this_id == util::generic_id_invalid()) { return false; }
  if(!world)                                { return false; }

  // Our id might have expired, so we need to check it.
  data_lock(world->entity);
  const bool exists = World_data::entity_data_exists(world->entity, this_id);
  data_unlock(world->entity);
  
  return exists;
}


void
destroy(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return; }
  
  // Destroy this.
  World_data::pending_scene_graph_change_delete(world->entity_graph_changes, this_id);
}


void
set_user_data(const util::generic_id this_id, World_data::World *world, const uintptr_t user_data)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return;
  }
  
  auto entity_data = world->entity;
  
  size_t index;
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    entity_data->property_user_data[index] = user_data;
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
}


uintptr_t
get_user_data(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return 0;
  }
  
  auto entity_data = world->entity;
  
  size_t index;
  uintptr_t user_data(0);
  
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    user_data = entity_data->property_user_data[index];
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
  
  return user_data;
}


uint32_t
get_tags(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return 0;
  }
  
  auto entity_data = world->entity;
  
  uint32_t tags = 0;
  
  size_t index;
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    tags = entity_data->property_tag[index];
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
  
  return tags;
}


bool
has_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag_id)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return 0;
  }
  
  const uint32_t tags = get_tags(this_id, world);
  
  return !!(tags & tag_id);
}


void
set_tags(const util::generic_id this_id, World_data::World *world, const uint32_t set_tags)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  auto entity_data = world->entity;
  
  size_t index;
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    entity_data->property_tag[index] = set_tags;
  }
  else
  {
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
}


void
add_tag(const util::generic_id this_id, World_data::World *world, const uint32_t add_tag)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags | add_tag);
}


void
remove_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags &~ tag);
}


void
set_name(const util::generic_id this_id, World_data::World *world, const char* set_name)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  // set string.
  World_data::data_lock(world->entity);
  
  World_data::entity_data_set_property_name(world->entity, this_id, set_name);
  
  World_data::data_unlock(world->entity);
}
  
  
const char*
get_name(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return nullptr;
  }
  assert(false);
  const char *name;
  World_data::entity_data_get_property_name(world->entity, this_id, &name);
  
  return name;
}


namespace
{
  inline bool
  get_index(size_t *index, const util::generic_id id, const util::generic_id ents[], const uint32_t size)
  {
    if(!ents)
    {
      return false;
    }
    
    if(util::generic_id_search_binary(index, id, ents, size))
    {
      return true;
    }
    
    return false;
  }
}


namespace
{

inline void
update_transform(const util::generic_id this_id, World_data::World *world, const math::transform *transform)
{
  auto transform_data = world->transform;
  
  World_data::data_lock(transform_data);

  size_t index;
  if(World_data::transform_data_exists(transform_data, this_id, &index))
  {
    transform_data->property_transform[index] = *transform;
  }
  
  World_data::data_unlock(transform_data);
}


inline void
update_mesh_renderer(const util::generic_id this_id, World_data::World *world, const math::transform *transform)
{
  auto mesh_data = world->mesh_data;

  // Update mesh renderer data
  {
    World_data::data_lock(mesh_data);
  
    size_t mesh_index;

    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &mesh_index))
    {
      const math::mat4 world_mat = math::transform_get_world_matrix(*transform);
      memcpy(mesh_data->property_draw_call[mesh_index].world_matrix, &world_mat, sizeof(world_mat));
    }
    
    World_data::data_unlock(mesh_data);
  }
}


inline void
update_collider(const util::generic_id this_id, World_data::World *world, const math::transform *transform)
{
  auto phys_data = world->physics_data;
  
  // If this is a physics object then update it.
  {
    World_data::data_lock(world->entity);
  
    uint32_t components;
    World_data::entity_data_get_property_components(world->entity,
                                                    this_id,
                                                    &components);
    
    World_data::data_unlock(world->entity);
    
    // Update the physics stuff.
    if(components & World_data::Entity_component::has_physics)
    {
      World_data::data_lock(phys_data);
      
      math::aabb box;
      World_data::physics_data_get_property_aabb_collider(phys_data, this_id, &box);
      
      math::aabb_set_origin(box, transform->position);
      math::aabb_scale(box, transform->scale);
      
      World_data::physics_data_set_property_transform(phys_data, this_id, *transform);
      World_data::physics_data_set_property_transformed_aabb_collider(phys_data, this_id, box);
      
      World_data::data_unlock(phys_data);
    }
  }
}


}


void
set_transform(const util::generic_id this_id,
              World_data::World *world,
              const Transform &set_transform)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  const math::transform new_transform = math::transform_init(set_transform.get_position(),
                                                           set_transform.get_scale(),
                                                           set_transform.get_rotation());
  
  // TODO: This can be done via async.
  update_transform(this_id, world, &new_transform);
  update_collider(this_id, world, &new_transform);
  update_mesh_renderer(this_id, world, &new_transform);

//  size_t index;
//  if(World_data::transform_data_exists(transform_data, this_id, &index))
//  {
//    const math::transform new_transform = math::transform_init(set_transform.get_position(),
//                                                               set_transform.get_scale(),
//                                                               set_transform.get_rotation());
//    transform_data->property_transform[index] = new_transform;
//    
//    // Update mesh renderer data
//    {
//      World_data::data_lock(world->mesh_data);
//    
//      size_t mesh_index;
//
//      if(World_data::renderer_mesh_data_exists(world->mesh_data, this_id, &mesh_index))
//      {
//        const math::mat4 world_mat = math::transform_get_world_matrix(new_transform);
//        memcpy(world->mesh_data->property_draw_call[mesh_index].world_matrix, &world_mat, sizeof(world_mat));
//      }
//      
//      World_data::data_unlock(world->mesh_data);
//    }
//    
//    // If this is a physics object then update it.
//    {
//      uint32_t components;
//      World_data::entity_data_get_property_components(world->entity,
//                                               this_id,
//                                               &components);
//      
//      // Update the physics stuff.
//      if(components & World_data::Entity_component::has_physics)
//      {
//        auto phys_data = world->physics_data;
//      
//        World_data::data_lock(phys_data);
//        
//        math::aabb box = transform_data->property_aabb[index];
//        
//        World_data::physics_data_set_property_transform(phys_data, this_id, new_transform);
//        World_data::physics_data_set_property_aabb_collider(phys_data, this_id, box);
//        
//        // Transform box
////        math::aabb_set_origin(box, new_transform.position);
////        math::aabb_scale(box, new_transform.scale);
//        
//        World_data::physics_data_set_property_transformed_aabb_collider(phys_data, this_id, box);
//        
//        World_data::data_unlock(phys_data);
//      }
//    }
//  }
//  else
//  {
//    LOG_ERROR(Error_string::entity_not_found());
//  }
}


Transform
get_transform(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return Transform();
  }
  
  auto data = world->transform;

  size_t index;
  if(World_data::transform_data_exists(data, this_id, &index))
  {
    math::transform local_transform = data->property_transform[index];
    
    const math::vec3 pos = local_transform.position;
    const math::vec3 scale = local_transform.scale;
    
    return Core::Transform(pos,
                           scale,
                           local_transform.rotation);
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
  }
  
  return Core::Transform();
}


void
set_material_id(const util::generic_id this_id, World_data::World *world, const uint32_t id)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  // Update mesh renderer data
  auto data = world->mesh_data;
  {
    World_data::data_lock(data);
    
    size_t index;
    
    if(World_data::renderer_mesh_data_exists(data, this_id, &index))
    {
      data->property_draw_call[index].texture = id;
    }
    
    World_data::data_unlock(data);
  }
}


uint32_t
get_material_id(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return 0;
  }

  uint32_t return_id = 0;

  auto data = world->mesh_data;
  
  World_data::data_lock(data);
  
  size_t index;
  
  if(World_data::renderer_mesh_data_exists(data, this_id, &index))
  {
    return_id = data->property_draw_call[index].texture;
  }
  
  World_data::data_unlock(data);
  
  return return_id;
}


void
set_material(const util::generic_id this_id, World_data::World *world, Core::Material &material)
{
  assert(false);
  LOG_ERROR(Error_string::no_implimentation());
}


Core::Material
get_material(const util::generic_id this_id, World_data::World *world)
{
  return Core::Material("");
}


void
set_model(const util::generic_id this_id, World_data::World *world, const Core::Model &model)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  // Update mesh renderer data
  auto mesh_data = world->mesh_data;
  {
    World_data::data_lock(mesh_data);
    
    size_t index;
    
    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &index))
    {
      mesh_data->property_draw_call[index].model = model.get_id();
    }
    
    World_data::data_unlock(mesh_data);
  }

  // Update aabb
  auto transform_data = world->transform;
  {
    World_data::data_lock(transform_data);
    World_data::transform_data_set_property_aabb(transform_data, this_id, model.get_model_aabb());
    World_data::data_unlock(transform_data);
  }
}


Core::Model
get_model(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return Core::Model();
  }
  
//  auto ent_pool = world->entity_pool;
//
//  size_t index;
//  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
//  return Core::Model((uint32_t)ent_pool->model[index]);
}


void
set_collider(const util::generic_id this_id, World_data::World *world, const Core::Collider &collider)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }

  auto entity_data = world->entity;
  assert(entity_data);

  auto phys_pool = world->physics_data;
  assert(phys_pool);
  
  if(phys_pool)
  {
    auto transform_data = world->transform;
    assert(transform_data);
  
    size_t index;
    if(World_data::transform_data_exists(transform_data, this_id, &index))
    {
      const math::transform transform = transform_data->property_transform[index];
      math::aabb aabb = transform_data->property_aabb[index];
    
      switch(collider.get_type())
      {
        case(Core::Collider::Type::box):
        {
          const Box_collider box_collider = Collider_utis::cast_to_box_collider(collider);
          const math::vec3 box_scale = math::vec3_init(box_collider.get_x_half_extent() * 2.f, box_collider.get_y_half_extent() * 2.f, box_collider.get_z_half_extent() * 2.f);
          const math::vec3 final_box_scale = math::vec3_multiply(box_scale, transform.scale);
          
          math::aabb_scale(aabb, final_box_scale);
          
          // Get the current components and add physics
          {
            uint32_t comps;
            World_data::entity_data_get_property_components(entity_data, this_id, &comps);
            
            // Add physics and set it.
            comps = comps | World_data::Entity_component::has_physics;
            
            World_data::entity_data_set_property_components(entity_data, this_id, comps);
            
            // Set physics
            {
              World_data::data_lock(phys_pool);
              
              if(World_data::physics_data_push_back(phys_pool, this_id))
              {
              
//                World_data::physics_data_set_property_transform(phys_pool, this_id, transform);
//                
                math::aabb box;
                World_data::physics_data_get_property_aabb_collider(phys_pool, this_id, &box);
                World_data::physics_data_set_property_aabb_collider(phys_pool, this_id, aabb);
//

                update_collider(this_id, world, &transform);
//                math::aabb_scale(aabb, transform.scale);
//                math::aabb_set_origin(aabb, transform.position);
//                
//                World_data::physics_data_set_property_transformed_aabb_collider(phys_pool, this_id, aabb);
//                World_data::physics_data_set_property_collision_id(phys_pool, this_id, 0);
              }
              
              World_data::data_unlock(phys_pool);
            }
          }
          break;
        }
        
        case(Core::Collider::Type::unknown):
        default:
          assert(false);
          LOG_ERROR(Error_string::unknown_type());
      }
    }
    else
    {
      LOG_ERROR(Error_string::entity_not_found());
      return;
    }
  }
  else
  {
    LOG_ERROR(Error_string::data_not_found());
  }
}


Collider
get_collider(const util::generic_id this_id, World_data::World *world)
{
  assert(false);
  LOG_ERROR(Error_string::no_implimentation());
  return Collider();
}


void
set_rigidbody_properties(const util::generic_id this_id, World_data::World *world, const Core::Rigidbody_properties props)
{
  auto phys_pool = world->physics_data;
  assert(phys_pool);

  if (phys_pool)
  {
    World_data::data_lock(phys_pool);
    
    const uint64_t mask = util::bits_pack(props.get_rb_id(), props.get_rb_mask());
    
    World_data::physics_data_set_property_collision_id(phys_pool, this_id, mask);
    World_data::data_unlock(phys_pool);
  }
}


Core::Rigidbody_properties
get_rigidbody_properties(const util::generic_id this_id, World_data::World *world)
{
  assert(false);
  LOG_ERROR(Error_string::no_implimentation());
  return Rigidbody_properties();
}


} // ns
} // ns