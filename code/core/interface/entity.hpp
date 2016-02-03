#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include "component.hpp"
#include "../entity_id.hpp"
#include "detail/entity_detail.hpp"
#include <systems/physics/rigidbody_properties.hpp>
#include <systems/physics/rigidbody_collider.hpp>
#include <math/math.hpp>
#include <data/data.hpp>
#include <stddef.h>
#include <stdint.h>


namespace Core {


/*!
  Entity is how we access the properties of an entity.
  This abstracts how all the data is handled underneath.
*/
class Entity final
{
public:

  explicit                      Entity();
  
  Core::Entity_id               get_id() const;
  bool                          is_valid() const;
  
  uint32_t                      get_tags() const;
  bool                          has_tag(const uint32_t tag_id);
  void                          set_tags(const uint32_t set_tags);
  void                          add_tag(const uint32_t add_tag);
  void                          remove_tag(const uint32_t tag);
  
  void                          set_parent(const Core::Entity_id id);
  Entity                        get_parent() const;
  
  size_t                        get_number_of_children() const;
  Entity                        get_child(const size_t index) const;

  void                          set_transform(const math::transform &transform);
  math::transform               get_transform() const;

  void                          set_material_id(const size_t id);
  size_t                        get_material_id() const;
  
  void                          set_model_id(const size_t id);
  size_t                        get_model_id() const;
  
  void                          set_rigidbody_properties(const Physics::Rigidbody_properties props);
  Physics::Rigidbody_properties get_rigidbody_properties() const;
  
  void                          set_rigidbody_collider(const Physics::Rigidbody_collider collider);
  Physics::Rigidbody_collider   get_rigidbody_collider() const;
  
  template<typename T>
  T*
  add_component()
  {
    const auto free_slot = Data::logic_pool_get_slot(m_world_data->logic_pool, get_id());
    auto comp = new(free_slot) T();

    auto base = reinterpret_cast<Core::Component*>(free_slot);
    base->m_world_data = m_world_data;
    base->m_entity_id  = this->get_id();
    
    return comp;
  }
  
  template<typename T>
  T* get_component(const uint32_t rtti_id)
  {
    assert(false); /* todo */ return nullptr;
  }
  
  size_t get_number_of_components() const;
  
private:

  ENTITY_MEMBERS

}; // class


}; // ns


#endif // inc guard