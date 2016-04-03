#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include <core/entity_id.hpp>
#include <core/entity/entity_fwd.hpp>
#include <core/transform/transform_fwd.hpp>
#include <core/physics/collider.hpp>
#include <core/model/model_fwd.hpp>
#include <data/world_data/world_pools.hpp>
#include <stdint.h>
#include <memory>


namespace Core {


/*!
  Entity is how we access the properties of an entity.
  This abstracts how all the data is handled underneath.
*/
class Entity final
{

                                Entity(const Entity&) = delete;
  Entity&                       operator=(const Entity&) = delete;

public:

  explicit                      Entity();
  explicit                      Entity(const Core::Entity_id id, World_data::World *world);
  
                                ~Entity();
  
                                Entity(Entity&&);
  Entity&                       operator=(Entity&&);
  
                                operator Entity_ref() const;
  
  void                          destroy(); // TODO: Should this move to the common interface?
  
  // ** Common Entity Interface ** //
  
  Core::Entity_id               get_id() const;
  bool                          is_valid() const;
  
  uint32_t                      get_tags() const;
  bool                          has_tag(const uint32_t tag_id) const;
  void                          set_tags(const uint32_t set_tags);
  void                          add_tag(const uint32_t add_tag);
  void                          remove_tag(const uint32_t tag);
  
  void                          set_name(const char* set_name);
  const char*                   get_name() const;

  void                          send_event(const uint32_t id, const void *data, const uint32_t size_of_data);

  void                          set_transform(const Transform &transform);
  Transform                     get_transform() const;

  void                          set_material_id(const uint32_t id);
  uint32_t                      get_material_id() const;
  
  void                          set_model(const Core::Model &model);
  Core::Model                   get_model() const;
  
  void                          set_collider(const Core::Collider &collider);
  Core::Collider                get_collider() const;

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


}; // ns


#endif // inc guard