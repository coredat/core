#ifndef CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550
#define CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550


#include <core/entity_id.hpp>
#include <systems/camera/camera.hpp>
#include <stddef.h>


#define MAX_CAMERAS 1


namespace Data {


struct Camera_pool
{
  Core::Entity_id             entity_id[MAX_CAMERAS];
  Camera::Camera_propertiess  camera[MAX_CAMERAS];
  
  const size_t                number_of_cameras = MAX_CAMERAS;
};

  
} // ns



#endif // inc guard