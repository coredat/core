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
  
  //math::aabb          get_model_aabb() const; // todo concat aabbs of the meshes together.
  
  uint32_t            get_number_of_meshes() const;
  Mesh                get_mesh(const uint32_t i) const;
  bool                set_mesh(const uint32_t i, const Mesh &mesh);
  
private:

  const uint32_t      m_model_id;
  
}; // class


} // ns


#endif // inc guard