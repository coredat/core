#ifndef CAMERA_PROPERTIES_INCLUDED_610DE4CF_5343_4840_9041_95568DA1B84B
#define CAMERA_PROPERTIES_INCLUDED_610DE4CF_5343_4840_9041_95568DA1B84B


#include <stdint.h>
#include <core/camera/camera_properties.hpp>


namespace Camera {


// TODO: Reduce the size of this struct.
struct Camera_properties
{
  Core::Camera_type type = Core::Camera_type::perspective;
  
  uint32_t  viewport_width    = 0;
  uint32_t  viewport_height   = 0;
  
  float     fov               = 0.7855f; // 45 degrees
  float     near_plane        = 0.1f;
  float     far_plane         = 1000.f;
  
  uint16_t  clear_flags       = Core::Camera_clear::color | Core::Camera_clear::depth;
  uint32_t  clear_color       = 0;
}; // struct


} // ns


#endif // inc guard