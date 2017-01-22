#include <transformations/entity/entity_camera.hpp>
#include <core/color/color.hpp>
//#include <data/world/entity_data.hpp>
#include <data/world/camera_data.hpp>
#include <common/data_types.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace Entity_detail {


void
set_camera(util::generic_id const this_id,
           Data::Entity_data *entity_data,
           Data::Camera_data *camera_data,
           Core::Camera const *camera)
{
  assert(this_id);
  assert(entity_data);
  assert(camera_data);
  assert(camera);

  // Check if Entity has camera data. If not add it.
  {
    Data::data_lock(entity_data);
    
    uint32_t current_data_types(0);
    Data::entity_get_components(entity_data, this_id, &current_data_types);
    
    if(!Common::Data_type::has_data_type(current_data_types, Common::Data_type::camera))
    {
      current_data_types |= Common::Data_type::camera;
      Data::entity_set_components(entity_data, this_id, &current_data_types);
    }
    
    Data::data_unlock(entity_data);
  }

  // Add entity properties
  {
    Data::data_lock(camera_data);
    
    // Insert camera if none exists for this entity.
    if(!Data::camera_exists(camera_data, this_id))
    {
      Data::camera_push(camera_data, this_id);
    }
    
    // Set data
    {
      Camera_util::Camera_properties properties;
      Data::camera_get_properties(camera_data, this_id, &properties);
      
      properties.clear_color     = camera->get_clear_color().get_color();
      properties.clear_flags     = camera->get_clear_flags();
      properties.cull_mask       = camera->get_tags_to_render();
      properties.far_plane       = camera->get_far_plane();
      properties.fov             = camera->get_field_of_view();
      properties.near_plane      = camera->get_near_plane();
      properties.type            = camera->get_type();
      properties.viewport_height = camera->get_height();
      properties.viewport_width  = camera->get_width();
      
      Data::camera_set_properties(camera_data, this_id, &properties);
      
      const uint32_t priority(camera->get_priority());
      Data::camera_set_priority(camera_data, this_id, &priority);
      
      const uint32_t zero(0);
      Data::camera_set_post_process_id(camera_data, this_id, &zero);
      Data::camera_set_texture_id(camera_data, this_id, &zero);
    }
    
    Data::data_unlock(camera_data);
  }
}


void
get_camera(util::generic_id const this_id,
           Data::Camera_data const *camera_data,
           Core::Camera *out_camera)
{
  assert(this_id);
  assert(camera_data);
  assert(out_camera);

  Data::data_lock(camera_data);
  
  if(Data::camera_exists(camera_data, this_id))
  {
    Camera_util::Camera_properties props;
    Data::camera_get_properties(camera_data, this_id, &props);
    
    out_camera->set_clear_color(Core::Color(props.clear_color));
    out_camera->set_clear_flags(props.clear_flags);
    out_camera->set_far_plane(props.far_plane);
    out_camera->set_near_plane(props.near_plane);
    out_camera->set_tags_to_render(props.cull_mask);
    out_camera->set_width(props.viewport_width);
    out_camera->set_height(props.viewport_height);
    out_camera->set_type(props.type);
    out_camera->set_feild_of_view(props.fov);
  }
  else
  {
    LOG_ERROR(Error_string::resource_not_found());
  }
  
  Data::data_unlock(camera_data);
}


void
remove_camera(const util::generic_id this_id,
              Data::Entity_data *entity_data,
              Data::Camera_data *camera_data)
{
  assert(this_id);
  assert(entity_data);
  assert(camera_data);

  // Remove component
  {
    Data::data_lock(entity_data);
    
    uint32_t current_data_types(0);
    Data::entity_get_components(entity_data, this_id, &current_data_types);
    
    current_data_types &= ~Common::Data_type::camera;
    Data::entity_set_components(entity_data, this_id, &current_data_types);

    Data::data_unlock(entity_data);
  }

  // Remove Camera
  {
    Data::data_lock(camera_data);
    Data::camera_remove(camera_data, this_id);
    Data::data_unlock(camera_data);
  }
}
  

bool
has_camera(const util::generic_id this_id,
           Data::Camera_data *camera_data)
{
  assert(this_id);
  assert(camera_data);
  
  Data::data_lock(camera_data);
  
  const bool has_camera = Data::camera_exists(camera_data, this_id);
  
  Data::data_unlock(camera_data);
  
  return has_camera;
}
  
  
} // ns