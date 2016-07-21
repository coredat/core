#include <core/physics/collision.hpp>
#include <math/vec/vec3.hpp>


namespace Core {


Collision::Collision(const Core::Entity_ref ent_a,
                     const Core::Entity_ref ent_b,
                     const math::vec3 collision_norm,
                     const float pen_depth)
: m_entity_a(ent_a)
, m_entity_b(ent_b)
, m_collision_normal(collision_norm)
, m_penitation_depth(pen_depth)
{
}


Entity_ref
Collision::get_entity_a() const
{
  return m_entity_a;
}


Entity_ref
Collision::get_entity_b() const
{
  return m_entity_b;
}


math::vec3
Collision::get_collision_normal() const
{
  return m_collision_normal;
}


float
Collision::get_penitration_depth() const
{
  return m_penitation_depth;
}


} // ns