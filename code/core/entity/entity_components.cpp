#include <core/entity/entity_components.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/camera/camera.hpp>
#include <core/color/color.hpp>
#include <core/world/detail/world_index.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/camera_data.hpp>
#include <common/data_types.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace Core {
namespace Entity_component {


bool
set_camera(const Core::Entity_ref &ref,
           const Core::Camera &camera)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);

  // Check if Entity has camera data. If not add it.
  {
    Data::Entity_data *entity_data(world_data->entity);
    assert(entity_data);

    Data::data_lock(entity_data);
    
    uint32_t current_data_types(0);
    Data::entity_get_components(entity_data, entity_uint_id, &current_data_types);
    
    if(!Common::Data_type::has_data_type(current_data_types, Common::Data_type::camera))
    {
      current_data_types |= Common::Data_type::camera;
      Data::entity_set_components(entity_data, entity_uint_id, &current_data_types);
    }
    
    Data::data_unlock(entity_data);
  }

  // Add entity properties
  {
    Data::Camera_data *cam_data(world_data->camera_data);
    assert(cam_data);
    
    Data::data_lock(cam_data);
    
    // Insert camera if none exists for this entity.
    if(!Data::camera_exists(cam_data, entity_uint_id))
    {
      Data::camera_push(cam_data, entity_uint_id);
    }
    
    // Set data
    {
      Camera_util::Camera_properties properties;
      Data::camera_get_properties(cam_data, entity_uint_id, &properties);
      
      properties.clear_color     = camera.get_clear_color().get_color();
      properties.clear_flags     = camera.get_clear_flags();
      properties.cull_mask       = camera.get_tags_to_render();
      properties.far_plane       = camera.get_far_plane();
      properties.fov             = camera.get_field_of_view();
      properties.near_plane      = camera.get_near_plane();
      properties.type            = camera.get_type();
      properties.viewport_height = camera.get_height();
      properties.viewport_width  = camera.get_width();
      
      Data::camera_set_properties(cam_data, entity_uint_id, &properties);
      
      const uint32_t priority(camera.get_priority());
      Data::camera_set_priority(cam_data, entity_uint_id, &priority);
      
      const uint32_t zero(0);
      Data::camera_set_post_process_id(cam_data, entity_uint_id, &zero);
      Data::camera_set_texture_id(cam_data, entity_uint_id, &zero);
    }
    
    Data::data_unlock(cam_data);
  }
  
  return true;
}


Core::Camera
get_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return Core::Camera();
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  Core::Camera return_camera;

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  Data::data_lock(cam_data);
  
  if(Data::camera_exists(cam_data, entity_uint_id))
  {
    Camera_util::Camera_properties props;
    Data::camera_get_properties(cam_data, entity_uint_id, &props);
    
    return_camera.set_clear_color(Core::Color(props.clear_color));
    return_camera.set_clear_flags(props.clear_flags);
    return_camera.set_far_plane(props.far_plane);
    return_camera.set_near_plane(props.near_plane);
    return_camera.set_tags_to_render(props.cull_mask);
    return_camera.set_width(props.viewport_width);
    return_camera.set_height(props.viewport_height);
    return_camera.set_type(props.type);
    return_camera.set_feild_of_view(props.fov);
  }
  else
  {
    LOG_ERROR(Error_string::resource_not_found());
  }
  
  Data::data_unlock(cam_data);
  
  return Core::Camera();
}


bool
has_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  Data::data_lock(cam_data);
  
  const bool has_camera = Data::camera_exists(cam_data, entity_uint_id);
  
  Data::data_unlock(cam_data);
  
  return has_camera;
}


} // ns
} // ns