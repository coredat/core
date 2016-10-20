#include <core/entity/entity_components.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/camera/camera.hpp>
#include <core/color/color.hpp>
#include <core/world/detail/world_index.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/camera_data.hpp>
#include <common/data_types.hpp>
#include <utilities/logging.hpp>


namespace Core {
namespace Entity_utils {


bool
set_camera_component(const Core::Entity_ref &ref, const Core::Camera &camera)
{
  if(!ref)
  {
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
      ::Camera::Camera_properties properties;
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
get_camera_component(const Core::Entity_ref &ref)
{
  assert(false);
  // Get and build a Core::Camera.
}


} // ns
} // ns