#ifndef RIGIDBODY_PROPERTIES_
#define RIGIDBODY_PROPERTIES_


#include <stdint.h>


namespace Core {


class Rigidbody_properties
{
public:

  void          set_collision_mask(const uint32_t rb_id, const uint32_t rb_collides_with_ids);

  uint32_t      get_rb_id() const     { return m_this_id; }
  uint32_t      get_rb_mask() const   { return m_mask_ids; }

private:

  uint32_t      m_mask_ids = -1;
  uint32_t      m_this_id  = -1;

};


} // ns


#endif // inc guard