#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <data/world_data/world_data_fwd.hpp>


namespace Core {


/*!
  Entity represents an object on the screen.
  This is an owning object, if it goes out of scope it will
  destroy the entity.
*/
class Entity final
{

                                Entity(const Entity&) = delete;
  Entity&                       operator=(const Entity&) = delete;

public:

  explicit                      Entity();
  explicit                      Entity(Core::World &world);
  
                                ~Entity();
  
                                Entity(Entity&&);
  Entity&                       operator=(Entity&&);

                                operator Entity_ref() const;
  
  void                          destroy();
  
  // ** Common Entity Interface ** //

  bool                          is_valid() const;
                                operator bool() const;
  
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

  friend class Entity_ref;
  friend class Camera;
  
                                operator util::generic_id() const;
  
  util::generic_id                          get_id() const;
  std::shared_ptr<const World_data::World>  get_world_data() const;
  std::shared_ptr<World_data::World>        get_world_data();

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


}; // ns


#endif // inc guard