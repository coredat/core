#ifndef ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07
#define ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07


#include <application/resources.hpp>
#include <systems/entity/entity_id.hpp>
#include <math/math.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <cstddef>


#define ENTITY_POOL_SIZE 128


namespace Data {


struct Entity_pool
{
  Entity::Entity_id               entity_id[ENTITY_POOL_SIZE];
  math::transform                 transform[ENTITY_POOL_SIZE];
  
  Resource::Model::ENUM           model[ENTITY_POOL_SIZE];
  Resource::Texture::ENUM         texture[ENTITY_POOL_SIZE];
  
  renderer::vertex_buffer         model_id[ENTITY_POOL_SIZE];
  renderer::texture               texture_id[ENTITY_POOL_SIZE];
  Physics::Rigidbody              rigidbody[ENTITY_POOL_SIZE];
  Physics::Rigidbody_properties   rigidbody_property[ENTITY_POOL_SIZE];
  
  const std::size_t               size = ENTITY_POOL_SIZE;
};


} // ns


#endif // inc guard