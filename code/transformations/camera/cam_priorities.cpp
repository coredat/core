#include <transformations/camera/cam_priorities.hpp>
#include <core/transform/transform.hpp>
#include <math/transform/transform.hpp>


namespace Camera_utils {


uint32_t
find_highest_priority(const uint32_t priorities[],
                      const uint32_t count)
{
  uint32_t result = 0;
  
  for(uint32_t i = 0; i < count; ++i)
  {
    if(result > priorities[0])
    {
      result = 0;
    }
  }
  
  return result;
}


size_t
find_insert_point_based_on_priority(const uint32_t priority,
                                    const uint32_t priorities[],
                                    const uint32_t count)
{
  for(uint32_t i = 0; i < count; ++i)
  {
    if(priority < priorities[0])
    {
      return (size_t)i;
    }
  }
  
  return 0;
}


void
calculate_camera_runs(const Camera::Camera_properties props[],
                      const Core::Transform transforms[],
                      Cam_run out_runs[],
                      const uint32_t count)
{
  // For props
  for(uint32_t i = 0; i < count; ++i)
  {
    const Camera::Camera_properties *prop = &props[i];
  
    if(prop->type == Core::Camera_type::perspective)
    {
      out_runs[i].proj = math::mat4_projection(prop->viewport_width,
                                               prop->viewport_height,
                                               prop->near_plane,
                                               prop->far_plane,
                                               prop->fov);
    }
    else
    {
      out_runs[i].proj = math::mat4_orthographic(prop->viewport_width,
                                                 prop->viewport_height,
                                                 prop->near_plane,
                                                 prop->far_plane);
    }
    
    
    out_runs[i].near_plane  = props->near_plane;
    out_runs[i].far_plane   = props->far_plane;
    out_runs[i].clear_color = props->clear_color;
    out_runs[i].clear_flags = props->clear_flags;
  }
  
  // View matrix
  for(uint32_t i = 0; i < count; ++i)
  {
    const Core::Transform *trans = &transforms[i];
  
    out_runs[i].view = math::mat4_lookat(trans->get_position(),
                                         math::vec3_add(trans->get_position(), trans->get_forward()),
                                         trans->get_up());
  }
}


void
get_camera_transforms(const World_data::Transform_data *transform_data,
                      const util::generic_id entity_ids[],
                      Core::Transform out_transforms[],
                      const uint32_t size)
{
  World_data::data_lock(transform_data);

  for(uint32_t i = 0; i < size; ++i)
  {
    // Get the transform entity and
    math::transform trans;
    World_data::transform_data_get_property_transform(transform_data, entity_ids[i], &trans);
    
    out_transforms[i] = Core::Transform(trans.position, trans.scale, trans.rotation);
  }
  
  World_data::data_unlock(transform_data);
}


} // ns