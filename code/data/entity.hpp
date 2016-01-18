#ifndef ENTITY_INCLUDED_E266E8DC_3E88_4826_9B3A_04A7BECA3065
#define ENTITY_INCLUDED_E266E8DC_3E88_4826_9B3A_04A7BECA3065


#include "data_fwd.hpp"
#include <math/transform/transform.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/physics/rigidbody_properties.hpp>
#include <systems/physics/rigidbody_collider.hpp>
#include <stdint.h>


namespace Data {


class Entity
{
public:

  // ** Relationships ** //
  void                              set_parent(const Entity_id parent_id);
  Entity_id                         get_parent();
  
  // ** Data ** //
  void                              set_transform(const math::transform &new_transform);
  math::transform                   get_transform() const;
  
  void                              set_rigidbody_properties(const Physics::Rigidbody_properties props);
  Physics::Rigidbody_properties     get_rigidbody_properties() const;
  
  void                              set_rigidbody_collider(const Physics::Rigidbody_collider collider);
  Physics::Rigidbody_collider       get_rigidbody_collider() const;
  
  void                              set_texture_resource_id(const uint32_t resource_id);
  uint32_t                          get_texture_resource() const;
  
  void                              set_model_resource(const uint32_t resource_id);
  uint32_t                          get_model_resource() const;
  
private:

  Entity::Entity_id                 this_id;

}; // ns


} // ns


#endif // inc guard