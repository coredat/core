#include <core/world/world.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <core/physics/collision.hpp>
#include <core/common/ray.hpp>
#include <data/context_data.hpp>
#include <data/context_data/input_pool.hpp>
#include <data/graph/graph.hpp>
#include <data/physics/physics.hpp>
#include <data/physics/contact.hpp>
#include <core/context/detail/context_data.hpp>
#include <common/fixed_string_search.hpp>
#include <systems/engine/engine.hpp>
#include <systems/engine/tick_information.hpp>
#include <debug_gui/debug_menu.hpp>
#include <utilities/utilities.hpp>


namespace Core {


struct World::Impl
{
  uint32_t           world_instance_id  = 0;
  Core::Context      *context           = nullptr;
  lib::milliseconds  dt_timer           = 0;
  float              dt                 = 0.f;
  float              dt_mul             = 1.f;
  float              running_time       = 0.f;
  Collision_callback collision_callback = nullptr;
  
  void
  move(World::Impl *this_one, World::Impl *that_one)
  {
    this_one->world_instance_id  = that_one->world_instance_id;
    that_one->world_instance_id  = 0;
    
    this_one->context            = that_one->context;
    that_one->context            = nullptr;
    
    this_one->dt_timer           = that_one->dt_timer;
    that_one->dt_timer           = 0;
    
    this_one->dt                 = that_one->dt;
    that_one->dt                 = 0.f;
    
    this_one->dt_mul             = that_one->dt_mul;
    that_one->dt_mul             = 1.f;
    
    this_one->running_time       = that_one->running_time;
    that_one->running_time       = 0.f;
    
    this_one->collision_callback = that_one->collision_callback;
    that_one->collision_callback = nullptr;
  }
};


World::World(Context &ctx, const World_setup setup)
: m_impl(new World::Impl)
{
  m_impl->world_instance_id = Core_detail::world_index_add_world_data(setup.entity_pool_size);
  m_impl->context = &ctx;
  m_impl->dt_timer = lib::timer::get_current_time();
  
  Engine::initialize(Core_detail::world_index_get_world_data(m_impl->world_instance_id));
}


World::~World()
{
  Core_detail::world_index_release_world_data(m_impl->world_instance_id);
}


World::World(World &&other)
{
  m_impl->move(m_impl.get(), other.m_impl.get());
}


World&
World::operator=(World &&other)
{
  m_impl->move(m_impl.get(), other.m_impl.get());
  return *this;
}


float
World::get_delta_time() const
{
  return m_impl->dt * m_impl->dt_mul;
}


void
World::set_delta_time_multiplier(const float multiplier)
{
  assert(m_impl);
  
  m_impl->dt_mul = multiplier;
}
  
  
float
World::get_delta_time_multiplier() const
{
  assert(m_impl);
  
  return m_impl->dt_mul;
}


uint32_t
World::get_time_running() const
{
  assert(m_impl);
  
  return static_cast<uint32_t>(m_impl->running_time * 1000);
}


void
World::think()
{
  // Calculate delta_time
  {
    const lib::milliseconds now = lib::timer::get_current_time();
    const lib::milliseconds frame_time = lib::timer::get_delta(m_impl->dt_timer, now);
    m_impl->dt = lib::timer::to_seconds(frame_time);
    m_impl->dt_timer = now;
    
    m_impl->running_time += m_impl->dt;
  }

  // Engine Think
  Engine::Tick_information tick_info;
  {
    auto resources = Data::get_context_data();
    assert(resources);
    
    auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
    assert(world);

    Engine::think(
      world,
      resources,
      m_impl->dt,
      m_impl->running_time,
      m_impl->context->get_width(),
      m_impl->context->get_height(),
      &tick_info);
  }
  
  /*
    Debug Menu
    --
    Shows the debugging menu bar at the top of the screen.
  */
  #ifdef CORE_DEBUG_MENU
  {
    auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
  
    Debug_menu::display_global_data_menu(m_impl->context->get_context_data()->input_pool);
    Debug_menu::display_world_data_menu(
      world.get(),
      m_impl->dt,
      m_impl->dt_mul,
      12345,
      tick_info.number_of_draw_calls,
      tick_info.camera_runs
    );
  }
  #endif
}


void
World::set_collision_callback(Collision_callback callback)
{
  assert(m_impl);
  m_impl->collision_callback = callback;
  
  LOG_TODO_ONCE("Remove this hack callback");
  Engine::set_collision_callback(callback);
}


size_t
World::get_entity_count_in_world() const
{
  auto world = Core_detail::world_index_get_world_data(1);

  return Data::Graph::node_count(world->scene_graph);
}


Entity_ref
World::find_entity_by_id(const uint32_t id) const
{
  return Entity_ref(Core_detail::entity_id_from_uint(id));
}


void
World::find_entities_by_tag(const uint32_t tag_id,
                            Entity_ref **out_array,
                            size_t *out_array_size)
{
//  static Entity_ref found_ents[1024];
//  static size_t count = 0;
//  
//  count = 0;
//  
//  assert(m_impl && m_impl->world_instance_id);
//  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
//  
//  // Loop through entity data and find entities.
//  auto data = world->entity;
//  
//  data_lock(data);
//  
//  for(uint32_t i = 0; i < data->size; ++i)
//  {
//    if(Data::entity_get_tags_data(data)[i])
//    {
//      found_ents[count] = Entity_ref(Core_detail::entity_id_from_uint(data->keys[i]));
//      count++;
//    }
//  }
//  
//  data_unlock(data);
//  
//  *out_array = found_ents;
//  *out_array_size = count;
}


Contact
World::find_entity_by_ray(const Ray ray) const
{
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);

