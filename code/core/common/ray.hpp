#ifndef RAY_INCLUDED_7498E040_552E_42FA_A700_40BFD4CE385A
#define RAY_INCLUDED_7498E040_552E_42FA_A700_40BFD4CE385A


#include <math/vec/vec_types.hpp>


namespace Core {


class Ray final
{
public:

  explicit          Ray(const math::vec3 origin, const math::vec3 direction);
  
  math::vec3        get_origin() const;
  math::vec3        get_direction() const;
  
private:

  const math::vec3  m_origin;
  const math::vec3  m_direction;

};


} // ns


#endif // inc guard