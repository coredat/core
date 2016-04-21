#include "entity_interface.hpp"
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <core/physics/collider.hpp>
#include <data/world_data/world_data.hpp>
#include <data/resource_data/resource_data.hpp>
#include <core/transform/transform.hpp>
#include <math/transform/transform.hpp>
#include <utilities/logging.hpp>


namespace Core {
namespace Entity_detail {


Core::Entity_id
get_id(const Core::Entity_id this_id, World_data::World *world)
{
  return this_id;
}


bool
is_valid(const Core::Entity_id this_id, World_data::World *world)
{
  return !(this_id == Core::Entity_id_util::invalid_id());
}


uint32_t
get_tags(const Core::Entity_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return 0; }
  
  uint32_t index;
  if(Entity_id_util::find_index_linearly(&index,
                                         this_id,
                                         world->entity_pool->entity_id,
                                         world->entity_pool->size))
  {
    return world->entity_pool->entity_properties[index].tags;
  }
  
  return 0;
}


bool
has_tag(const Core::Entity_id this_id, World_data::World *world, const uint32_t tag_id)
{
  if(!is_valid(this_id, world)) { return 0; }
  
  const uint32_t tags = get_tags(this_id, world);
  
  return !!(tags & tag_id);
}


void
set_tags(const Core::Entity_id this_id, World_data::World *world, const uint32_t set_tags)
{
  if(!is_valid(this_id, world)) { return; }
  
  uint32_t index;
  if(Entity_id_util::find_index_linearly(&index,
                                         this_id,
                                         world->entity_pool->entity_id,
                                         world->entity_pool->size))
  {
    world->entity_pool->entity_properties[index].tags = set_tags;
  }
}


void
add_tag(const Core::Entity_id this_id, World_data::World *world, const uint32_t add_tag)
{
  if(!is_valid(this_id, world)) { return; }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags | add_tag);
}


void
remove_tag(const Core::Entity_id this_id, World_data::World *world, const uint32_t tag)
{
  if(!is_valid(this_id, world)) { return; }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags &~ tag);
}


void
set_name(const Core::Entity_id this_id, World_data::World *world, const char* set_name)
{
  if(!is_valid(this_id, world)) { return; }
  
  // set string.
  World_data::entity_pool_set_entity_name(world->entity_pool, this_id, set_name);
}
  
  
const char*
get_name(const Core::Entity_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return nullptr; }
  
  // set string.
  return World_data::entity_pool_get_entity_name(world->entity_pool, this_id);
}


namespace
{
  inline bool
  get_index(uint32_t *index, const Core::Entity_id id, const Core::Entity_id ents[], const uint32_t size)
  {
    if(!ents)
    {
      return false;
    }
    
    if(Entity_id_util::find_index_linearly(index, id, ents, size))
    {
      return true;
    }
    
    return false;
  }
}


void
send_event(const Core::Entity_id this_id, World_data::World *world, const uint32_t event_id,
                   const void *data,
                   const uint32_t size_of_data)
{
  assert(false); // This used to be about components. do we still need it?
  LOG_ERROR("Send event is disabled currently")
}


void
set_transform(const Core::Entity_id this_id, World_data::World *world,const Transform &set_transform)
{
  if(!is_valid(this_id, world)) { return; }
  
  auto ent_pool = world->entity_pool;

  uint32_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  
  math::transform new_transform = math::transform_init(set_transform.get_position(), set_transform.get_scale(), set_transform.get_rotation());
  ent_pool->transform[index] = new_transform;
  
  World_data::entity_graph_change_push(world->entity_graph_changes, this_id, World_data::Entity_graph_change::updated);
}


Transform
get_transform(const Core::Entity_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) {
    return Transform();
  }
  
  auto ent_pool = world->entity_pool;

  uint32_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  math::transform local_transform = ent_pool->transform[index];
  
  const math::vec3 pos = local_transform.position;
  const math::vec3 scale = local_transform.scale;
  
  return Core::Transform(pos,
                         scale,
                         local_transform.rotation);
}


void
set_material_id(const Core::Entity_id this_id, World_data::World *world, const uint32_t id)
{
  if(!is_valid(this_id, world)) { return; }
  
  auto ent_pool = world->entity_pool;

  uint32_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->texture[index] = id;
}


uint32_t
get_material_id(const Core::Entity_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return 0; }

  auto ent_pool = world->entity_pool;

  uint32_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  return (uint32_t)ent_pool->texture[index];
}


void
set_model(const Core::Entity_id this_id, World_data::World *world, const Core::Model &model)
{
  if(!is_valid(this_id, world)) { return; }
  
  auto ent_pool = world->entity_pool;

  uint32_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->model[index] = model.get_id();
  
  Resource_data::Resources *resource = Resource_data::get_resources();
  ent_pool->aabb[index] = model.get_model_aabb();
}


Core::Model
get_model(const Core::Entity_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return Core::Model(); }
  
  auto ent_pool = world->entity_pool;

  uint32_t index;
  assert(get_index(&index, this_id, ent_pool->entity_id, ent_pool->size));
  return Core::Model((uint32_t)ent_pool->model[index]);
}


void
set_collider(const Core::Entity_id this_id, World_data::World *world, const Core::Collider &collider)
{
}


Collider
get_collider(const Core::Entity_id this_id, World_data::World *world)
{
  return Collider();
}


} // ns
} // ns