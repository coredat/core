#ifndef RAY_INCLUDED_7498E040_552E_42FA_A700_40BFD4CE385A
#define RAY_INCLUDED_7498E040_552E_42FA_A700_40BFD4CE385A


#include <math/vec/vec_types.hpp>


namespace Core {
namespace Ray_utils {

constexpr float default_distance() { return 10000.f; };

} // ns


class Ray final
{
public:

  explicit          Ray();
  explicit          Ray(const math::vec3 origin,
                        const math::vec3 direction,
                        const float distance = Ray_utils::default_distance());
  
  math::vec3        get_origin() const;
  math::vec3        get_direction() const;
  float             get_distance() const;
  
private:

  const math::vec3  m_origin;
  const math::vec3  m_direction;
  const float       m_distance;

};


} // ns


#endif // inc guard