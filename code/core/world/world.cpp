#include <core/world/world.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <core/physics/collision.hpp>
#include <core/common/ray.hpp>
#include <data/world/entity_data.hpp>
#include <data/context_data/input_pool.hpp>
#include <core/context/detail/context_data.hpp>
#include <transformations/physics/q3_math_extensions.hpp>
#include <systems/engine/engine.hpp>
#include <systems/engine/tick_information.hpp>
#include <debug_gui/debug_menu.hpp>
#include <utilities/timer.hpp>
#include <utilities/logging.hpp>


namespace Core {


struct World::Impl
{
  uint32_t world_instance_id;
  
  Core::Context *context = nullptr;
  util::timer dt_timer;
  float       dt           = 0.f;
  float       dt_mul       = 1.f;
  float       running_time = 0.f;
  Collision_callback collision_callback = nullptr;
};


World::World(Context &ctx, const World_setup setup)
: m_impl(new World::Impl)
{
  LOG_TODO_ONCE("World should be 'moveable'");
  
  m_impl->world_instance_id = Core_detail::world_index_add_world_data(setup.entity_pool_size);
  
//  m_impl->world_data = std::make_shared<World_data::World>(setup.entity_pool_size);
  m_impl->context = &ctx;
  
  m_impl->dt_timer.start();
  
  Engine::initialize();
}


World::~World()
{
  Core_detail::world_index_release_world_data(m_impl->world_instance_id);
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
    const util::milliseconds frame_time = m_impl->dt_timer.split();
    m_impl->dt = static_cast<float>(frame_time) / 1000.f;
    
    m_impl->running_time += m_impl->dt;
  }

  // Engine Think
  Engine::Tick_information tick_info;
  {
    auto resources = Resource_data::get_resource_data();
    auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);

    Engine::think(world,
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
    Debug_menu::display_world_data_menu(world.get(),
                                        m_impl->dt,
                                        m_impl->dt_mul,
                                        world->scene->GetBodyCount(),
                                        tick_info.number_of_draw_calls,
                                        tick_info.camera_runs);
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
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);

  return world->entity->size;
}


Entity_ref
World::find_entity_by_id(const util::generic_id id) const
{
  return Entity_ref(Core_detail::entity_id_from_uint(id));
}


void
World::find_entities_by_tag(const uint32_t tag_id,
                            Entity_ref **out_array,
                            size_t *out_array_size)
{
  static Entity_ref found_ents[1024];
  static size_t count = 0;
  
  count = 0;
  
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
  
  // Loop through entity data and find entities.
  auto data = world->entity;
  
  data_lock(data);
  
  for(uint32_t i = 0; i < data->size; ++i)
  {
    if(Data::entity_get_tags_data(data)[i])
    {
      found_ents[count] = Entity_ref(Core_detail::entity_id_from_uint(data->keys[i]));
      count++;
    }
  }
  
  data_unlock(data);
  
  *out_array = found_ents;
  *out_array_size = count;
}


Collision
World::find_entity_by_ray(const Ray ray) const
{
  struct Raycast : public q3QueryCallback
  {
    bool ReportShape(q3Box *shape)
    {
      if(distance > ray_data.toi)
      {
        distance = ray_data.toi;
        auto parent = shape->body;
        auto user_data = util::generic_id_from_ptr(parent->GetUserData());
        
        hit_entity = Entity_ref(Core_detail::entity_id_from_uint(user_data));
        hit_normal = math::vec3_from_q3vec(ray_data.normal);
        hit_pos    = math::vec3_from_q3vec(ray_data.GetImpactPoint());
      }
      
      return hit_entity;
    }
    
    std::shared_ptr<const Data::World> data;
    Entity_ref hit_entity = Entity_ref();
    math::vec3 hit_pos    = math::vec3_zero();
    math::vec3 hit_normal = math::vec3_zero();
    q3RaycastData ray_data;
    float distance = FLT_MAX;
  };
  
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
  
  Raycast raycast;
  
  raycast.data           = get_world_data();
  raycast.ray_data.start = math::vec3_to_q3vec(ray.get_origin());
  raycast.ray_data.dir   = math::vec3_to_q3vec(math::vec3_normalize(ray.get_direction()));
  raycast.ray_data.t     = FLT_MAX;
  raycast.ray_data.toi   = raycast.ray_data.t;
  
  world->scene->RayCast(&raycast, raycast.ray_data);
  
  const Contact contact(raycast.hit_pos, raycast.hit_normal, 0.f);
  
  return Collision(raycast.hit_entity, &contact, 1);
}


Entity_ref
World::find_entity_by_name(const char *name) const
{
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);

  auto data = world->entity;
  
  data_lock(data);
  
  auto
  entity_data_search_property_name = [](const Data::Entity_data *data, const char *value, util::generic_id *out_key) -> bool
  {
    LOG_TODO_ONCE("This is a hack solve it.");
    bool found = false;

    for(size_t i = 0; i < data->size; ++i)
    {
      if(!strcmp(value, &data->field_name[i * 32]))
      {
        found = true;

        if(out_key)
        {
          *out_key = data->keys[i];
        }

        break;
      }
    }

    return found;
  };
  
  util::generic_id id;
  const bool found = entity_data_search_property_name(data, name, &id);
  
  data_unlock(data);
  
  if(found)
  {
    return Entity_ref(Core_detail::entity_id_from_uint(id));
  }
  
  return Entity_ref();
}


void
World::find_entities_by_name(const char *name,
                             Entity_ref **out_array,
                             size_t *out_array_size)
{
  static Entity_ref found_ents[1024];
  static size_t count = 0;
  
  count = 0;
  
  assert(m_impl && m_impl->world_instance_id);
  auto world = Core_detail::world_index_get_world_data(m_impl->world_instance_id);
  
  // Loop through entity data and find entities.
  auto data = world->entity;
  
  data_lock(data);
  
  auto
  entity_data_search_property_name = [](const Data::Entity_data *data, const char *value, util::generic_id *out_key) -> bool
  {
    LOG_TODO_ONCE("This is a hack solve it.");
    bool found = false;

    for(size_t i = 0; i < data->size; ++i)
    {
      if(!strcmp(value, &data->field_name[i * 32]))
      {
        found = true;

        if(out_key)
        {
          *out_key = data->keys[i];
        }

        break;
      }
    }

    return found;
  };
  
  for(uint32_t i = 0; i < data->size; ++i)
  {
    const bool found = entity_data_search_property_name(data, name, &data->keys[i]);

    if(found)
    {
      found_ents[count] = Entity_ref(Core_detail::entity_id_from_uint(data->keys[i]));
      count++;
    }
  }
  
  data_unlock(data);
  
  *out_array = found_ents;
  *out_array_size = count;
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