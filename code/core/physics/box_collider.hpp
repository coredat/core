#ifndef BOX_COLLIDER_INCLUDED_17A0E0D2_625F_4872_9487_2F43006A04C3
#define BOX_COLLIDER_INCLUDED_17A0E0D2_625F_4872_9487_2F43006A04C3


#include <math/vec/vec3.hpp>


namespace Core {


/*
  Box Collider.
*/
class Box_collider
{
public:

  /*!
    Constructor takes the full extent of the box.
  */
  constexpr explicit
  Box_collider(const float x_extent,
               const float y_extent,
               const float z_extent)
  : m_x_extent(x_extent)
  , m_y_extent(y_extent)
  , m_z_extent(z_extent)
  {
  }
  
  
  /* constexpr */ explicit
  Box_collider(const math::vec3 extents)
  : m_x_extent(math::vec3_get_x(extents))
  , m_y_extent(math::vec3_get_y(extents))
  , m_z_extent(math::vec3_get_z(extents))
  {
  }
  
  float                 get_x_extent() const { return m_x_extent; }
  float                 get_y_extent() const { return m_y_extent; }
  float                 get_z_extent() const { return m_z_extent; }
  
private:

  const float           m_x_extent;
  const float           m_y_extent;
  const float           m_z_extent;

};


} // ns


#endif // inc guard