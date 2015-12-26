#ifndef ENTITY_DATA_INCLUDED_BF840C0D_BDAE_446D_92FB_3782559F72D3
#define ENTITY_DATA_INCLUDED_BF840C0D_BDAE_446D_92FB_3782559F72D3


#include "entity_id.hpp"
#include <math/math.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <data/physics_world/rigidbody.hpp>
#include <data/physics_world/physics_fwd.hpp>
#include <cstddef>


namespace Entity {


class Data
{
public:

  explicit Data(const std::size_t number_of_entities);
  
  Entity_id*                        get_entity_data()         { return m_entities.data();       }
  math::transform*                  get_transform_data()      { return m_transforms.data();     }
  renderer::vertex_buffer*          get_mesh_data()           { return m_vertex_buffers.data(); }
  renderer::texture*                get_texture_data()        { return m_textures.data();       }
  Physics::Rigidbody*               get_rigidbody_data()      { return m_rigidbodies.data();    }
  Physics::Rigidbody_properties*    get_collider_data()       { return m_colliders.data();      }
  
  std::size_t                 find_index(const Entity_id id) const;
  std::size_t                 add_entity(const Entity_id id);
  bool                        erase_entity(const Entity_id id);
  
  std::size_t                 capacity() const  { return m_entities.capacity(); }
  std::size_t                 size() const      { return m_entities.size();     }
  
private:

  std::vector<Entity_id>                        m_entities;
  std::vector<math::transform>                  m_transforms;
  std::vector<renderer::vertex_buffer>          m_vertex_buffers;
  std::vector<renderer::texture>                m_textures;
  std::vector<Physics::Rigidbody>               m_rigidbodies;
  std::vector<Physics::Rigidbody_properties>    m_colliders;
};

} // ns


#endif // inc guard