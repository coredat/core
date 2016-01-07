#include "entity_factory.hpp"
#include <systems/physics/physics.hpp>
#include <systems/entity/entity_data.hpp>
#include <systems/entity/generic_id.hpp>
#include <data/model_pool.hpp>
#include <data/entity_pool.hpp>
#include <data/texture_pool.hpp>
#include "resources.hpp"


namespace
{
  uint32_t instance = 0;
}


namespace Entity_factory {


Entity::Entity_id
create_ground(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources)
{
  const Entity::Entity_id id = Entity::Entity_id{1, ++instance};
  const std::size_t index = entity->add_entity(id);
  
  // Setup mesh
  {
    assert(Resource::Model::unit_plane < mesh_resources->size);
    entity->get_mesh_data()[index] = mesh_resources->vbo[Resource::Model::unit_plane];
  }
  
  // Setup texture
  {
    assert(Resource::Texture::dev_red < texture_resources->size);
    entity->get_texture_data()[index] = texture_resources->textures[Resource::Texture::dev_red];
  }
  
  // Setup phys
  {
    entity->get_collider_data()[index].collider_type = Physics::Collider_type::static_plane;
    entity->get_collider_data()[index].collider_info.static_plane.normal[0] = 0;
    entity->get_collider_data()[index].collider_info.static_plane.normal[1] = 1;
    entity->get_collider_data()[index].collider_info.static_plane.normal[2] = 0;
    entity->get_collider_data()[index].collider_info.static_plane.offset = 0;
    entity->get_collider_data()[index].mass = 0;
    
    entity->get_transform_data()[index] = math::transform_init(math::vec3_zero(), math::vec3_init(10,10,10), math::quat_init());
    
    entity->get_rigidbody_data()[index].motion_state.reset(new Physics::Motion_state(entity->get_entity_data()[index], entity));
  }
  
  return id;
}


Entity::Entity_id
create_random_cube(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources)
{
  const Entity::Entity_id id = Entity::Entity_id{2, ++instance};
  const std::size_t index = entity->add_entity(id);
  
  // Setup phys
  {
    const float scale_x = static_cast<float>(rand() % 200 + 10) / 200;
    const float scale_y = static_cast<float>(rand() % 200 + 10) / 200;
    const float scale_z = static_cast<float>(rand() % 200 + 10) / 200;

    const float pos_x = static_cast<float>(rand() % 200 + 10) / 200;
    const float pos_y = static_cast<float>(rand() % 200 + 10) / 200;
    const float pos_z = static_cast<float>(rand() % 200 + 10) / 200;
    
    entity->get_transform_data()[index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                             math::vec3_init(scale_x, scale_y, scale_z),
                                                             math::quat_init());
    
    entity->get_collider_data()[index].collider_type = Physics::Collider_type::cube;
    entity->get_collider_data()[index].collider_info.cube.extents[0] = scale_x * 0.5;
    entity->get_collider_data()[index].collider_info.cube.extents[1] = scale_y * 0.5;
    entity->get_collider_data()[index].collider_info.cube.extents[2] = scale_z * 0.5;
    entity->get_collider_data()[index].mass = 1;
    
    entity->get_rigidbody_data()[index].motion_state.reset(new Physics::Motion_state(entity->get_entity_data()[index], entity));
  }
  
  return id;
}


Entity::Entity_id
create_actor(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources)
{
  const Entity::Entity_id id = Entity::Entity_id{3, ++instance};
  const std::size_t index = entity->add_entity(id);

  // Get mesh with mesh id.
  {
    assert(Resource::Model::unit_cube < mesh_resources->size);
    entity->get_mesh_data()[index] = mesh_resources->vbo[Resource::Model::unit_cube];
  }
  
  // Setup texture
  {
    assert(Resource::Texture::dev_green < texture_resources->size);
    entity->get_texture_data()[index] = texture_resources->textures[Resource::Texture::dev_green];
  }

  // Setup phys
  {
    const float scale_x = 1.f;
    const float scale_y = 2.f;
    const float scale_z = 1.f;

    const float pos_x = 0.f;
    const float pos_y = 2.5f;
    const float pos_z = 0.f;
    
    entity->get_transform_data()[index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                             math::vec3_init(scale_x, scale_y, scale_z),
                                                             math::quat_init());
    
    entity->get_collider_data()[index].collider_type                = Physics::Collider_type::capsule;
    entity->get_collider_data()[index].collider_info.capsule.radius = scale_x * 0.5;
    entity->get_collider_data()[index].collider_info.capsule.height = scale_x * 1;
    entity->get_collider_data()[index].rotation_axis                = Physics::Axis::y;
    entity->get_collider_data()[index].mass                         = 1;
    
    entity->get_rigidbody_data()[index].motion_state.reset(new Physics::Motion_state(entity->get_entity_data()[index], entity));
  }
  
  return id;
}


Entity::Entity_id
create_kinematic_actor(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources)
{
  const Entity::Entity_id id = Entity::Entity_id{4, ++instance};

  const std::size_t index = entity->add_entity(id);
  
  entity->get_collider_data()[index].collider_type = Physics::Collider_type::none;
  
  // Get mesh with mesh id.
  {
    assert(Resource::Model::unit_cube < mesh_resources->size);
    entity->get_mesh_data()[index] = mesh_resources->vbo[Resource::Model::unit_cube];
  }
  
  // Setup texture
  {
    assert(Resource::Texture::dev_orange < texture_resources->size);
    entity->get_texture_data()[index] = texture_resources->textures[Resource::Texture::dev_orange];
  }
  
  // Setup transform
  {
    const float scale_x = 0.5f;
    const float scale_y = 0.5f;
    const float scale_z = 0.5f;

    const float pos_x = 3.f;
    const float pos_y = 2.5f;
    const float pos_z = 0.f;
    
    entity->get_transform_data()[index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                             math::vec3_init(scale_x, scale_y, scale_z),
                                                             math::quat_init());
  }
  
  return id;
}


} // ns