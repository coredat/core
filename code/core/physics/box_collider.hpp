#ifndef BOX_COLLIDER_INCLUDED_17A0E0D2_625F_4872_9487_2F43006A04C3
#define BOX_COLLIDER_INCLUDED_17A0E0D2_625F_4872_9487_2F43006A04C3


#include <core/physics/physics_fwd.hpp>


namespace Core {


/*
  Box Collider.
*/
class Box_collider final
{
public:

  /*!
    Constructor takes the half extent of the box.
  */
  constexpr explicit
  Box_collider(const float x_extent,
               const float y_extent,
               const float z_extent)
  : m_x_extents(x_extent)
  , m_y_extents(y_extent)
  , m_z_extents(z_extent)
  {
  }
  
  float                 get_x_extent() const { return m_x_extents; }
  float                 get_y_extent() const { return m_y_extents; }
  float                 get_z_extent() const { return m_z_extents; }
  
                        operator Collider() const;
  
private:

  const float           m_x_extents;
  const float           m_y_extents;
  const float           m_z_extents;

};


} // ns


#endif // inc guard