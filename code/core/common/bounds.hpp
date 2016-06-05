#ifndef BOUNDS_INCLUDED_CAD9D322_AF6A_4578_9287_8AC13E2DCC5C
#define BOUNDS_INCLUDED_CAD9D322_AF6A_4578_9287_8AC13E2DCC5C


#include <math/vec/vec_types.hpp>


namespace Core {


/*!
  Represents a 3d region.
  This is an immutable object.
*/
class Bounds final
{
public:

  explicit                Bounds();
  explicit                Bounds(const math::vec3 min, const math::vec3 max); //! Constructor does no correction of min and max
  
  math::vec3              get_min() const; //! The minimum point of the bounds.
  math::vec3              get_max() const; //! The maximum point of the bounds.
  
private:

  const math::vec3        m_min;
  const math::vec3        m_max;

};


} // ns


#endif // inc guard