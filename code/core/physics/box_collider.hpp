#ifndef BOX_COLLIDER_INCLUDED_17A0E0D2_625F_4872_9487_2F43006A04C3
#define BOX_COLLIDER_INCLUDED_17A0E0D2_625F_4872_9487_2F43006A04C3


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