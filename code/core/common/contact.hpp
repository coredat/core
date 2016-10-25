#ifndef CONTACT_INCLUDED_99B6022F_C765_4103_AA53_4B6BE90D0163
#define CONTACT_INCLUDED_99B6022F_C765_4103_AA53_4B6BE90D0163


#include <core/entity/entity_ref.hpp>
#include <math/vec/vec_types.hpp>


namespace Core {


class Contact
{
public:

  explicit        Contact();
  explicit        Contact(const Entity_ref contact_object,
                          const math::vec3 pos,
                          const math::vec3 norm,
                          const float pen_depth);

  Entity_ref      get_object() const;
  math::vec3      get_position() const;
  math::vec3      get_normal() const;
  float           get_penetration_depth() const;
  
                  operator bool() const;

private:

  Entity_ref  m_contact_object;
  math::vec3  m_position;
  math::vec3  m_normal;
  float       m_penetration_depth;

};


} // ns


#endif // inc guard