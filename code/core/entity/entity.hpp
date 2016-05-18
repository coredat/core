#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include <utilities/generic_id.hpp>
#include <core/entity/entity_fwd.hpp>
#include <core/transform/transform_fwd.hpp>
#include <core/physics/physics_fwd.hpp>
#include <core/model/model_fwd.hpp>
#include <core/world/world_fwd.hpp>
#include <core/world/detail/world_detail_fwd.hpp>
#include <core/renderer/renderer_fwd.hpp>
#include <stdint.h>
#include <memory>


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
  
  void                          destroy(); // TODO: Should this move to the common interface?
  
  // ** Common Entity Interface ** //

  bool                          is_valid() const;
  
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

  void                          set_material_id(const uint32_t id);
  uint32_t                      get_material_id() const;

  void                          set_model(const Core::Model &model);
  Core::Model                   get_model() const;
  
  void                          set_collider(const Core::Collider &collider);
  Core::Collider                get_collider() const;

  void                          set_rigidbody_properties(const Core::Rigidbody_properties rb_props);
  Core::Rigidbody_properties    set_rigidbody_properties() const;
  
  void                          set_renderer(const Core::Renderer &renderer);
  Core::Renderer                get_renderer() const;
  
                                operator bool() const;

  // ** Equality ** //
  
  bool                          operator ==(const Entity &other) const;
  bool                          operator ==(const Entity_ref &other) const;
  bool                          operator !=(const Entity &other) const;
  bool                          operator !=(const Entity_ref &other) const;

private:

  friend class Entity_ref;
  friend class Camera;
  
  util::generic_id                           get_id() const; // todo make private friend
  std::shared_ptr<const World_detail::Data>  get_world_data() const;
  std::shared_ptr<World_detail::Data>        get_world_data();

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


}; // ns


#endif // inc guard