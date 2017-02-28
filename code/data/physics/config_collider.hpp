#ifndef COLLIDER_DATA_INCLUDED_57054271_0A16_4F9C_B1C2_E40783610994
#define COLLIDER_DATA_INCLUDED_57054271_0A16_4F9C_B1C2_E40783610994


namespace Data {
namespace Physics {


enum class Collider_type
{
  unknown,
  box,
};


struct Collider_config
{
  Collider_type type;
  float args[4];
};


} // ns
} // ns


#endif // inc guard
