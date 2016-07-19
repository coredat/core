#ifndef RIGIDBODY_INCLUDED_824A6223_0FFD_4B0F_9E07_7E7C81B6618B
#define RIGIDBODY_INCLUDED_824A6223_0FFD_4B0F_9E07_7E7C81B6618B


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Rigidbody final
{
public:

  explicit      Rigidbody();
                ~Rigidbody();
  
                Rigidbody(const Rigidbody&);
  
  uint32_t      get_rb_id() const;
  uint32_t      get_rb_mask() const;
  void          set_collision_mask(const uint32_t rb_id, const uint32_t rb_collides_with_ids);
  
  Collider      get_collider() const;
  void          set_collider(const Collider &collider);
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard