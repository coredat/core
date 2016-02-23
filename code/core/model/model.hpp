#ifndef MODEL_INCLUDED_292C333A_8EB1_40B3_A4AA_19E7E9D0447A
#define MODEL_INCLUDED_292C333A_8EB1_40B3_A4AA_19E7E9D0447A


#include "mesh.hpp"
#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


namespace Core {


class Model
{
public:

  explicit            Model(const uint32_t model_resource_id);
  
  math::aabb          get_model_aabb() const;
  
private:

  const uint32_t      m_model_id;
  
}; // class


} // ns


#endif // inc guard