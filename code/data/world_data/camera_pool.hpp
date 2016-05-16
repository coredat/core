#ifndef CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550
#define CAMERA_POOL_INCLUDED_8872977A_D39E_4A73_915A_A0058E6B3550


#include <utilities/generic_id.hpp>
#include <systems/camera/camera_properties.hpp>
#include <stddef.h>


#define MAX_CAMERAS 10


namespace World_data {


/*!
  Holding camera properties?
  
  TODO:
    Add new camera should just generate the projection matrix, we don't need to keep rebuilding the same thing.
    Although that does raise questions over the networking stuff. We don't want to send the mats. too big.
  TODO:
    Smaller types for priority. Remember network structrue as well.
*/
struct Camera_pool
{
  util::generic_id            entity_id[MAX_CAMERAS];
  Camera::Camera_properties   camera[MAX_CAMERAS];
  uint32_t                    peer_priority_00[MAX_CAMERAS];
  uint32_t                    peer_priority_01[MAX_CAMERAS];
  uint32_t                    peer_priority_02[MAX_CAMERAS];
  uint32_t                    peer_priority_03[MAX_CAMERAS];
  
  const uint32_t              number_of_cameras = MAX_CAMERAS;
};


/*!
  Zeros the data in the pool.
  \param pool This is the pool to be initialized.
*/
void
camera_pool_init(Camera_pool *pool);


/*!
  Pushes a camera into the pool.
  \param pool The pool to push data into.
  \param id The id of the camera. This is so we can find the transform.
  \param props the properties of the transform.
*/
void
camera_pool_add_camera(Camera_pool *pool,
                       const util::generic_id id,
                       const Camera::Camera_properties props);

/*!
  Update a camera in the pool.
  \param pool The pool to push data into.
  \param id The id of the camera. This is so we can find the transform.
  \param props the properties of the transform.
*/
void
camera_pool_update_camera(Camera_pool *pool,
                          const util::generic_id id,
                          const Camera::Camera_properties props);

/*!
  Removes a camera from the pool.
  \param pool The pool to remove data from.
  \param id The id of the camera you wish to remove.
*/
void
camera_pool_remove_camera(Camera_pool *pool,
                          const util::generic_id id);

/*!
  Set priority of the camera
  We need to set the render order of things currently we just set 1.
  \param pool The camera pool we wish to update.
  \param id The id of the camera we wish to change priority.
  \param peer The network peer this camera priority should effect.
  \param priority The priority the camera should be.
*/
void
camera_pool_set_priority(Camera_pool *pool,
                         const util::generic_id id,
                         const uint32_t peer,
                         const uint32_t priority);

/*!
  Returns the priority of the camera.
  If the camera cannot be found it will return 0.
  \param pool The camera pool.
  \param id The id of the camera we wish to get.
  \param peer The network peer for which we want the result.
  \return The priority for that id, or zero if that id wasn't found.
*/
uint32_t
camera_pool_get_priority(Camera_pool *pool,
                         const util::generic_id id,
                         const uint32_t peer);

/*!
  Get the entity with priority.
  \param pool The camera data pool.
  \param peer The networkk peer we want the results for.
  \param priority The camera properties for this priority.
  \return a struct with the properties data.
*/
Camera::Camera_properties
camera_pool_get_properties_for_priority(Camera_pool *pool,
                                        const uint32_t peer,
                                        const uint32_t priority,
                                        const uint32_t default_vp_width = 800,
                                        const uint32_t default_vp_height = 480);


/*!
  Find an id that matches the priority.
  \param pool The pool to search.
  \param peer The network peer you wish to get.
  \param priority The priority you want.
  \return The entity_id. invalid_entity if none found.
*/
util::generic_id
camera_pool_get_entity_id_for_priority(Camera_pool *pool,
                                       const uint32_t peer,
                                       const uint32_t priority);


} // ns



#endif // inc guard