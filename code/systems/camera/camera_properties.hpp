#ifndef CAMERA_PROPERTIES_INCLUDED_610DE4CF_5343_4840_9041_95568DA1B84B
#define CAMERA_PROPERTIES_INCLUDED_610DE4CF_5343_4840_9041_95568DA1B84B


#include <stdint.h>


namespace Camera {


enum class Type
{
  perspective,
}; // enum


struct Camera_propertiess
{
  Type      type             = Type::perspective;
  uint32_t  viewport_width   = 0;
  uint32_t  viewport_height  = 0;
  float     fov              = 45.f;
  float     near_plane       = 0.f;
  float     far_plane        = 1000.f;
}; // struct


} // ns


#endif // inc guard