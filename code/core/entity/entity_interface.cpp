#include <core/entity/entity_interface.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>

#include <core/transform/transform.hpp>
#include <core/renderer/renderer.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/world/world.hpp>
#include <core/world/detail/world_index.hpp>

#include <data/world/pending_entity_removal_data.hpp>
#include <data/world_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/graph/graph.hpp>

#include <common/data_types.hpp>

#include <transformations/entity/entity_common.hpp>
#include <transformations/entity/entity_data.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_renderer.hpp>
#include <transformations/entity/entity_callback.hpp>

#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace
{
  std::atomic<uint32_t> entity_instance_counter(0);
}


namespace Core {


Entity_interface::Entity_interface()
{
}


Entity_interface::Entity_interface(const Entity_interface &other)
{
  copy(other);
}


Entity_interface::Entity_interface(Entity_interface &&other)
{
  move(other);
}

Entity_interface::Entity_interface(const Core_detail::Entity_id id)
: m_id(Core_detail::entity_id_to_uint(id))
{
}

Entity_interface::Entity_interface(Core::World &world)
: m_id(0)
{
//  bool success = true;
 
  auto world_data = Core_detail::world_index_get_world_data(world.get_id());
 
  if(world_data)
  {
    m_id = Data::Graph::node_add(world_data->scene_graph);
  }
  else
  {
    LOG_ERROR(Error_string::entity_is_invalid());
  }
}


Entity_interface::~Entity_interface()
{
}


// ** Life time ** //


void
Entity_interface::destroy()
{
  if(!m_id)
  {
    return;
  }
  
  auto world_data = Core_detail::world_index_get_world_data(m_id);

  Data::Graph::node_remove(world_data->scene_graph, m_id);
  
  m_id = 0;
}


bool
Entity_interface::is_valid() const
{
  if(!m_id)
  {
    return false;
  }
  
  auto world_data = Core_detail::world_index_get_world_data(1);

  return Data::Graph::node_exists(world_data->scene_graph, m_id);
}


Entity_interface::operator bool() const
{
  return is_valid();
}


// ** General Interface ** //


uint32_t
Entity_interface::get_tags() const
{
  // Validity check
  if(!m_id)
  {
    return 0;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);
  
  uint32_t tags = 0;
  Data::Graph::tags_get(world_data->scene_graph, m_id, &tags);
  
  return tags;
}


void
Entity_interface::set_user_data(const uintptr_t user_data)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);
  
  Data::Graph::user_data_set(world_data->scene_graph, m_id, user_data);
}


uintptr_t
Entity_interface::get_user_data() const
{
  // Validity check
  if(!m_id)
  {
    return 0;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);
  
  uintptr_t user_data = 0;
  Data::Graph::user_data_get(world_data->scene_graph, m_id, &user_data);
  
  return user_data;
}


bool
Entity_interface::has_tag(const uint32_t tag_id) const
{
  // Validity check
  if(!m_id)
  {
    return false;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);
  
  uint32_t tags = 0;
  Data::Graph::tags_get(world_data->scene_graph, m_id, &tags);
  
  return !!(tags & tag_id);
}


void
Entity_interface::set_tags(const uint32_t set_tags)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);

  Data::Graph::tags_set(world_data->scene_graph, m_id, set_tags);
}


void
Entity_interface::add_tag(const uint32_t add_tag)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);
  
  uint32_t tags = 0;
  Data::Graph::tags_get(world_data->scene_graph, m_id, &tags);
  
  tags = tags | add_tag;
  Data::Graph::tags_set(world_data->scene_graph, m_id, tags);
}


void
Entity_interface::remove_tag(const uint32_t tag)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);

  uint32_t tags = 0;
  Data::Graph::tags_get(world_data->scene_graph, m_id, &tags);
  
  tags = tags &~ tag;
  Data::Graph::tags_set(world_data->scene_graph, m_id, tags);
}


void
Entity_interface::set_name(const char *set_name)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  LOG_WARNING("Not setup");
}


const char*
Entity_interface::get_name() const
{
  // Validity check
  if(!m_id)
  {
    return "";
  }
  
  LOG_WARNING("Not setup");

  return "";
}


uint32_t
Entity_interface::get_id() const
{
  return m_id;
}


// ** Callbacks ** //


void
Entity_interface::on_collision_callback(
  const uintptr_t user_data,
  const on_collision_callback_fn &callback)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  auto world_data = Core_detail::world_index_get_world_data(1);
  
  Data::Graph::node_add_collision_callback(
    world_data->scene_graph,
    m_id,
    user_data,
    (uintptr_t)callback
  );
}


// ** Equality ** //


bool
Entity_interface::operator==(const Entity_interface &other) const
{
  return this->get_id() == other.get_id();
}


bool
Entity_interface::operator!=(const Entity_interface &other) const
{
  return this->get_id() != other.get_id();
}


// ** Protected/private utilities ** //


void
Entity_interface::copy(const Entity_interface &other)
{
  m_id = other.m_id;
}


void
Entity_interface::move(Entity_interface &other)
{
  copy(other);
  
  other.m_id = 0;
}


} // ns