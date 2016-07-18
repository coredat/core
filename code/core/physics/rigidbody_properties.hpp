#ifndef RIGIDBODY_PROPERTIES_2048F3C1_1E18_4460_BFB7_008D13C3994D
#define RIGIDBODY_PROPERTIES_2048F3C1_1E18_4460_BFB7_008D13C3994D


#include <stdint.h>


namespace Core {


class Rigidbody_properties
{
public:

  void          set_collision_mask(const uint32_t rb_id, const uint32_t rb_collides_with_ids);

  uint32_t      get_rb_id() const           { return m_this_id;  }
  uint32_t      get_rb_mask() const         { return m_mask_ids; }
  
  void          set_mass(const float mass)  { m_mass = mass; }
  float         get_mass() const            { return m_mass; }
  
private:

  uint32_t      m_mask_ids  = -1;
  uint32_t      m_this_id   = -1;
  float         m_mass      = 1.f;

};


} // ns


#endif // inc guard