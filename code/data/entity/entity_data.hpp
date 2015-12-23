#ifndef ENTITY_DATA_INCLUDED_BF840C0D_BDAE_446D_92FB_3782559F72D3
#define ENTITY_DATA_INCLUDED_BF840C0D_BDAE_446D_92FB_3782559F72D3


#include "entity_id.hpp"
#include <math/math.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <data/physics_world/rigidbody.hpp>
#include <data/physics_world/collider_data.hpp>
#include <cstddef>


#define ENTITY_POOL 128 * 3


namespace Data {


struct Entity
{
  Entity_id               entity_id[ENTITY_POOL];
  math::transform         transform[ENTITY_POOL];
  renderer::vertex_buffer vbo[ENTITY_POOL];
  renderer::texture       texture[ENTITY_POOL];
  Rigidbody               rigidbodies[ENTITY_POOL];
  Data::Collider_detail   collider[ENTITY_POOL];
  
  
  const std::size_t       max_number_of_entities = ENTITY_POOL;
  std::size_t             number_of_entities = 0;
};



class Entity_data
{
public:

  explicit Entity_data(const std::size_t number_of_entities);
  
  
  
private:

  const std::size_t number_of_entities;
  

};


bool        entity_get_index(const Entity entities[], const std::size_t number_of_entities, const Entity_id id, std::size_t *index);
void        entity_set_transform(Entity entities[], const std::size_t size_of_entities, const Entity_id id, math::transform *set_transform);
void        entity_get_transform(const Entity entities[], const std::size_t size_of_entities, const Entity_id id, math::transform *get_transform);


} // ns


#endif // inc guard