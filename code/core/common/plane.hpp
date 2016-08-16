#ifndef PLANE_INCLUDED_9D69E57C_2EAE_4F34_9652_29A3A0A66E89
#define PLANE_INCLUDED_9D69E57C_2EAE_4F34_9652_29A3A0A66E89


#include <math/vec/vec_types.hpp>


namespace Core {


class Plane
{
public:

  explicit      Plane(const math::vec3 position, const math::vec3 normal);
  
  math::vec3    get_position() const;
  math::vec3    get_normal() const;
  
private:

  const math::vec3 m_position;
  const math::vec3 m_normal;

};


} // ns


#endif // inc guard