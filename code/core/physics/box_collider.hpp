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
  Box_collider(const float x_half_extent,
               const float y_half_extent,
               const float z_half_extent)
  : m_x_half_extent(x_half_extent)
  , m_y_half_extent(y_half_extent)
  , m_z_half_extent(z_half_extent)
  {
  }
  
  float                 get_x_half_extent() const { return m_x_half_extent; }
  float                 get_y_half_extent() const { return m_y_half_extent; }
  float                 get_z_half_extent() const { return m_z_half_extent; }
  
  
                        operator Collider() const;
  
private:

  const float           m_x_half_extent;
  const float           m_y_half_extent;
  const float           m_z_half_extent;

};


} // ns


#endif // inc guard