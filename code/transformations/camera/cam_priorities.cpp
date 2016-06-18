#include <transformations/camera/cam_priorities.hpp>
#include <core/transform/transform.hpp>
#include <math/transform/transform.hpp>


namespace Camera_utils {


uint32_t
find_highest_priority(const uint32_t priorities[],
                      const uint32_t count)
{
//  uint32_t result = (uint32_t)-1;
//  
//  for(uint32_t i = 0; i < count; ++i)
//  {
//    if(priorities[i] < result)
//    {
//      result = priorities[i];
//    }
//  }
//  
//  return result;
  return 0;
}


size_t
find_insert_point_based_on_priority(const uint32_t priority,
                                    const uint32_t priorities[],
                                    const uint32_t count)
{
  for(uint32_t i = 0; i < count; ++i)
  {
    if(priority > priorities[i])
    {
      return i;
    }
  }
  
  return count - 1;
}


void
calculate_camera_runs(const World_data::Camera_data *cam_data,
                      const Resource_data::Texture_data *tex_data,
                      const Core::Transform transforms[],
                      Cam_run out_runs[],
                      const uint32_t count)
{
  // Get the camera targets.
  for(uint32_t i = 0; i < count; ++i)
  {
    const auto tex_id = cam_data->property_texture_id[i];
    
    if(tex_id)
    {
      Resource_data::texture_data_get_property_render_target(tex_data, tex_id, &out_runs[i].fbo);
    }
  }

  // Get the properties.
  for(uint32_t i = 0; i < count; ++i)
  {
    const Camera::Camera_properties *prop = &cam_data->property_camera[i];
  
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
    
    
    out_runs[i].width       = prop->viewport_width;
    out_runs[i].height      = prop->viewport_height;
    out_runs[i].clear_color = prop->clear_color;
    out_runs[i].clear_flags = prop->clear_flags;
    out_runs[i].cull_mask   = prop->cull_mask;
  }
  
  // View matrix
  for(uint32_t i = 0; i < count; ++i)
  {
    const Core::Transform *trans = &transforms[i];
  
    out_runs[i].view = math::mat4_lookat(trans->get_position(),
                                         math::vec3_add(trans->get_position(), trans->get_forward()),
                                         trans->get_up());
  }
  
  // Post process id
  for(uint32_t i = 0; i < count; ++i)
  {
    out_runs[i].post_process_id = cam_data->property_post_process_id[i];
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