#ifndef ENTITY_REF_INCLUDED_6EB9BAB7_7AB4_4092_9FBC_BD003D9C8F83
#define ENTITY_REF_INCLUDED_6EB9BAB7_7AB4_4092_9FBC_BD003D9C8F83


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <data/world_data/world_data_fwd.hpp>


namespace Core {


/*!
  Entity_ref is a non owning entity.
*/
class Entity_ref final
{
public:

  explicit                      Entity_ref();
  explicit                      Entity_ref(Entity &entity);
  explicit                      Entity_ref(const util::generic_id id, World &world);
  explicit                      Entity_ref(const util::generic_id id, std::shared_ptr<World_data::World> world_data);
                                ~Entity_ref();
  
                                Entity_ref(const Entity_ref&);
                                Entity_ref(Entity_ref&&);
  
  Entity_ref&                   operator=(const Entity_ref&);
  Entity_ref&                   operator=(Entity_ref&&);

  // ** Common Entity Interface ** //
  
  bool                          is_valid() const;
                                operator bool() const;
  
  void                          destroy();

  void                          set_user_data(const uintptr_t user_data);
  uintptr_t                     get_user_data() const;
  
  uint32_t                      get_tags() const;
  bool                          has_tag(const uint32_t tag_id) const;
  void                          set_tags(const uint32_t set_tags);
  void                          add_tag(const uint32_t add_tag);
  void                          remove_tag(const uint32_t tag);
  
  void                          set_name(const char* set_name);
  const char*                   get_name() const;

  void                          set_transform(const Transform &transform);
  Transform                     get_transform() const;

  void                          set_renderer(const Renderer &renderer);
  Renderer                      get_renderer() const;
  
  void                          set_collider(const Core::Collider &collider);
  Core::Collider                get_collider() const;

  void                          set_rigidbody_properties(const Core::Rigidbody_properties rb_props);
  Core::Rigidbody_properties    set_rigidbody_properties() const;
  

  // ** Equality ** //
  
  bool                          operator ==(const Entity &other) const;
  bool                          operator ==(const Entity_ref &other) const;
  bool                          operator !=(const Entity &other) const;
  bool                          operator !=(const Entity_ref &other) const;

private:

  friend class Entity;
  friend class Camera;
                                operator util::generic_id() const;
  
  util::generic_id                            get_id() const;
  std::shared_ptr<const World_data::World>    get_world_data() const;
  std::shared_ptr<World_data::World>          get_world_data();
  

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;
  
  
}; // class


} // ns


#endif // inc guard