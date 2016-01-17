#include "entity_factory.hpp"
#include <systems/physics/physics.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/entity/generic_id.hpp>
#include <data/data.hpp>
#include "resources.hpp"
#include <atomic>


namespace
{
  std::atomic<uint32_t> instance(0);
}


namespace Entity_factory {


Entity::Entity_id
create_ground(Data::Entity_pool *entity,
              Data::Pending_rigidbody_pool *pending_rb_pool,
              const Data::Model_pool *mesh_resources,
              const Data::Texture_pool *texture_resources)
{
  std::size_t empty_index;
  if(Entity::find_index_linearly(&empty_index, Entity::invalid_id(), entity->entity_id, entity->size))
  {
    const Entity::Entity_id id = Entity::Entity_id{1, ++instance};
    entity->entity_id[empty_index] = id;
   
    entity->transform[empty_index] = math::transform_init(math::vec3_zero(), math::vec3_init(10,10,10), math::quat_init());
    
    // Setup mesh
    {
      entity->model[empty_index] = Resource::Model::unit_plane;
    }
    
    // Setup texture
    {
      entity->texture[empty_index] = Resource::Texture::dev_squares;
    }
    
    // Setup phys
    {
      entity->rigidbody_collider[empty_index].collider_type = Physics::Collider_type::static_plane;
      entity->rigidbody_collider[empty_index].collider_info.static_plane.normal[0] = 0;
      entity->rigidbody_collider[empty_index].collider_info.static_plane.normal[1] = 1;
      entity->rigidbody_collider[empty_index].collider_info.static_plane.normal[2] = 0;
      entity->rigidbody_collider[empty_index].collider_info.static_plane.offset = 0;
      
      entity->rigidbody_property[empty_index].mass = 0;
      
      entity->rigidbody[empty_index].motion_state.reset(new Physics::Motion_state(entity->entity_id[empty_index], entity));
      
      Data::pending_rigidbody_pool_push(pending_rb_pool, entity->rigidbody_property[empty_index], entity->rigidbody_collider[empty_index], &entity->rigidbody[empty_index]);
      
      return id;
    }
  }
  
  return Entity::invalid_id();
}


Entity::Entity_id
create_random_cube(Data::Entity_pool *entity,
                   Data::Pending_rigidbody_pool *pending_rb_pool,
                   const Data::Model_pool *mesh_resources,
                   const Data::Texture_pool *texture_resources)
{
  std::size_t empty_index;
  if(Entity::find_index_linearly(&empty_index, Entity::invalid_id(), entity->entity_id, entity->size))
  {
    const Entity::Entity_id id = Entity::Entity_id{2, ++instance};
    entity->entity_id[empty_index] = id;
    
    // Setup texture
    {
      entity->texture[empty_index] = static_cast<Resource::Texture::ENUM>(rand() % Resource::Texture::size);
    }    
    
    // Setup phys
    {
      const float scale_x = static_cast<float>(rand() % 200 + 10) / 200;
      const float scale_y = static_cast<float>(rand() % 200 + 10) / 200;
      const float scale_z = static_cast<float>(rand() % 200 + 10) / 200;

      const float pos_x = static_cast<float>(rand() % 200 + 10) / 200;
      const float pos_y = static_cast<float>(rand() % 200 + 10) / 200;
      const float pos_z = static_cast<float>(rand() % 200 + 10) / 200;
      
      entity->transform[empty_index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                               math::vec3_init(scale_x, scale_y, scale_z),
                                                               math::quat_init());
      
      entity->rigidbody_collider[empty_index].collider_type = Physics::Collider_type::cube;
      entity->rigidbody_collider[empty_index].collider_info.cube.extents[0] = scale_x * 0.5f;
      entity->rigidbody_collider[empty_index].collider_info.cube.extents[1] = scale_y * 0.5f;
      entity->rigidbody_collider[empty_index].collider_info.cube.extents[2] = scale_z * 0.5f;
      
      entity->rigidbody_property[empty_index].move_axis     = Physics::Axis::x | Physics::Axis::y | Physics::Axis::z;
      entity->rigidbody_property[empty_index].rotation_axis = Physics::Axis::x | Physics::Axis::y | Physics::Axis::z;
      entity->rigidbody_property[empty_index].mass = 1;
      
      entity->rigidbody[empty_index].motion_state.reset(new Physics::Motion_state(entity->entity_id[empty_index], entity));
      
      Data::pending_rigidbody_pool_push(pending_rb_pool, entity->rigidbody_property[empty_index], entity->rigidbody_collider[empty_index], &entity->rigidbody[empty_index]);
    }
    
    return id;
  }
  
  return Entity::invalid_id();
}


Entity::Entity_id
create_actor(Data::Entity_pool *entity,
             Data::Pending_rigidbody_pool *pending_rb_pool,
             const Data::Model_pool *mesh_resources,
             const Data::Texture_pool *texture_resources)
{
  std::size_t empty_index;
  if(Entity::find_index_linearly(&empty_index, Entity::invalid_id(), entity->entity_id, entity->size))
  {
    const Entity::Entity_id id = Entity::Entity_id{3, ++instance};
    entity->entity_id[empty_index] = id;
    
    // Get mesh with mesh id.
    {
      entity->model[empty_index] = Resource::Model::unit_cube;
    }
    
    // Setup texture
    {
      entity->texture[empty_index] = Resource::Texture::dev_green;
    }

    // Setup phys
    {
      const float scale_x = 1.f;
      const float scale_y = 2.f;
      const float scale_z = 1.f;

      const float pos_x = 0.f;
      const float pos_y = 2.5f;
      const float pos_z = 0.f;
      
      entity->transform[empty_index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                               math::vec3_init(scale_x, scale_y, scale_z),
                                                               math::quat_init());
      
      entity->rigidbody_collider[empty_index].collider_type                = Physics::Collider_type::capsule;
      entity->rigidbody_collider[empty_index].collider_info.capsule.radius = scale_x * 0.5;
      entity->rigidbody_collider[empty_index].collider_info.capsule.height = scale_x * 1;
      
      entity->rigidbody_property[empty_index].rotation_axis                = Physics::Axis::y;
      entity->rigidbody_property[empty_index].mass                         = 1;
      
      entity->rigidbody[empty_index].motion_state.reset(new Physics::Motion_state(entity->entity_id[empty_index], entity));
      
      Data::pending_rigidbody_pool_push(pending_rb_pool, entity->rigidbody_property[empty_index], entity->rigidbody_collider[empty_index], &entity->rigidbody[empty_index]);
    }
    
    return id;
  }
  
  return Entity::invalid_id();
}


Entity::Entity_id
create_kinematic_actor(Data::Entity_pool *entity,
                       Data::Pending_rigidbody_pool *pending_rb_pool,
                       const Data::Model_pool *mesh_resources,
                       const Data::Texture_pool *texture_resources)
{
  std::size_t empty_index;
  if(Entity::find_index_linearly(&empty_index, Entity::invalid_id(), entity->entity_id, entity->size))
  {
    const Entity::Entity_id id = Entity::Entity_id{4, ++instance};
    entity->entity_id[empty_index] = id;
    
    entity->rigidbody_collider[empty_index].collider_type = Physics::Collider_type::none;
    
    // Get mesh with mesh id.
    {
      entity->model[empty_index] = Resource::Model::unit_cube;
    }
    
    // Setup texture
    {
      entity->texture[empty_index] = Resource::Texture::dev_orange;
    }
    
    // Setup transform
    {
      const float scale_x = 0.5f;
      const float scale_y = 0.5f;
      const float scale_z = 0.5f;

      const float pos_x = 3.f;
      const float pos_y = 2.5f;
      const float pos_z = 0.f;
      
      entity->transform[empty_index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                               math::vec3_init(scale_x, scale_y, scale_z),
                                                               math::quat_init());
    }
    
    return id;
  }
  
  return Entity::invalid_id();
}


Entity::Entity_id
create_placement_cube(Data::Entity_pool *entity,
                      Data::Pending_rigidbody_pool *pending_rb_pool,
                      const Data::Model_pool *mesh_resources,
                      const Data::Texture_pool *texture_resources)
{
  std::size_t empty_index;
  if(Entity::find_index_linearly(&empty_index, Entity::invalid_id(), entity->entity_id, entity->size))
  {
    const Entity::Entity_id id = Entity::Entity_id{2, ++instance};
    entity->entity_id[empty_index] = id;
    
    // Setup texture
    {
      entity->texture[empty_index] = static_cast<Resource::Texture::ENUM>(rand() % Resource::Texture::size);
    }    
    
    // Setup phys
    {
      const float scale_x = static_cast<float>(rand() % 200 + 10) / 200;
      const float scale_y = static_cast<float>(rand() % 200 + 10) / 200;
      const float scale_z = static_cast<float>(rand() % 200 + 10) / 200;

      const float pos_x = static_cast<float>(rand() % 200 + 10) / 200;
      const float pos_y = static_cast<float>(rand() % 200 + 10) / 200;
      const float pos_z = static_cast<float>(rand() % 200 + 10) / 200;
      
      entity->transform[empty_index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                               math::vec3_init(scale_x, scale_y, scale_z),
                                                               math::quat_init());
    }
    
    return id;
  }
  
  return Entity::invalid_id();
}



Entity::Entity_id
create_connection_node(Data::Entity_pool *entity,
                       Data::Pending_rigidbody_pool *pending_rb_pool,
                       const Data::Model_pool *mesh_resources,
                       const Data::Texture_pool *texture_resources)
{
  std::size_t empty_index;
  if(Entity::find_index_linearly(&empty_index, Entity::invalid_id(), entity->entity_id, entity->size))
  {
    const Entity::Entity_id id = Entity::Entity_id{2, ++instance};
    entity->entity_id[empty_index] = id;
    
    // Setup texture
    {
      entity->texture[empty_index] = static_cast<Resource::Texture::ENUM>(Resource::Texture::dev_red);
    }    
    
    // Setup phys
    {
      const float size = 2.f;
      const float scale_x = size;
      const float scale_y = size * math::g_ratio();
      const float scale_z = size;

      const float pos_x = 0;
      const float pos_y = scale_x * 0.5f;
      const float pos_z = 0;
      
      entity->transform[empty_index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                            math::vec3_init(scale_x, scale_y, scale_z),
                                                            math::quat_init());
      
      entity->rigidbody_collider[empty_index].collider_type = Physics::Collider_type::cube;
      entity->rigidbody_collider[empty_index].collider_info.cube.extents[0] = scale_x * 0.5f;
      entity->rigidbody_collider[empty_index].collider_info.cube.extents[1] = scale_y * 0.5f;
      entity->rigidbody_collider[empty_index].collider_info.cube.extents[2] = scale_z * 0.5f;      
      
      entity->rigidbody_property[empty_index].move_axis     = Physics::Axis::x | Physics::Axis::y | Physics::Axis::z;
      entity->rigidbody_property[empty_index].rotation_axis = Physics::Axis::x | Physics::Axis::y | Physics::Axis::z;
      entity->rigidbody_property[empty_index].mass = 0;
      
      entity->rigidbody[empty_index].motion_state.reset(new Physics::Motion_state(entity->entity_id[empty_index], entity));
      
      Data::pending_rigidbody_pool_push(pending_rb_pool, entity->rigidbody_property[empty_index], entity->rigidbody_collider[empty_index], &entity->rigidbody[empty_index]);
    }
    
    return id;
  }
  
  return Entity::invalid_id();}
} // ns