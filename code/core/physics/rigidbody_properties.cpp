#include <core/physics/rigidbody_properties.hpp>


namespace Core {


void
Rigidbody_properties::set_collision_mask(const uint32_t rb_id, const uint32_t rb_collides_with_ids)
{
  m_this_id = rb_id;
  m_mask_ids = rb_collides_with_ids;
}


} // ns