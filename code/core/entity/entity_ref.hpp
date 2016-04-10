#ifndef ENTITY_REF_INCLUDED_6EB9BAB7_7AB4_4092_9FBC_BD003D9C8F83
#define ENTITY_REF_INCLUDED_6EB9BAB7_7AB4_4092_9FBC_BD003D9C8F83


#include <core/entity_id.hpp>
#include <core/transform/transform_fwd.hpp>
#include <core/physics/collider.hpp>
#include <core/model/model_fwd.hpp>
#include <data/world_data/world_pools.hpp>
#include <stdint.h>
#include <memory>


namespace Core {


/*!
  Entity_ref is a non owning entity.
*/
class Entity_ref final
{
public:

  explicit                      Entity_ref();
  explicit                      Entity_ref(const Core::Entity_id id, World_data::World *world);
                                ~Entity_ref();
  
                                Entity_ref(const Entity_ref&);
                                Entity_ref(Entity_ref&&);
  
  Entity_ref&                   operator=(const Entity_ref&);
  Entity_ref&                   operator=(Entity_ref&&);

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


} // ns


#endif // inc guard