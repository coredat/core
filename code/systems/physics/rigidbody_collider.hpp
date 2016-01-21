#ifndef RIGIDBODY_COLLIDER_INCLUDED_0DC9ABFE_397E_4BE9_A80C_82E19EAB9EDC
#define RIGIDBODY_COLLIDER_INCLUDED_0DC9ABFE_397E_4BE9_A80C_82E19EAB9EDC


#include "physics_fwd.hpp"
#include <cstddef>


namespace Physics {


enum class Collider_type
{
  none,
  static_plane,
  cube,
  capsule,
};


namespace Collider {


struct Unknown
{
  float args[4];
};

struct Static_plane
{
  float normal[3];
  float offset;
};

struct Cube
{
  float extents[3];
  float not_used;
};

struct Capsule
{
  float radius;
  float height;
  float not_used[2];
};


} // ns


struct Rigidbody_collider
{
  Collider_type collider_type = Collider_type::none;

  union
  {
    Collider::Unknown       unknown;
    Collider::Static_plane  static_plane;
    Collider::Cube          cube;
    Collider::Capsule       capsule;
  } collider_info;
};



/*
  Generates the underlying collider structures for a given set of properties.
*/
void
colliders_generate(const Rigidbody_collider colliders[],
                   const std::size_t number_of_colliders,
                   Rigidbody out_rb_info[],
                   const std::size_t number_of_outs);


} // ns


#endif // inc guard