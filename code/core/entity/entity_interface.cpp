#include "entity_interface.hpp"
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/collider_utils.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <core/material/material.hpp>
#include <data/world_data/world_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <core/transform/transform.hpp>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>
#include <utilities/logging.hpp>


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
  return !(this_id == util::generic_id_invalid());
}


uint32_t
get_tags(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return 0; }
  
  size_t index;
  if(util::generic_id_search_binary(&index,
                                    this_id,
                                    world->entity_pool->entity_id,
                                    world->entity_pool->size))
  {
    return world->entity_pool->entity_properties[index].tags;
  }
  
  return 0;
}


bool
has_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag_id)
{
  if(!is_valid(this_id, world)) { return 0; }
  
  const uint32_t tags = get_tags(this_id, world);
  
  return !!(tags & tag_id);
}


void
set_tags(const util::generic_id this_id, World_data::World *world, const uint32_t set_tags)
{
  if(!is_valid(this_id, world)) { return; }
  
  size_t index;
  if(util::generic_id_search_binary(&index,
                                    this_id,
                                    world->entity_pool->entity_id,
                                    world->entity_pool->size))
{
    world->entity_pool->entity_properties[index].tags = set_tags;
  }
}


void
add_tag(const util::generic_id this_id, World_data::World *world, const uint32_t add_tag)
{
  if(!is_valid(this_id, world)) { return; }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags | add_tag);
}


void
remove_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag)
{
  if(!is_valid(this_id, world)) { return; }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags &~ tag);
}


void
set_name(const util::generic_id this_id, World_data::World *world, const char* set_name)
{
  if(!is_valid(this_id, world)) { return; }
  
  // set string.
  World_data::entity_pool_set_entity_name(world->entity_pool, this_id, set_name);
}
  
  
const char*
get_name(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return nullptr; }
  
  // set string.
  return World_data::entity_pool_get_entity_name(world->entity_pool, this_id);
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
send_event(const util::generic_id this_id, World_data::World *world, const uint32_t event_id,
                   const void *data,
                   const uint32_t size_of_data)
{
  assert(false); // This used to be about components. do we still need it?
  LOG_ERROR("Send event is disabled currently")
}


void
set_transform(const util::generic_id this_id, World_data::World *world,const Transform &set_transform)
{
  if(!is_valid(this_id, world)) { return; }
  
  auto ent_pool = world->entity_pool;

  size_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  
  math::transform new_transform = math::transform_init(set_transform.get_position(), set_transform.get_scale(), set_transform.get_rotation());
  ent_pool->transform[index] = new_transform;
  
  World_data::entity_graph_change_push(world->entity_graph_changes, this_id, World_data::Entity_graph_change::updated);
  
  // Update collider
  {
    World_data::physics_update(world->physics_data, this_id, &ent_pool->aabb[index], &ent_pool->transform[index]);
  }
}


Transform
get_transform(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) {
    return Transform();
  }
  
  auto ent_pool = world->entity_pool;

  size_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  math::transform local_transform = ent_pool->transform[index];
  
  const math::vec3 pos = local_transform.position;
  const math::vec3 scale = local_transform.scale;
  
  return Core::Transform(pos,
                         scale,
                         local_transform.rotation);
}


void
set_material_id(const util::generic_id this_id, World_data::World *world, const uint32_t id)
{
  if(!is_valid(this_id, world)) { return; }
  
  auto data = world->mesh_data;
  World_data::mesh_renderer_update_texture(data, this_id, id);
}


uint32_t
get_material_id(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return 0; }

  auto data = world->mesh_data;

  size_t index;
  assert(get_index(&index, this_id, data->entity_id, data->size));
  return (uint32_t)data->texture[index];
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
  if(!is_valid(this_id, world)) { return; }
  
  auto data = world->mesh_data;
  auto ent_pool = world->entity_pool;

  World_data::mesh_renderer_update_model(data, this_id, model.get_id());

  size_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  
  ent_pool->aabb[index] = model.get_model_aabb();
}


Core::Model
get_model(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return Core::Model(); }
  
  auto ent_pool = world->entity_pool;

  size_t index;
//  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
//  return Core::Model((uint32_t)ent_pool->model[index]);
}


void
set_collider(const util::generic_id this_id, World_data::World *world, const Core::Collider &collider)
{
  auto phys_pool = world->physics_data;
  assert(phys_pool);
  
  if(phys_pool)
  {
    auto entity_pool = world->entity_pool;
    assert(entity_pool);
  
    size_t index;
    assert(World_data::entity_pool_find_index(entity_pool, this_id, &index));
    
    const math::transform transform = World_data::entity_pool_get_transform(entity_pool, this_id);
    math::aabb aabb = entity_pool->aabb[index];
  
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
        LOG_ERROR("Unknown collider type.");
    }
  }
  else
  {
    LOG_ERROR("No physics data has been setups.");
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