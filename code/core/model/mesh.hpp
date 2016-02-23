#ifndef MESH_INCLUDED_3B02E00E_C938_4A7E_93EE_52E659913C99
#define MESH_INCLUDED_3B02E00E_C938_4A7E_93EE_52E659913C99


#include <math/geometry/geometry_types.hpp>


namespace Core {


class Mesh
{
public:

  // Need to figure out how to store what data.
  // ie I only need to store data for meshes that are editable. ie a navmesh.
  
  math::aabb          get_aabb() const;

}; // class


} // ns



#endif // inc guard