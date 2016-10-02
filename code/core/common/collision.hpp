#ifndef COLLISION_INCLUDED_EC62C163_8A3A_4D7C_98C9_54F303F02751
#define COLLISION_INCLUDED_EC62C163_8A3A_4D7C_98C9_54F303F02751


#include <core/entity/entity_ref.hpp>
#include <math/vec/vec_types.hpp>


namespace Core {


class Collision
{
public:

  explicit        Collision(const Entity_ref ref, const math::vec3 pos, const math::vec3 norm);
  
  Entity_ref      get_entity() const;
  math::vec3      get_position() const;
  math::vec3      get_normal() const;
  
  bool            has_hit() const;
                  operator bool() const;

private:

  const Entity_ref  m_object;
  const math::vec3  m_position;
  const math::vec3  m_normal;
};


} // ns


#endif // inc guard