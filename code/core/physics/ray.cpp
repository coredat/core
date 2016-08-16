#include <core/physics/ray.hpp>
#include <core/world/world.hpp>
#include <core/entity/entity_ref.hpp>
#include <3rdparty/qu3e/q3.h>
#include <data/world_data/world_pools.hpp>
#include <transformations/physics/q3_math_extensions.hpp>
#include <utilities/logging.hpp>


namespace Core {


struct Ray::Impl
{
  struct Ray_cast : public q3QueryCallback
  {
    bool ReportShape(q3Box *shape)
    {
      if(distance > ray_data.toi)
      {
        distance = ray_data.toi;
        auto parent = shape->body;
        auto user_data = util::generic_id_from_ptr(parent->GetUserData());
        
        hit = Entity_ref(user_data, std::const_pointer_cast<World_data::World>(data));
      }
      
      return hit;
    }
    
    std::shared_ptr<const World_data::World> data;
    Entity_ref hit;
    math::vec3 hit_pos = math::vec3_zero();
    q3RaycastData ray_data;
    float distance = FLT_MAX;
  };
  
  Ray_cast ray;
};


Ray::Ray(const Core::World &world,
         const math::vec3 from,
         const math::vec3 dir,
         const Ray_search search)
: m_impl(new Impl)
{
  m_impl->ray.data           = world.get_world_data();
  m_impl->ray.ray_data.start = math::vec3_to_q3vec(from);
  m_impl->ray.ray_data.dir   = math::vec3_to_q3vec(math::vec3_normalize(dir));
  m_impl->ray.ray_data.t     = FLT_MAX;
  m_impl->ray.ray_data.toi   = m_impl->ray.ray_data.t;
  
  world.get_world_data()->scene.RayCast(&m_impl->ray, m_impl->ray.ray_data);
}


Ray::~Ray()
{
}


Ray::Ray(const Ray &other)
: m_impl(new Impl)
{
  m_impl->ray = other.m_impl->ray;
}


bool
Ray::has_hit() const
{
  return (m_impl->ray.hit.is_valid());
}


Entity_ref
Ray::get_entity(const uint32_t i) const
{
  if(i == 0)
  {
    return m_impl->ray.hit;
  }
  
  return Entity_ref();
}


math::vec3
Ray::get_hit_position(const uint32_t i) const
{
  LOG_TODO_ONCE("Need to store the vectors, as the callback might get used multiple times.");

  if(i == 0)
  {
    const math::vec3 start      = math::vec3_from_q3vec(m_impl->ray.ray_data.start);
    const math::vec3 dir        = math::vec3_from_q3vec(m_impl->ray.ray_data.dir);
    const math::vec3 scaled_dir = math::vec3_scale(dir, m_impl->ray.distance);
    
    return math::vec3_add(start, scaled_dir);
  }
  
  return math::vec3_zero();
}


math::vec3
Ray::get_hit_normal(const uint32_t i) const
{
  if(i == 0)
  {
    return math::vec3_from_q3vec(m_impl->ray.ray_data.normal);
  }
  
  return math::vec3_zero_zero_one();
}


float
Ray::get_distance(const uint32_t i) const
{
  if(i == 0)
  {
    return m_impl->ray.distance;
  }
  
  return FLT_MAX;
}



} // ns