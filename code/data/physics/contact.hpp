#ifndef CONTACT_DAA49F10_51B1_4BB0_899A_BEDF5B846D6A
#define CONTACT_DAA49F10_51B1_4BB0_899A_BEDF5B846D6A


#include <math/vec/vec_types.hpp>
#include <stdint.h>


namespace Data {
namespace Physics {


struct Contact
{
  math::vec3    contact_point;
  math::vec3    contact_normal;
  float         distance; // for collisions this is pen depth, for ray this is distance.
  uint64_t      collision_a_b;
};


struct Collision
{
};


} // ns
} // ns


#endif // inc guard
