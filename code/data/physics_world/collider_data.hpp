#ifndef COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F
#define COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F


namespace Data {


enum class Collider_type
{
  unknown,
  static_plane,
  cube,
};


namespace Collider
{
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
} // ns


struct Collider_detail
{
  Collider_type type;
  float mass;
  
  union
  {
    Collider::Unknown           unknown;
    Collider::Static_plane      static_plane;
    Collider::Cube              cube;
  } collider_info;
};


} // ns


#endif // inc guard