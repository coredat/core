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
  
  bool                          is_valid() const;                             //!< Returns true if then entity is valid.
                                operator bool() const;                        //!< Shorthand for is_valid()
  
  void                          destroy();                                    //!< Marks the entity for destruction, this wont happen till then end of the frame.
  
  void                          set_user_data(const uintptr_t user_data);     //!< Set user_data for the entity, can be a pointer, core doesn't callback on this.
  uintptr_t                     get_user_data() const;                        //!< Get the current user_data.
  
  uint32_t                      get_tags() const;                             //!< Get the current bitfeld tags for this object.
  bool                          has_tag(const uint32_t tag_id) const;         //!< Check to see if the object has a bit set in the tags.
  void                          set_tags(const uint32_t set_tags);            //!< Set the tag bitfield.
  void                          add_tag(const uint32_t add_tag);              //!< Add a bit to the bitfield.
  void                          remove_tag(const uint32_t tag);               //!< Remove a bit from the bitfield.
  
  void                          set_name(const char* set_name);               //!< Set the name of object.
  const char*                   get_name() const;                             //!< Returns a volatile pointer to the name. This is considered for debugging only.

  void                          set_transform(const Transform &transform);    //!< Set the transform for the entity.
  Transform                     get_transform() const;                        //!< Gets a copy of the current transform.
  
  void                          set_renderer(const Renderer &renderer);       //!< Set the entities renderer, an entity may only have one renderer.
  Renderer                      get_renderer() const;                         //!< Get the entities renderer.
  
  void                          set_rigidbody(const Rigidbody &rigidbody);    //!< Set the rigidbody for the entity.
  Rigidbody                     get_rigidbody() const;                        //!< Get the rigidbody for the entity. Returns by type.


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