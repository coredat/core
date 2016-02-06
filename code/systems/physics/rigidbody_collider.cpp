#include "rigidbody_collider.hpp"
#include "rigidbody.hpp"
#include <algorithm>
#include <assert.h>


namespace Physics {


void
colliders_generate(const Rigidbody_collider colliders[],
                   const size_t number_of_colliders,
                   Rigidbody out_rb_info[],
                   const size_t number_of_outs)
{
  const size_t number_to_process = std::min(number_of_colliders, number_of_outs);
  
  for(size_t i = 0; i < number_to_process; ++i)
  {
    const auto coll = colliders[i];
    auto out_rb = &out_rb_info[i];
  
    /*
      Create the collision shape.
    */
    switch(coll.collider_type)
    {
      case(Collider_type::static_plane):
      {
        const btVector3 normal(coll.collider_info.static_plane.normal[0],
                               coll.collider_info.static_plane.normal[1],
                               coll.collider_info.static_plane.normal[2]);
        const btScalar offset(coll.collider_info.static_plane.offset);
        
        out_rb->shape.reset(new btStaticPlaneShape(normal, offset));
        break;
      }
      
      case(Collider_type::cube):
      {
        const btVector3 extents(coll.collider_info.cube.extents[0],
                                coll.collider_info.cube.extents[1],
                                coll.collider_info.cube.extents[2]);

        out_rb->shape.reset(new btBoxShape(extents));
        break;
      }
      
      case(Collider_type::capsule):
      {
        const btScalar radius(coll.collider_info.capsule.radius);
        const btScalar height(coll.collider_info.capsule.height);
        
        out_rb->shape.reset(new btCapsuleShape(radius, height));
        break;
      }
      
      case(Collider_type::none):
        continue; // not a rigidbody
      
      default:
        assert(false); // unknown collider type.
        return;
    }
    
    out_rb->compound_shape.reset(new btCompoundShape());
  }
}



} // ns