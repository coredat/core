#ifndef CAMERA_PROPERTIES_INCLUDED_610DE4CF_5343_4840_9041_95568DA1B84B
#define CAMERA_PROPERTIES_INCLUDED_610DE4CF_5343_4840_9041_95568DA1B84B


#include <stdint.h>


namespace Camera {


enum class Type
{
  perspective,
}; // enum


// TODO: Reduce the size of this struct.
struct Camera_properties
{
  Type      type              = Type::perspective;
  
  uint32_t  viewport_width    = 0;
  uint32_t  viewport_height   = 0;
  
  float     fov               = 0.7855f; // 45 degrees
  float     near_plane        = 0.1f;
  float     far_plane         = 1000.f;
  
  bool      clear_depth       = true;
  bool      clear_color       = true;
}; // struct


} // ns


#endif // inc guard