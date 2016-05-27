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
  lock(world->entity);
  const bool exists = World_data::entity_data_exists(world->entity, this_id);
  unlock(world->entity);
  
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
  lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    entity_data->user_data[index] = user_data;
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  unlock(entity_data);
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
  
  lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    user_data = entity_data->user_data[index];
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  unlock(entity_data);
  
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
  lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    tags = entity_data->tags[index];
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  unlock(entity_data);
  
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
  lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    entity_data->tags[index] = set_tags;
  }
  else
  {
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  unlock(entity_data);
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
  World_data::entity_data_set_name(world->entity, this_id, set_name);
}
  
  
const char*
get_name(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return nullptr;
  }
  
  return World_data::entity_data_get_name(world->entity, this_id);
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


void
set_transform(const util::generic_id this_id, World_data::World *world,const Transform &set_transform)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  auto data = world->transform;

  size_t index;
  if(World_data::transform_data_exists(data, this_id, &index))
  {
    const math::transform new_transform = math::transform_init(set_transform.get_position(), set_transform.get_scale(), set_transform.get_rotation());
    data->transform[index] = new_transform;
    
    // Update mesh renderer data
    {
      size_t mesh_index;
      if(World_data::mesh_renderer_exists(world->mesh_data, this_id, &mesh_index))
      {
        const math::mat4 world_mat = math::transform_get_world_matrix(new_transform);
        memcpy(world->mesh_data->mesh_draw_calls[mesh_index].world_matrix, &world_mat, sizeof(world_mat));
      }
    }
    
    World_data::physics_update(world->physics_data, this_id, &data->aabb[index], &data->transform[index]);
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
  }
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
    assert(get_index(&index, this_id, data->entity_id, data->size));
    math::transform local_transform = data->transform[index];
    
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
  
  auto data = world->mesh_data;
  World_data::mesh_renderer_update_texture(data, this_id, id);
}


uint32_t
get_material_id(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return 0;
  }

  auto data = world->mesh_data;

  size_t index;
  assert(get_index(&index, this_id, data->entity_id, data->size));
  return (uint32_t)data->mesh_draw_calls[index].texture;
}


void
set_material(const util::generic_id this_id, World_data::World *world, Core::Material &material)
{

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
  
  auto data = world->mesh_data;
  auto transform_data = world->transform;

  World_data::mesh_renderer_update_model(data, this_id, model.get_id());

  size_t index;
  if(World_data::transform_data_exists(transform_data, this_id, &index))
  {
    transform_data->aabb[index] = model.get_model_aabb();
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::entity_not_found());
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

  auto phys_pool = world->physics_data;
  assert(phys_pool);
  
  if(phys_pool)
  {
    auto transform_data = world->transform;
    assert(transform_data);
  
    size_t index;
    if(World_data::transform_data_exists(transform_data, this_id, &index))
    {
      const math::transform transform = transform_data->transform[index];
      math::aabb aabb = transform_data->aabb[index];
    
      switch(collider.get_type())
      {
        case(Core::Collider::Type::box):
        {
          const Box_collider box_collider = Collider_utis::cast_to_box_collider(collider);
          const math::vec3 box_scale = math::vec3_init(box_collider.get_x_half_extent() * 2.f, box_collider.get_y_half_extent() * 2.f, box_collider.get_z_half_extent() * 2.f);
          const math::vec3 final_box_scale = math::vec3_multiply(box_scale, transform.scale);
          
          math::aabb_scale(aabb, final_box_scale);
          
          World_data::physics_add(phys_pool, this_id, &aabb, &transform);
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
  return Collider();
}


void
set_rigidbody_properties(const util::generic_id this_id, World_data::World *world, const Core::Rigidbody_properties props)
{
  auto phys_pool = world->physics_data;
  assert(phys_pool);

  if (phys_pool)
  {
    World_data::physics_update_collision_mask(phys_pool, this_id, props.get_rb_id(), props.get_rb_mask());
  }
}


Core::Rigidbody_properties
get_rigidbody_properties(const util::generic_id this_id, World_data::World *world)
{
  return Rigidbody_properties();
}


} // ns
} // ns