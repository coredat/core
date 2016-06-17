#ifndef CAM_UTILS_INCLUDED_24851027_2A53_4F2A_88FA_93B9B5FC7810
#define CAM_UTILS_INCLUDED_24851027_2A53_4F2A_88FA_93B9B5FC7810


#include <systems/camera/camera_properties.hpp>
#include <data/world_data/transform_data.hpp> // fwd dec
#include <core/transform/transform_fwd.hpp>
#include <math/mat/mat4.hpp>
#include <utilities/generic_id.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Camera_utils {


uint32_t
find_highest_priority(const uint32_t priorities[],
                      const uint32_t count);


size_t
find_insert_point_based_on_priority(const uint32_t priority,
                                    const uint32_t priorities[],
                                    const uint32_t count);


struct Cam_run
{
  math::mat4 view;
  math::mat4 proj;
  uint32_t width;
  uint32_t height;
  uint32_t clear_color;
  uint32_t clear_flags;
  uint32_t cull_mask;
};

/*
  Builds the data for each run for the camera.
  All arrays must be at least the size of count.
*/
void
calculate_camera_runs(const Camera::Camera_properties props[],
                      const Core::Transform transforms[],
                      Cam_run out_runs[],
                      const uint32_t count);


void
get_camera_transforms(const World_data::Transform_data *transform_data,
                      const util::generic_id entity_ids[],
                      Core::Transform out_transforms[],
                      const uint32_t size);


} // ns


#endif // inc guard