#include <core/entity/entity_interface.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>

#include <core/transform/transform.hpp>
#include <core/renderer/renderer.hpp>
#include <core/camera/camera.hpp>
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

#include <core/entity/entity_components.hpp>

#include <common/error_strings.hpp>
#include <utilities/utilities.hpp>


namespace Core {

// ----------------------------------------------------- [ Entity Interface ]--

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

Entity_interface::Entity_interface(const uint32_t id)
: m_id(id)
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


// ------------------------------------------------------------- [ Lifetime ]--


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


// -------------------------------------------------------------- [ General ]--


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


// ------------------------------------------------------------ [ Relations ]--


bool
Entity_interface::set_parent(const Core::Entity_ref other)
{
  auto world_data = Core_detail::world_index_get_world_data(1);
  
  const bool success = Data::Graph::node_set_parent(
    world_data->scene_graph,
    m_id,
    other.get_id()
  );
  
  return success;
}


Core::Entity_ref
Entity_interface::get_parent() const
{
  auto world_data = Core_detail::world_index_get_world_data(1);
  
  const uint32_t parent_id = Data::Graph::node_get_parent(
    world_data->scene_graph,
    m_id
  );
  
  return Core::Entity_ref(parent_id);
}


  
uint32_t
Entity_interface::get_child_count() const
{
  auto world_data = Core_detail::world_index_get_world_data(1);
  
  return Data::Graph::node_get_child_count(
    world_data->scene_graph,
    m_id
  );
}


Core::Entity_ref
Entity_interface::get_child(const size_t i)
{
  auto world_data = Core_detail::world_index_get_world_data(1);
  
  const uint32_t child = Data::Graph::node_get_child(
    world_data->scene_graph,
    m_id,
    i
  );
  
  return Core::Entity_ref(child);
}


// ------------------------------------------------------------- [ Messages ]--


void
Entity_interface::on_message_callback(const on_message_callback_fn &callback,
                                      const uintptr_t user_data)
{
  auto world_data = Core_detail::world_index_get_world_data(1);
  
  Data::Graph::message_callback_set(
    world_data->scene_graph,
    m_id,
    user_data,
    (uintptr_t)callback
  );
}


void
Entity_interface::send_message(const Entity_ref to,
                               const uint32_t id,
                               const uintptr_t data) const
{
  // -- Param Check -- //
  LIB_ASSERT(to);

  auto world_data = Core_detail::world_index_get_world_data(1);
  
  uintptr_t user_data = 0;
  uintptr_t callback_fn = 0;
  
  Data::Graph::message_callback_get(
    world_data->scene_graph,
    to.m_id,
    &user_data,
    &callback_fn
  );
  
  if(callback_fn)
  {
    ((on_message_callback_fn)callback_fn)
    (
      to,
      Core::Entity_ref(m_id),
      id,
      data,
      user_data
    );
  }
}


// ------------------------------------------------------------ [ Transform ]--

  
bool
Entity_interface::has_transform() const
{
  return Entity_component::has_transform(Core::Entity_ref(m_id));
}


Core::Transform
Entity_interface::get_transform() const
{
  return Entity_component::get_transform(Core::Entity_ref(m_id));
}


void
Entity_interface::set_transform(const Core::Transform &trans)
{
  Entity_component::set_transform(Core::Entity_ref(m_id), trans);
}

// --------------------------------------------------------------- [ Camera ]--

bool
Entity_interface::has_camera() const
{
  return Entity_component::has_camera(Core::Entity_ref(m_id));
}


Core::Camera
Entity_interface::get_camera() const
{
  return Entity_component::get_camera(Core::Entity_ref(m_id));
}


void
Entity_interface::set_camera(const Core::Camera &camera)
{
  Entity_component::set_camera(Core::Entity_ref(m_id), camera);
}


// ------------------------------------------------------------- [ Renderer ]--


// ------------------------------------------------------------ [ Rigidbody ]--

bool
Entity_interface::has_rigidbody() const
{
  return Entity_component::has_rigidbody(Core::Entity_ref(m_id));
}


Core::Rigidbody
Entity_interface::get_rigidbody() const
{
  return Entity_component::get_rigidbody(Core::Entity_ref(m_id));
}


void
Entity_interface::set_rigidbody(const Core::Rigidbody &rb)
{
  Entity_component::set_rigidbody(Core::Entity_ref(m_id), rb);
}


void
Entity_interface::on_collision_callback(
  const uintptr_t user_data,
  const on_collision_callback_fn &callback
)
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

// ------------------------------------------------------------- [ Lighting ]--

// ------------------------------------------------------------- [ Equality ]--

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


// ---------------------------------------------------- [ Private/Protected ]--


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
