#include "entity_interface.hpp"
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <data/world_data/world_data.hpp>
#include <core/transform/transform.hpp>
#include <math/transform/transform.hpp>
#include <utilities/logging.hpp>


namespace Core {


Core::Entity_id
Entity_interface::get_id() const
{
  return m_this_id;
}


bool
Entity_interface::is_valid() const
{
  return !(m_this_id == Core::Entity_id_util::invalid_id());
}


uint32_t
Entity_interface::get_tags() const
{
  if(!is_valid()) { return 0; }
  
  uint32_t index;
  if(Entity_id_util::find_index_linearly(&index,
                                         m_this_id,
                                         m_world_data->entity_pool->entity_id,
                                         m_world_data->entity_pool->size))
  {
    return m_world_data->entity_pool->entity_properties[index].tags;
  }
  
  return 0;
}


bool
Entity_interface::has_tag(const uint32_t tag_id) const
{
  if(!is_valid()) { return 0; }
  
  const uint32_t tags = get_tags();
  
  return !!(tags & tag_id);
}


void
Entity_interface::set_tags(const uint32_t set_tags)
{
  if(!is_valid()) { return; }
  
  uint32_t index;
  if(Entity_id_util::find_index_linearly(&index,
                                         m_this_id,
                                         m_world_data->entity_pool->entity_id,
                                         m_world_data->entity_pool->size))
  {
    m_world_data->entity_pool->entity_properties[index].tags = set_tags;
  }
}


void
Entity_interface::add_tag(const uint32_t add_tag)
{
  if(!is_valid()) { return; }

  const uint32_t tags = get_tags();
  set_tags(tags | add_tag);
}


void
Entity_interface::remove_tag(const uint32_t tag)
{
  if(!is_valid()) { return; }

  const uint32_t tags = get_tags();
  set_tags(tags &~ tag);
}


void
Entity_interface::set_name(const char* set_name)
{
  if(!is_valid()) { return; }
  
  // set string.
  World_data::entity_pool_set_entity_name(m_world_data->entity_pool, m_this_id, set_name);
}
  
  
const char*
Entity_interface::get_name() const
{
  if(!is_valid()) { return nullptr; }
  
  // set string.
  return World_data::entity_pool_get_entity_name(m_world_data->entity_pool, m_this_id);
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
Entity_interface::send_event(const uint32_t event_id,
                   const void *data,
                   const uint32_t size_of_data)
{
  assert(false); // This used to be about components. do we still need it?
  LOG_ERROR("Send event is disabled currently")
}


void
Entity_interface::set_transform(const Transform &set_transform)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  const math::transform old_transform = ent_pool->transform[index];
  
  math::transform new_transform = math::transform_init(set_transform.get_position(), set_transform.get_scale(), set_transform.get_rotation());
  ent_pool->transform[index] = new_transform;
  
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, m_this_id, World_data::Entity_graph_change::updated);
}


Transform
Entity_interface::get_transform() const
{
  if(!is_valid()) {
    return Transform();
  }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  math::transform* local_transform = &ent_pool->transform[index];
  
  return Core::Transform(local_transform->position, local_transform->scale, local_transform->rotation);
}


void
Entity_interface::set_material_id(const uint32_t id)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->texture[index] = id;
}


uint32_t
Entity_interface::get_material_id() const
{
  if(!is_valid()) { return 0; }

  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return (uint32_t)ent_pool->texture[index];
}


void
Entity_interface::set_model(const Core::Model &model)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->model[index] = model.get_id();
  
  //ent_pool->aabb[index] = m_world_data->model_pool->aabb[id];
}


Core::Model
Entity_interface::get_model() const
{
  if(!is_valid()) { return Core::Model(); }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return Core::Model((uint32_t)ent_pool->model[index]);
}


void
Entity_interface::set_collider(const Core::Collider &collider)
{
}


Collider
Entity_interface::get_collider() const
{
  return Collider();
}


} // ns