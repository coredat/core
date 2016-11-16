#ifndef RIGIDBODY_INCLUDED_824A6223_0FFD_4B0F_9E07_7E7C81B6618B
#define RIGIDBODY_INCLUDED_824A6223_0FFD_4B0F_9E07_7E7C81B6618B


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <core/physics/collider.hpp>


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
  
  bool          is_kinematic() const;
  void          set_is_kinematic(const bool set);
  
  bool          is_trigger() const;
  void          set_is_trigger(const bool set);
  
  Collider      get_collider() const;
  void          set_collider(const Collider &collider);
  
  float         get_mass() const;
  void          set_mass(const float mass);
      
private:

  Collider m_collider;
  
  bool m_is_dynamic = true;
  bool m_is_trigger = false;
  
  uint32_t m_collision_id = -1;
  uint32_t m_mask_id = -1; // combine these two please
  
  float m_mass = 1;
};


} // ns


#endif // inc guard