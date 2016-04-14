#include <core/camera/camera.hpp>
#include <core/color/color.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/camera_pool.hpp>
#include <core/camera/camera_properties.hpp>
#include <systems/camera/camera_properties.hpp>


namespace Core {
  
  
struct Camera::Impl
{
  Core::Entity_id attached_entity = Core::Entity_id_util::invalid_id();
  ::Camera::Camera_properties properties;
};


Camera::Camera()
: m_impl(new Impl)
{
}


Camera::~Camera()
{
  set_attached_entity(Entity_ref()); // Passing an invalid entity will unload the current entity.
}


Camera::Camera(Camera &&other)
: m_impl(new Impl{
  other.m_impl->attached_entity,
  other.m_impl->properties
})
{
  // Null the other stuff
  other.m_impl->attached_entity = Core::Entity_id_util::invalid_id();
  other.m_impl->properties = ::Camera::Camera_properties();
}


Camera&
Camera::operator=(Camera &&other)
{
  // Copy other.
  m_impl->attached_entity = other.m_impl->attached_entity;
  m_impl->properties = other.m_impl->properties;

  // Null the other stuff
  other.m_impl->attached_entity = Core::Entity_id_util::invalid_id();
  other.m_impl->properties = ::Camera::Camera_properties();

  return *this;
}


namespace
{
  void
  update_properties(::Camera::Camera_properties props,
                    Core::Entity_id id)
  {
    World_data::World* world = World_data::get_world();
    World_data::camera_pool_update_camera(world->camera_pool, id, props);
  }
}



void
Camera::set_attached_entity(const Entity_ref entity)
{
  World_data::World *world = World_data::get_world();

  // If we are retargeting this to another transform.
  // Then we must remove the old one.
  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    World_data::camera_pool_remove_camera(world->camera_pool, m_impl->attached_entity);
  }

  m_impl->attached_entity = entity.get_id();
  
  World_data::camera_pool_add_camera(world->camera_pool, m_impl->attached_entity, m_impl->properties);
}


Entity_ref
Camera::get_attached_entity() const
{
  World_data::World* world = World_data::get_world();
  
  if(!world)
  {
    return Entity_ref();
  }
  
  Entity_ref entity;
  World_data::world_find_entity(world, &entity, m_impl->attached_entity);
  
  return entity;
}


void
Camera::set_priority(const uint32_t priority)
{
  set_peer_priority(0, priority);
}


uint32_t
Camera::get_priority() const
{
  return get_peer_priority(0);
}


void
Camera::set_peer_priority(const uint32_t peer, const uint32_t priority)
{
  World_data::World* world = World_data::get_world();
  World_data::camera_pool_set_priority(world->camera_pool, m_impl->attached_entity, peer, priority);
}


uint32_t
Camera::get_peer_priority(const uint32_t peer) const
{
  World_data::World* world = World_data::get_world();
  return World_data::camera_pool_get_priority(world->camera_pool, m_impl->attached_entity, peer);
}


void
Camera::set_type(const Core::Camera_type cam_type)
{
  m_impl->properties.type = cam_type;

  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


Camera_type
Camera::get_type() const
{
  return m_impl->properties.type;
}


void
Camera::set_clear_flags(const uint32_t flags)
{
  m_impl->properties.clear_flags = flags;
  
  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


bool
Camera::will_clear_color() const
{
  return m_impl->properties.clear_flags & Camera_clear::color;
}


bool
Camera::will_clear_depth() const
{
  return m_impl->properties.clear_flags & Camera_clear::depth;
}


uint32_t
Camera::get_clear_flags() const
{
  return m_impl->properties.clear_flags;
}


void
Camera::set_width(const uint32_t width)
{
  m_impl->properties.viewport_width = width;

  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


uint32_t
Camera::get_width() const
{
  return m_impl->properties.viewport_width;
}


void
Camera::set_height(const uint32_t height)
{
  m_impl->properties.viewport_height = height;

  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


uint32_t
Camera::get_height() const
{
  return m_impl->properties.viewport_height;
}


void
Camera::set_feild_of_view(const float fov_radians)
{
  m_impl->properties.fov = fov_radians;

  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


float
Camera::get_field_of_view() const
{
  return m_impl->properties.fov;
}


void
Camera::set_near_plane(const float near_plane)
{
  m_impl->properties.near_plane = near_plane;

  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


float
Camera::get_near_plane() const
{
  return m_impl->properties.near_plane;
}


void
Camera::set_far_plane(const float far_plane)
{
  m_impl->properties.far_plane = far_plane;

  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


float
Camera::get_far_plane() const
{
  return m_impl->properties.far_plane;
}


void
Camera::set_clear_color(const Core::Color color)
{
  m_impl->properties.clear_color = color.get_color();

  if(m_impl->attached_entity != Core::Entity_id_util::invalid_id())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


Color
Camera::get_clear_color() const
{
  return Color(m_impl->properties.clear_color);
}


}; // ns