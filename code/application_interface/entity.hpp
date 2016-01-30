#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include "detail/entity_detail.hpp"
#include <data/data_fwd.hpp>
#include <data/logic_pool.hpp>
#include <systems/logic/logic_base.hpp>
#include <stddef.h>
#include <stdint.h>


// namespace Terminal {
namespace Data {


/*!
  Entity is how we access the properties of an entity.
  This abstracts how all the data is handled underneath.
*/
class Entity final
{
public:

  explicit                      Entity();
  
  ::Entity::Entity_id           get_id() const;
  bool                          is_valid() const;
  
  void                          set_parent(const ::Entity::Entity_id id);
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

    auto base = reinterpret_cast<Logic::Base*>(free_slot);
    base->set_entity(get_id());
    base->world_data = m_world_data;
    
    return comp;
  }
  
private:

  ENTITY_MEMBERS

}; // class


}; // ns


#endif // inc guard