#ifndef COLLISION_INCLUDED_439C50AF_1E6B_470F_A8D5_C95C16251E04
#define COLLISION_INCLUDED_439C50AF_1E6B_470F_A8D5_C95C16251E04


#include <core/common/core_types.hpp>
#include <core/common/core_fwd.hpp>
#include <core/entity/entity_ref.hpp>


namespace Core {


class Collision
{
public:

  explicit    Collision(const Core::Entity_ref ent_a,
                        const Core::Entity_ref ent_b,
                        const math::vec3 collision_norm,
                        const float pen_depth);
  
  Entity_ref  get_entity_a() const;
  Entity_ref  get_entity_b() const;
  
  math::vec3  get_collision_normal() const;
  float       get_penitration_depth() const;
  
private:

  Entity_ref  m_entity_a;
  Entity_ref  m_entity_b;
  math::vec3  m_collision_normal;
  float       m_penitation_depth;
  
};


} // ns


#endif // inc guard