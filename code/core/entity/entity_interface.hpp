#ifndef ENTITY_INTERFACE_INCLUDED_5B0A948B_E6ED_4694_9A78_D05FFE173BA2
#define ENTITY_INTERFACE_INCLUDED_5B0A948B_E6ED_4694_9A78_D05FFE173BA2


#include <core/common/core_types.hpp>
#include <core/common/core_fwd.hpp>
#include <core/entity/detail/entity_detail_fwd.hpp>
#include <data/fwd_dec_data.hpp>


namespace Core {

// -------------------------------------------------- [ Callback Prototypes ]--

using on_collision_callback_fn = void(*)(const uintptr_t user_data,
                                         const Entity_ref ref,
                                         const Collision &collisions);

using on_message_callback_fn = void(*)(Entity_ref to,
                                       Entity_ref from,
                                       const uint32_t id,
                                       const uintptr_t data,
                                       const uintptr_t user_data);



// ----------------------------------------------------- [ Entity Interface ]--

class Entity_interface
{
protected:

                              Entity_interface();
                              Entity_interface(Core::World &world);
                              Entity_interface(const Core_detail::Entity_id id);
                              Entity_interface(const uint32_t id);
  
  virtual                     ~Entity_interface();
  
                              Entity_interface(const Entity_interface &other);
                              Entity_interface(Entity_interface &&other);

public:
  
  // ----------------------------------------------------------- [ Lifetime ]--
  
  void                        destroy();
  bool                        is_valid() const;
                              operator bool() const;

  // ------------------------------------------------------------ [ General ]--

  void                        set_user_data(const uintptr_t user_data);
  uintptr_t                   get_user_data() const;
  
  void                        set_tags(const uint32_t set_tags);
  uint32_t                    get_tags() const;
  bool                        has_tag(const uint32_t tag_id) const;
  void                        add_tag(const uint32_t add_tag);
  void                        remove_tag(const uint32_t tag);
  
  void                        set_name(const char* set_name);
  const char*                 get_name() const;
  
  uint32_t                    get_id() const;
  
  // ----------------------------------------------------------- [ Messages ]--
  
  void                        on_message_callback(
                                const on_message_callback_fn &callback,
                                const uintptr_t user_data = 0
                              );
  
  void                        send_message(
                                Core::Entity_ref to,
                                const uint32_t id,
                                const uintptr_t data = 0
                              ) const;
  
  // ---------------------------------------------------------- [ Transform ]--
  
  bool                        has_transform() const;
  Core::Transform             get_transform() const;
  void                        set_transform(const Core::Transform &trans);
  
  // ------------------------------------------------------------- [ Camera ]--
  
  bool                        has_camera() const;
  Core::Camera                get_camera() const;
  void                        set_camera(const Core::Camera &camera);

  // ----------------------------------------------------------- [ Renderer ]--
  
  bool                        has_renderer() const;
  
  // ---------------------------------------------------------- [ Rigidbody ]--
  
  bool                        has_rigidbody() const;
  Core::Rigidbody             get_rigidbody() const;
  void                        set_rigidbody(const Core::Rigidbody &rb);
  void                        on_collision_callback(
                                const uintptr_t user_data,
                                const on_collision_callback_fn &callback
                              );
  
  // ----------------------------------------------------------- [ Lighting ]--
  
  bool                        has_light() const;
  Core::Point_light           get_light() const;
  void                        set_light(const Core::Point_light &light);
  
  // ----------------------------------------------------------- [ Equality ]--
  
  bool                        operator ==(const Entity_interface &other) const;
  bool                        operator !=(const Entity_interface &other) const;
  
protected:

  void                        copy(const Entity_interface &other);
  void                        move(Entity_interface &other);
  util::generic_id            create_entity(const Core::World &world);
    
private:

  uint32_t m_id = 0;

};

} // ns


#endif // inc guard