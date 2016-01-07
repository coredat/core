#ifndef ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07
#define ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07


#include <systems/entity/entity_id.hpp>
#include <math/math.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/entity/entity_id.hpp>
#include <cstddef>


namespace Data {


class Entity_pool
{
public:

  explicit Entity_pool(const std::size_t number_of_entities);
  
  Entity::Entity_id*                get_entity_data()         { return m_entities.data();       }
  math::transform*                  get_transform_data()      { return m_transforms.data();     }
  renderer::vertex_buffer*          get_mesh_data()           { return m_vertex_buffers.data(); }
  renderer::texture*                get_texture_data()        { return m_textures.data();       }
  Physics::Rigidbody*               get_rigidbody_data()      { return m_rigidbodies.data();    }
  Physics::Rigidbody_properties*    get_collider_data()       { return m_colliders.data();      }
  
  std::size_t                 find_index(const Entity::Entity_id id) const;
  std::size_t                 add_entity(const Entity::Entity_id id);
  bool                        erase_entity(const Entity::Entity_id id);
  
  std::size_t                 capacity() const  { return m_entities.capacity(); }
  std::size_t                 size() const      { return m_entities.size();     }
  
private:

  std::vector<Entity::Entity_id>                m_entities;
  std::vector<math::transform>                  m_transforms;
  std::vector<renderer::vertex_buffer>          m_vertex_buffers;
  std::vector<renderer::texture>                m_textures;
  std::vector<Physics::Rigidbody>               m_rigidbodies;
  std::vector<Physics::Rigidbody_properties>    m_colliders;
};


} // ns


#endif // inc guard