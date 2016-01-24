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


// This is used to allow the factory to make a public version
// of the object to set members. Keeping setters etc out of the
// objects interface.
#define ENTITY_MEMBERS                                                  \
  ::Entity::Entity_id           m_this_id     = ::Entity::invalid_id(); \
  Data::World                   *m_world_data = nullptr;                \


namespace Data {

namespace Detail
{
  struct Private_entity
  {
    ENTITY_MEMBERS
  };
  
  inline void
  set_entity_members(Data::Entity *ent, Data::World *world_data, ::Entity::Entity_id id)
  {
    auto private_entity          = reinterpret_cast<Private_entity*>(ent);
    private_entity->m_world_data = world_data;
    private_entity->m_this_id    = id;
  }
} // ns


class Entity final
{
public:

  explicit                      Entity();
  
  ::Entity::Entity_id           get_id() const;
  bool                          is_valid() const;
  
  void                          set_parent(const ::Entity::Entity_id id);
  Entity                        get_parent() const;
  size_t                        get_number_of_children() const;

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
  
private:

  ENTITY_MEMBERS

}; // class


}; // ns


#endif // inc guard