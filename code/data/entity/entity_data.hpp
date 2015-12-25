#ifndef ENTITY_DATA_INCLUDED_BF840C0D_BDAE_446D_92FB_3782559F72D3
#define ENTITY_DATA_INCLUDED_BF840C0D_BDAE_446D_92FB_3782559F72D3


#include "entity_id.hpp"
#include <math/math.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <data/physics_world/rigidbody.hpp>
#include <data/physics_world/collider_data.hpp>
#include <cstddef>


#define ENTITY_POOL 128 * 2


namespace Entity
{

class Data
{
public:

  explicit Data(const std::size_t number_of_entities);
  
  Entity_id*                  get_entity_data()         { return m_entities.data();       }
  math::transform*            get_transform_data()      { return m_transforms.data();     }
  renderer::vertex_buffer*    get_mesh_data()           { return m_vertex_buffers.data(); }
  renderer::texture*          get_texture_data()        { return m_textures.data();       }
  Rigidbody*                  get_rigidbody_data()      { return m_rigidbodies.data();    }
  Physics::Collider_detail*   get_collider_data()       { return m_colliders.data();      }
  
  std::size_t                 find_index(const Entity_id id) const;
  std::size_t                 add_entity(const Entity_id id);
  bool                        erase_entity(const Entity_id id);
  
  std::size_t                 capacity() const  { return m_entities.capacity(); }
  std::size_t                 size() const      { return m_entities.size();     }
  
private:

  std::vector<Entity_id>                  m_entities;
  std::vector<math::transform>            m_transforms;
  std::vector<renderer::vertex_buffer>    m_vertex_buffers;
  std::vector<renderer::texture>          m_textures;
  std::vector<Rigidbody>                  m_rigidbodies;
  std::vector<Physics::Collider_detail>   m_colliders;
};

}


namespace Data {


struct Entity
{
  Entity_id                 entity_id[ENTITY_POOL];
  math::transform           transform[ENTITY_POOL];
  renderer::vertex_buffer   vbo[ENTITY_POOL];
  renderer::texture         texture[ENTITY_POOL];
  Rigidbody                 rigidbodies[ENTITY_POOL];
  Physics::Collider_detail  collider[ENTITY_POOL];
  
  const std::size_t       max_number_of_entities = ENTITY_POOL;
  std::size_t             number_of_entities = 0;
};


bool        entity_get_index(const Entity entities[], const std::size_t number_of_entities, const Entity_id id, std::size_t *index);
void        entity_set_transform(Entity entities[], const std::size_t size_of_entities, const Entity_id id, math::transform *set_transform);
void        entity_get_transform(const Entity entities[], const std::size_t size_of_entities, const Entity_id id, math::transform *get_transform);


} // ns


#endif // inc guard