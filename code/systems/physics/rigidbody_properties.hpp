#ifndef COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F
#define COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F


#include <stdint.h>


namespace Physics {


namespace Axis {


enum ENUM
{
  x = 1 << 0,
  y = 1 << 1,
  z = 1 << 2,
};


} // ns


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


// TODO: Make this struct smaller.
// Can merge axis
// Can reduce collider_type
// Might be able to remove a float from collider details.
struct Rigidbody_properties
{
  Collider_type collider_type = Collider_type::none;
  uint32_t      move_axis     = Axis::x | Axis::y | Axis::z;
  uint32_t      rotation_axis = Axis::x | Axis::y | Axis::z;
  float         mass          = 1;

  union
  {
    Collider::Unknown       unknown;
    Collider::Static_plane  static_plane;
    Collider::Cube          cube;
    Collider::Capsule       capsule;
  } collider_info;
};


} // ns


#endif // inc guard