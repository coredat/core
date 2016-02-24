#include "entity.hpp"
#include <data/world_data/world_data.hpp>


namespace Core {


Entity::Entity()
{
}


void
Entity::destroy()
{
  if(!is_valid()) { return; }
  
  // Destroy all children.
  for(uint32_t c = 0; c < get_number_of_children(); ++c)
  {
    get_child(c).destroy();
  }
  
  // Destroy this.
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, m_this_id, World_data::Entity_graph_change::removed);
}


Core::Entity_id
Entity::get_id() const
{
  return m_this_id;
}


bool
Entity::is_valid() const
{
  return !(m_this_id == Core::Entity_id_util::invalid_id());
}


uint32_t
Entity::get_tags() const
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
Entity::has_tag(const uint32_t tag_id) const
{
  if(!is_valid()) { return 0; }
  
  const uint32_t tags = get_tags();
  
  return !!(tags & tag_id);
}


void
Entity::set_tags(const uint32_t set_tags)
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
Entity::add_tag(const uint32_t add_tag)
{
  if(!is_valid()) { return; }

  const uint32_t tags = get_tags();
  set_tags(tags | add_tag);
}


void
Entity::remove_tag(const uint32_t tag)
{
  if(!is_valid()) { return; }

  const uint32_t tags = get_tags();
  set_tags(tags &~ tag);
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
Entity::set_parent(const Core::Entity_id parent_id)
{
  if(!is_valid()) { return; }

  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->parent_id[index] = parent_id;
  
  // TODO: Need to check parent is valid?
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, parent_id, World_data::Entity_graph_change::updated);
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, m_this_id, World_data::Entity_graph_change::moved);
}


Entity
Entity::get_parent() const
{
  if(!is_valid()) { return Entity(); }
  
  uint32_t index;
  if(Entity_id_util::find_index_linearly(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size))
  {
    Entity parent;
    Detail::set_entity_members(&parent, m_world_data, m_world_data->entity_pool->parent_id[index]);
    
    return parent;
  }
  
  return Entity(); // has no parent.
}


uint32_t
Entity::get_number_of_children() const
{
  if(!is_valid()) { return 0; }

  auto ent_pool = m_world_data->entity_pool;

  uint32_t children_count(0);
  
  for(uint32_t i = 0; i < ent_pool->size; ++i)
  {
    const Core::Entity_id id = ent_pool->parent_id[i];
  
    if(id == m_this_id)
    {
      ++children_count;
    }
  }
  
  return children_count;
}


Entity
Entity::get_child(const uint32_t index) const
{
  if(!is_valid()) { return Entity(); }

  auto ent_pool = m_world_data->entity_pool;

  uint32_t children_count(0);
  
  for(uint32_t i = 0; i < ent_pool->size; ++i)
  {
    const Core::Entity_id id = ent_pool->parent_id[i];

    if(id == m_this_id)
    {
      if(index == children_count)
      {
        Core::Entity_id child_id = ent_pool->entity_id[i];
      
        Entity child;
        Detail::set_entity_members(&child, m_world_data, child_id);
        
        return child;
      }
    
      ++children_count;
    }
  }
  
  return Entity();
}


void
Entity::send_event(const uint32_t id,
                   const void *data,
                   const uint32_t size_of_data)
{
  // Find all components and send an event to.
  auto pool = m_world_data->logic_pool;
  
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->entity_id[i] == m_this_id)
    {
      reinterpret_cast<Core::Component*>(pool->object_locations[i])->on_event(id, data, size_of_data);
    }
  }
}


void
Entity::set_transform(const Transform &set_transform)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  const math::transform old_transform = ent_pool->transform[index];
  
  math::transform new_transform = math::transform_init(set_transform.get_position(), set_transform.get_scale(), set_transform.get_rotation());
  ent_pool->transform[index] = new_transform;
  
  // Apply transforms to children
  for(uint32_t c = 0; c < get_number_of_children(); ++c)
  {
    Entity child = get_child(c);
    
    const Transform child_transform = child.get_transform();
    
    // Offset rotation
    const math::quat rot_conj  = math::quat_conjugate(old_transform.rotation);
    const math::quat rot_prim  = math::quat_multiply(new_transform.rotation, rot_conj);
    const math::quat final_rot = math::quat_multiply(rot_prim, child_transform.get_rotation());
    
    // Offset position TODO: This needs to take into account rotatations, currently the calling code needs to deal with this.
    const math::vec3 new_offset = math::vec3_subtract(new_transform.position, old_transform.position);
    const math::vec3 final_pos  = math::vec3_add(child_transform.get_position(), new_offset);
    
    // Apply offset transform.
    const Transform offset_transform(final_pos, child_transform.get_scale(), final_rot);
    child.set_transform(offset_transform);
  }
  
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, m_this_id, World_data::Entity_graph_change::updated);
}


Transform
Entity::get_transform() const
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
Entity::set_material_id(const uint32_t id)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->texture[index] = (Resource::Texture::ENUM)id;
}


uint32_t
Entity::get_material_id() const
{
  if(!is_valid()) { return 0; }

  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return (uint32_t)ent_pool->texture[index];
}


void
Entity::set_model_id(const uint32_t id)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->model[index] = (Resource::Model::ENUM)id;
}


uint32_t
Entity::get_model_id() const
{
  if(!is_valid()) { return 0; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return (uint32_t)ent_pool->model[index];
}


void
Entity::set_rigidbody_properties(const Physics::Rigidbody_properties props)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  
  auto rb_prop = &ent_pool->rigidbody_property[index];

  *rb_prop = props;
  rb_prop->id = m_this_id;
  
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, m_this_id, World_data::Entity_graph_change::moved);
}


Physics::Rigidbody_properties
Entity::get_rigidbody_properties() const
{
  if(!is_valid()) { return Physics::Rigidbody_properties(); }

  uint32_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  return m_world_data->entity_pool->rigidbody_property[index];
}


void
Entity::set_rigidbody_collider(const Physics::Rigidbody_collider collider)
{
  if(!is_valid()) { return; }

  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  
  auto rb_coll = &ent_pool->rigidbody_collider[index];
  
  *rb_coll = collider;
  
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, m_this_id, World_data::Entity_graph_change::moved);
}


Physics::Rigidbody_collider
Entity::get_rigidbody_collider() const
{
  if(!is_valid()) { return Physics::Rigidbody_collider(); }

  auto ent_pool = m_world_data->entity_pool;

  uint32_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return m_world_data->entity_pool->rigidbody_collider[index];
}


uint32_t
Entity::get_number_of_components() const
{
  const uint32_t count = World_data::logic_pool_get_slot_count(m_world_data->logic_pool, m_this_id);

  return count;
}


} // ns