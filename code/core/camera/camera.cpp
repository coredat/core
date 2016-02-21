#include "camera.hpp"
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/camera_pool.hpp>


namespace Core {


Camera::Camera()
{
}


Camera::~Camera()
{
  set_attached_entity(Entity()); // Passing an invalid entity will unload the current entity.
}


void
Camera::set_attached_entity(const Entity &entity)
{
  World_data::World *world = World_data::get_world();

  // If we are retargeting this to another transform.
  // Then we must remove the old one.
  if(m_attached_entity != Core::Entity_id_util::invalid_id())
  {
    World_data::camera_pool_remove_camera(world->camera_pool, m_attached_entity);
  }

  m_attached_entity = entity.get_id();
  
  World_data::camera_pool_add_camera(world->camera_pool, m_attached_entity, ::Camera::Camera_properties());
}


Entity
Camera::get_attached_entity() const
{
  World_data::World* world = World_data::get_world();
  
  if(!world)
  {
    return Entity();
  }
  
  Entity entity;
  World_data::world_find_entity(world, &entity, m_attached_entity);
  
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
  World_data::camera_pool_set_priority(world->camera_pool, m_attached_entity, peer, priority);
}


uint32_t
Camera::get_peer_priority(const uint32_t peer) const
{
  World_data::World* world = World_data::get_world();
  return World_data::camera_pool_get_priority(world->camera_pool, m_attached_entity, peer);
}


void
Camera::set_type(const Core::Camera_type cam_type)
{
  
}


Camera_type
Camera::get_type() const
{
  return Camera_type::perspective;
}


void
Camera::set_clear_flags(const uint32_t flags)
{
}


uint32_t
Camera::get_clear_flags() const
{
  return 0;
}


void
Camera::set_width(const uint32_t width)
{
}


uint32_t
Camera::get_width() const
{
  return 0;
}


void
Camera::set_height(const uint32_t height)
{
}


uint32_t
Camera::get_height() const
{
  return 0;
}


void
Camera::set_feild_of_view(const float fov_radians)
{
}


float
Camera::get_field_of_view() const
{
  return 0;
}


void
Camera::set_near_plane(const float near_plane)
{
}


float
Camera::get_near_plane() const
{
  return 0.f;
}


void
Camera::set_far_plane(const float far_plane)
{
}


float
Camera::get_far_plane() const
{
  return 0.f;
}


void
Camera::set_clear_color(const float r, const float g, const float b)
{

}


}; // ns