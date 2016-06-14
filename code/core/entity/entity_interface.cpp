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
update_collider(const util::generic_id this_id,
                World_data::World *world,
                const math::transform *transform,
                const math::aabb *model_aabb)
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
      
      math::aabb collider_box;
      World_data::physics_data_get_property_aabb_collider(phys_data, this_id, &collider_box);
      
      const math::vec3 collider_scale = math::aabb_get_extents(collider_box);
      const math::vec3 transform_scale = transform->scale;
      const math::vec3 total_scale = math::vec3_multiply(collider_scale, transform_scale);

      // Order is important here! Scale then shift origin.
      math::aabb_scale(collider_box, total_scale);
      math::aabb_set_origin(collider_box, transform->position);      
      
      World_data::physics_data_set_property_transform(phys_data, this_id, *transform);
      World_data::physics_data_set_property_transformed_aabb_collider(phys_data, this_id, collider_box);
      
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
  
  math::aabb curr_aabb;
  World_data::data_lock(world->transform);
  World_data::transform_data_get_property_aabb(world->transform, this_id, &curr_aabb);
  World_data::data_unlock(world->transform);
  
  const math::transform new_transform = math::transform_init(set_transform.get_position(),
                                                           set_transform.get_scale(),
                                                           set_transform.get_rotation());
  
  // TODO: Some possible async ness here?
  update_transform(this_id, world, &new_transform);
  update_collider(this_id, world, &new_transform, &curr_aabb);
  update_mesh_renderer(this_id, world, &new_transform);
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
set_material(const util::generic_id this_id, World_data::World *world, const Core::Material &material)
{
  assert(this_id && world && material.exists());
  
  auto mesh_data = world->mesh_data;
  assert(mesh_data);

  // We need to find the material remove it.
  // Then insert it with draw calls with the same id.
  World_data::data_lock(mesh_data);
  {
    size_t find_index;
    World_data::Mesh_renderer_draw_call *draw;
    World_data::Mesh_renderer_draw_call copy;    

    // If it already exists. The data and erase the old info.
    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &find_index))
    {
      World_data::renderer_mesh_data_get_property_draw_call(mesh_data, this_id, &draw);
      copy = World_data::Mesh_renderer_draw_call(*draw);
      World_data::renderer_mesh_data_erase(mesh_data, this_id);
    }
    
    // Insert new draw call in order of material_id
    {
      size_t insert_point = mesh_data->size;
    
      // Loop through and find insert point
      for(size_t i = 0; i < mesh_data->size; ++i)
      {
        if(mesh_data->property_material_id[i] == material.get_id())
        {
          insert_point = i;
          break;
        }
      }
    
      World_data::renderer_mesh_data_insert(mesh_data, this_id, insert_point);
      World_data::renderer_mesh_data_set_property_material_id(mesh_data, this_id, material.get_id());
      World_data::renderer_mesh_data_set_property_draw_call(mesh_data, this_id, &copy);
    }
    
  }
  World_data::data_unlock(mesh_data);
}


Core::Material
get_material(const util::generic_id this_id, World_data::World *world)
{
  assert(this_id && world);
  
  util::generic_id id = 0;
  
  World_data::data_lock(world->mesh_data);
  
  World_data::renderer_mesh_data_get_property_material_id(world->mesh_data, this_id, &id);
  
  World_data::data_unlock(world->mesh_data);
  
  return Core::Material(id);
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
      mesh_data->property_draw_call[index].model_id = model.get_id();
    }
    else
    {
      // Has no material yet. Will insert one for the moment.
      World_data::renderer_mesh_data_insert(mesh_data, this_id, 0);
      mesh_data->property_draw_call[0].model_id = model.get_id();
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
}


namespace
{

inline void update_component(const util::generic_id this_id, World_data::World *data, const uint32_t component_id)
{
  auto entity_data = data->entity;
  
  World_data::data_lock(entity_data);

  uint32_t comps;
  World_data::entity_data_get_property_components(entity_data, this_id, &comps);

  // Add new component_id
  comps = comps | component_id;

  World_data::entity_data_set_property_components(entity_data, this_id, comps);
  
  World_data::data_unlock(entity_data);
}

}


void
set_collider(const util::generic_id this_id, World_data::World *world, const Core::Collider &collider)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  auto phys_pool = world->physics_data;
  assert(phys_pool);
  
  if(phys_pool)
  {
    auto transform_data = world->transform;
    assert(transform_data);
  
    size_t index;
    if(World_data::transform_data_exists(transform_data, this_id, &index))
    {
      //TODO: This can be async
      update_component(this_id, world, World_data::Entity_component::has_physics);
    
      switch(collider.get_type())
      {
        case(Core::Collider::Type::box):
        {
          const Box_collider box_collider = Collider_utis::cast_to_box_collider(collider);
          const math::vec3 box_scale = math::vec3_init(box_collider.get_x_half_extent() * 2.f,
                                                       box_collider.get_y_half_extent() * 2.f,
                                                       box_collider.get_z_half_extent() * 2.f);
          
          math::aabb collider_box = math::aabb_init(math::vec3_init(-0.5), math::vec3_init(+0.5));
          math::aabb_scale(collider_box, box_scale);
          
          // Get the current components and add physics
          {
            math::aabb entity_aabb;
            math::transform curr_transform;
            {
              World_data::data_lock(world->transform);
              World_data::transform_data_get_property_aabb(world->transform, this_id, &entity_aabb);
              World_data::transform_data_get_property_transform(world->transform, this_id, &curr_transform);
              World_data::data_unlock(world->transform);
            }
          
            // Set physics
            {
              World_data::data_lock(phys_pool);
              
              // Add the collider box
              if(World_data::physics_data_push_back(phys_pool, this_id))
              {
                World_data::physics_data_set_property_aabb_collider(phys_pool, this_id, collider_box);
              }
              
              World_data::data_unlock(phys_pool);
              
              update_collider(this_id, world, &curr_transform, &entity_aabb);
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