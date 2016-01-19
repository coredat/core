#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include <systems/entity/entity_id.hpp>
#include <systems/physics/rigidbody_properties.hpp>
#include <systems/physics/rigidbody_collider.hpp>
#include <stddef.h>


namespace Data {


class Entity final
{
public:

  explicit                      Entity();
  
  void                          set_parent(const ::Entity::Entity_id id);
  ::Entity::Entity_id           get_parent() const;

  void                          set_material_id(const size_t id);
  size_t                        get_material_id() const;
  
  void                          set_model_id(const size_t id);
  size_t                        get_model_id() const;
  
  void                          set_rigidbody_properties(const Physics::Rigidbody_properties props);
  Physics::Rigidbody_properties get_rigidbody_properties() const;
  
  void                          set_rigidbody_collider(const Physics::Rigidbody_collider collider);
  Physics::Rigidbody_collider   get_rigidbody_collider() const;
  
private:

  ::Entity::Entity_id           m_this_id   = ::Entity::invalid_id();
  ::Entity::Entity_id           m_parent_id = ::Entity::invalid_id();
  
  size_t                        m_material_id = 0;
  size_t                        m_model_id    = 0;
  
  Physics::Rigidbody_properties m_rb_props;
  Physics::Rigidbody_collider   m_rb_collider;

}; // class


}; // ns


#endif // inc guard