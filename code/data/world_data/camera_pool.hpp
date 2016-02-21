#ifndef CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550
#define CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550


#include <core/entity_id.hpp>
#include <systems/camera/camera_properties.hpp>
#include <stddef.h>


#define MAX_CAMERAS 10


namespace World_data {


/*!
  Holding camera properties?
  
  TODO:
    add new camera should just generate the projection matrix, we don't need to keep rebuilding.
*/
struct Camera_pool
{
  Core::Entity_id             entity_id[MAX_CAMERAS];
  Camera::Camera_properties   camera[MAX_CAMERAS];
  uint32_t                    peer_priority_00[MAX_CAMERAS];
  uint32_t                    peer_priority_01[MAX_CAMERAS];
  uint32_t                    peer_priority_02[MAX_CAMERAS];
  uint32_t                    peer_priority_03[MAX_CAMERAS];
  
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
  Removes a camera from the pool.
*/
void
camera_pool_remove_camera(Camera_pool *pool,
                          const Core::Entity_id id);

/*!
  Set priority of the camera
  TODO: This is just a stub function.
  We need to set the render order of things currently we just set 1
*/
void
camera_pool_set_priority(Camera_pool *pool,
                         const Core::Entity_id id,
                         const uint32_t peer,
                         const uint32_t priority);

/*!
  Returns the priority of the camera.
  If the camera cannot be found it will return 0.
*/
uint32_t
camera_pool_get_priority(Camera_pool *pool,
                         const Core::Entity_id id,
                         const uint32_t peer);

/*!
  Get the entity with priority.
*/
Camera::Camera_properties
camera_pool_get_properties_for_priority(Camera_pool *pool,
                                        const uint32_t priority);


Core::Entity_id
camera_pool_get_entity_id_for_priority(Camera_pool *pool,
                                       const size_t peer,
                                       const uint32_t priority);


} // ns



#endif // inc guard