#ifndef CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550
#define CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550


#include <core/entity_id.hpp>
#include <systems/camera/camera.hpp>
#include <stddef.h>


#define MAX_CAMERAS 10


namespace Data {


/*!
  Holding camera properties?
  
  TODO:
    add new camera should just generate the projection matrix, we don't need to keep rebuilding.
*/
struct Camera_pool
{
  Core::Entity_id             entity_id[MAX_CAMERAS];
  Camera::Camera_properties   camera[MAX_CAMERAS];
  uint32_t                    priority[MAX_CAMERAS];
  
  const size_t                number_of_cameras = MAX_CAMERAS;
};


/*!
  Initializers the camera pool list.
*/
void
camera_pool_init(Camera_pool *pool);


/*!
  Pushes a camera into the pool.
*/
void
camera_pool_add_camera(Camera_pool *pool,
                       const Core::Entity_id id,
                       const Camera::Camera_properties props);

/*!
  Set priority of the camera
  TODO: This is just a stub function.
  We need to set the render order of things currently we just set 1
*/
void
camera_pool_set_priority(Camera_pool *pool,
                         const Core::Entity_id id,
                         const uint32_t priority);

/*!
  Get the entity with priority.
*/
Camera::Camera_properties
camera_pool_get_properties_for_priority(Camera_pool *pool,
                                        const uint32_t priority);


Core::Entity_id
camera_pool_get_entity_id_for_priority(Camera_pool *pool,
                                       const uint32_t priority);


} // ns



#endif // inc guard