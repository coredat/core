#include <core/entity/entity_components.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/camera/camera.hpp>
#include <core/lighting/point_light.hpp>
#include <core/color/color.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/transform/transform.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/renderer/renderer.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/camera_data.hpp>
#include <common/data_types.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <transformations/entity/entity_camera.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_renderer.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_light.hpp>


namespace Core {
namespace Entity_component {


// Transform Component //

bool
set_transform(const Core::Entity_ref &ref,
              const Core::Transform &transform)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  Entity_detail::set_transform(entity_uint_id,
                               world_data->entity,
                               world_data->transform,
                               world_data->rigidbody_data,
                               &world_data->physics_world,
                               world_data->trigger_data,
                               world_data->mesh_data,
                               world_data->text_data,
                               transform);
  
  return true;
}


Core::Transform
get_transform(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return Core::Transform();
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  return Entity_detail::get_core_transform(entity_uint_id,
                                           world_data->entity,
                                           world_data->transform);

  return Core::Transform();
}


bool
has_transform(const Core::Entity_ref &ref)
{
  // You cannot remove a transform from an entity.
  return true;
}


// Renderer Component //

bool
set_renderer(const Core::Entity_ref &ref,
             const Core::Renderer &renderer)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  Entity_detail::set_renderer(entity_uint_id,
                              world_data->entity,
                              world_data->transform,
                              world_data->mesh_data,
                              world_data->text_data,
                              renderer);
  
  return true;
}
  

Core::Renderer
get_renderer(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return Core::Renderer();
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  return Entity_detail::get_renderer(entity_uint_id,
                                     world_data->entity,
                                     world_data->mesh_data,
                                     world_data->text_data);
}


void
remove_renderer(const Core::Entity_ref &renderer)
{
  assert(false); // not impl because churn going on here.
}


bool
has_renderer(const Core::Entity_ref &ref)
{
  assert(false); // churn
  return false;
}


/*
  Rigidbody Component
  --
  Attaching a rigidbody will use the use physics
  engine to update the entities transform.
*/

bool
set_rigidbody(const Core::Entity_ref &ref,
              const Core::Rigidbody &rigidbody)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  Entity_detail::set_rigidbody(entity_uint_id,
                               rigidbody,
                               world_data->transform,
                               world_data->entity,
                               world_data->trigger_data,
                               world_data->rigidbody_data,
                               &world_data->physics_world);
  
  return true;
}
  

Core::Rigidbody
get_rigidbody(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return Core::Rigidbody();
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  return Entity_detail::get_rigidbody(entity_uint_id,
                                      world_data->entity,
                                      world_data->transform,
                                      world_data->rigidbody_data,
                                      world_data->trigger_data);
}


void
remove_rigidbody(const Core::Entity_ref &rigidbody)
{
  assert(false); // churn alert
}


bool
has_rigidbody(const Core::Entity_ref &ref)
{
  assert(false); // churn alert.
  return false;
}


/*
  Camera component
*/


bool
set_camera(const Core::Entity_ref &ref,
           const Core::Camera &camera)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  Data::Entity_data *entity_data = world_data->entity;
  assert(entity_data);
  
  Data::Camera_data *camera_data = world_data->camera_data;
  assert(camera_data);

  Entity_detail::set_camera(entity_uint_id, entity_data, camera_data, &camera);
  
  return true;
}


Core::Camera
get_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return Core::Camera();
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  Core::Camera return_camera;

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  Entity_detail::get_camera(entity_uint_id, cam_data, &return_camera);
  
  return return_camera;
}


void
remove_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return;
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(ref.get_id());
  
  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  Data::Entity_data *entity_data = world_data->entity;
  assert(entity_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  Entity_detail::remove_camera(entity_uint_id, entity_data, cam_data);
}


bool
has_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  return Entity_detail::has_camera(entity_uint_id, cam_data);
}


/*
  Light Component
*/


bool
set_light(const Core::Entity_ref &ref,
          const Core::Point_light &light)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  Data::Entity_data *entity_data = world_data->entity;
  assert(entity_data);
  
  Data::Light_data *light_data = world_data->light_data;
  assert(light_data);
  
  Data::Transform_data *transform_data = world_data->transform;
  assert(transform_data);

  Entity_detail::set_light(entity_uint_id, entity_data, light_data, transform_data, &light);
  
  return true;
}
  
  
Core::Point_light
get_light(const Core::Entity_ref &ref)
{
  return Core::Point_light();
}


void
remove_light(const Core::Entity_ref &ref)
{
}


bool
has_light(const Core::Entity_ref &ref)
{
  return false;
}


} // ns
} // ns