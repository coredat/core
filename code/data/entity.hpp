#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include "data_fwd.hpp"
#include <systems/entity/entity_id.hpp>
#include <systems/physics/rigidbody_properties.hpp>
#include <systems/physics/rigidbody_collider.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <math/math.hpp>
#include <stddef.h>
#include <stdint.h>


namespace Data {


class Entity final
{
public:

  explicit                      Entity(const uint8_t type_id = 0, World *world_data = nullptr);
  
  ::Entity::Entity_id           get_id() const;
  
  void                          set_parent(const ::Entity::Entity_id id);
  ::Entity::Entity_id           get_parent_id() const;

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
  
  Physics::World                *m_phy_world = nullptr;
  
private:

  ::Entity::Entity_id           m_this_id   = ::Entity::invalid_id();
  Data::World                   *m_world_data = nullptr;

}; // class


}; // ns


#endif // inc guard