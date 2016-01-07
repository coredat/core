#include "entity_data.hpp"
#include <systems/physics/physics.hpp>
#include <assert.h>



namespace Entity
{


Data::Data(const std::size_t number_of_entities)
{
  m_entities.reserve(number_of_entities);
  m_transforms.reserve(number_of_entities);
  m_vertex_buffers.reserve(number_of_entities);
  m_textures.reserve(number_of_entities);
  m_rigidbodies.reserve(number_of_entities);
  m_colliders.reserve(number_of_entities);
}
  
  
std::size_t
Data::find_index(const Entity_id id) const
{
  for(std::size_t i = 0; i < m_entities.size(); ++i)
  {
    if(id == m_entities[i])
    {
      return i;
    }
  }
  
  assert(false);
  return 0; // Hmmm 0 is valid?
}


std::size_t
Data::add_entity(const Entity_id id)
{
  if(size() < capacity())
  {
    m_entities.emplace_back(id);
    m_transforms.emplace_back(math::transform_init(math::vec3_zero(), math::vec3_one(), math::quat_init()));
    m_vertex_buffers.emplace_back();
    m_textures.emplace_back();
    m_rigidbodies.emplace_back();
    m_colliders.emplace_back();
    
//    assert(m_entities.size() ==
//       m_transforms.size() ==
//       m_vertex_buffers.size() ==
//       m_textures.size() ==
//       m_rigidbodies.size() ==
//       m_colliders.size());
  
    return m_entities.size() - 1;
  }
  
  return 0;
}


bool
Data::erase_entity(const Entity_id id)
{
  const std::size_t index = find_index(id);
  
  // TODO : Issue with index 0 again.
  // ie how do we know if we found a valid index.
  
  if(size() < capacity())
  {
    m_entities.erase(std::begin(m_entities) + index);
    m_transforms.erase(std::begin(m_transforms) + index);
    m_vertex_buffers.erase(std::begin(m_vertex_buffers) + index);
    m_textures.erase(std::begin(m_textures) + index);
    m_rigidbodies.erase(std::begin(m_rigidbodies) + index);
    m_colliders.erase(std::begin(m_colliders) + index);
    
//    assert(m_entities.size() ==
//           m_transforms.size() ==
//           m_vertex_buffers.size() ==
//           m_textures.size() ==
//           m_rigidbodies.size() ==
//           m_colliders.size());
  
    return true;
  }
  
  return false;
}
  

} // ns