  Data::Physics::Physics_data *phys = world->physics;
  
  uint32_t contacts = 0;
  Data::Physics::Contact contact;
  
  Data::Physics::world_find_with_ray(
    phys,
    ray.get_origin(),
    math::vec3_scale(
      ray.get_direction(),
      ray.get_distance()
    ),
    &contact,
    1,
    &contacts
  );
  
  if(contacts)
  {
    return Contact(
      Core::Entity_ref(contact.user_data),
      contact.contact_point,
      contact.contact_normal,
      contact.distance
    );
  }
  
  return Contact();
}


Entity_ref
World::find_entity_by_name(const char *name) const
{
//  assert(m_impl && m_impl->world_instance_id);
//
//  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
//  assert(world);
//
//  Data::Entity_data *ent_data = world->entity;
//  assert(ent_data);
//  
//  Entity_ref return_ref;
//  
//  Data::data_lock(ent_data);
//  
//  size_t search_index = 0;
//  if(Common::fixed_string_search(name,
//                                 Data::entity_get_name_data(ent_data),
//                                 Data::entity_get_name_stride(),
//                                 Data::entity_get_size(ent_data),
//                                 &search_index))
//  {
//    return_ref = Entity_ref(
//                  Core_detail::entity_id_from_uint(
//                    ent_data->keys[search_index]));
//  }
//  
//  Data::data_unlock(ent_data);
//  
//  return return_ref;

  return Core::Entity_ref();
}


void
World::find_entities_by_ray(const Ray ray,
                            Contact **contacts,
                            size_t *out_array_size) const
{
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);

  LOG_TODO_ONCE("Find by ray");  
//  Physics_transform::find_entities_from_ray(
//    ray,
//    world->physics_world.dynamics_world,
//    contacts,
//    out_array_size);
}


void
World::find_entities_by_name(const char *name,
                             Entity_ref **out_array,
                             size_t *out_array_size)
{
//  static Entity_ref found_ents[1024];
//  static size_t count = 0;
//  
//  count = 0;
//  
//  assert(m_impl && m_impl->world_instance_id);
//  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
//  
//  // Loop through entity data and find entities.
//  auto data = world->entity;
//  
//  Data::data_lock(data);
//  
//  for(uint32_t i = 0; i < data->size; ++i)
//  {
//    const char *test_name;
//    Data::entity_get_name(data, data->keys[i], &test_name);
//    
//    const bool found = strcmp(name, test_name) == 0;
//
//    if(found)
//    {
//      found_ents[count] = Entity_ref(Core_detail::entity_id_from_uint(data->keys[i]));
//      count++;
//    }
//  }
//  
//  Data::data_unlock(data);
//  
//  *out_array = found_ents;
//  *out_array_size = count;
}


uint32_t
World::get_id() const
{
  assert(m_impl);
  
  return m_impl->world_instance_id;
}


std::shared_ptr<const Data::World>
World::get_world_data() const
{
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);

  return world;
}


std::shared_ptr<Data::World>
World::get_world_data()
{
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);

  return world;
}


} // ns
