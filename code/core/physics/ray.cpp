#include <core/physics/ray.hpp>
#include <core/world/world.hpp>
#include <core/entity/entity_ref.hpp>
#include <3rdparty/qu3e/q3.h>
#include <data/world_data/world_pools.hpp>


namespace Core {


struct Ray::Impl
{
  struct Ray_cast : public q3QueryCallback
  {
    bool ReportShape(q3Box *shape)
    {
      auto parent = shape->body;
      auto user_data = util::generic_id_from_ptr(parent->GetUserData());
      
      hit = Entity_ref(user_data, data);
      
      return user_data != util::generic_id_invalid();
    }
    
    std::shared_ptr<World_data::World> data;
    Entity_ref hit;
  };
  
  Ray_cast ray;
};


Ray::Ray(Core::World &world,
         const math::vec3 from,
         const math::vec3 to,
         const Search search)
: m_impl(new Impl)
{
  m_impl->ray.data = world.get_world_data();

  q3RaycastData ray_data;
  ray_data.start = q3Vec3(0,-10,0);
  ray_data.dir = q3Vec3(0, 1, 0);
  ray_data.t = r32(100000.0);
  ray_data.toi = ray_data.t;
  
  world.get_world_data()->scene.RayCast(&m_impl->ray, ray_data);
}


Ray::~Ray()
{
}


Entity_ref
Ray::get_entity(const uint32_t i) const
{
  return m_impl->ray.hit;
}


bool
Ray::has_hit() const
{
  return (m_impl->ray.hit);
}


} // ns