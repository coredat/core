#ifndef ENTITY_INTERFACE_INCLUDED_5B0A948B_E6ED_4694_9A78_D05FFE173BA2
#define ENTITY_INTERFACE_INCLUDED_5B0A948B_E6ED_4694_9A78_D05FFE173BA2


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <core/entity/detail/entity_detail_fwd.hpp>
#include <data/fwd_dec_data.hpp>


namespace Core {


class Entity_interface
{
protected:

                                Entity_interface();
                                Entity_interface(Core::World &world);
                                Entity_interface(const Core_detail::Entity_id id);
  
  virtual                       ~Entity_interface();
  
                                Entity_interface(const Entity_interface &other);
                                Entity_interface(Entity_interface &&other);

public:

  uint32_t                      get_id() const;
  
  
  // ** Life time ** //
  void                          destroy();                                    //!< Marks the entity for destruction, this wont happen till then end of the frame.
  bool                          is_valid() const;                             //!< Returns true if then entity is valid.
                                operator bool() const;                        //!< Shorthand for is_valid()

  // ** General Interface ** //
  void                          set_user_data(const uintptr_t user_data);     //!< Set user_data for the entity, can be a pointer, core doesn't callback on this.
  uintptr_t                     get_user_data() const;                        //!< Get the current user_data.
  
  void                          set_tags(const uint32_t set_tags);            //!< Set the tag bitfield.
  uint32_t                      get_tags() const;                             //!< Get the current bitfeld tags for this object.
  bool                          has_tag(const uint32_t tag_id) const;         //!< Check to see if the object has a bit set in the tags.
  void                          add_tag(const uint32_t add_tag);              //!< Add a bit to the bitfield.
  void                          remove_tag(const uint32_t tag);               //!< Remove a bit from the bitfield.
  
  void                          set_name(const char* set_name);               //!< Set the name of object.
  const char*                   get_name() const;                             //!< Returns a volatile pointer to the name.

  void                          set_transform(const Transform &transform);    //!< Set the transform for the entity.
  Transform                     get_transform() const;                        //!< Gets a copy of the current transform.
  
  void                          set_renderer(const Renderer &renderer);       //!< Set the entities renderer, an entity may only have one renderer.
  Renderer                      get_renderer() const;                         //!< Get the entities renderer.
  
  void                          set_rigidbody(const Rigidbody &rigidbody);    //!< Set the rigidbody for the entity.
  Rigidbody                     get_rigidbody() const;                        //!< Get the rigidbody for the entity. Returns by type.
  
  
  // ** Equality ** //
  bool                          operator ==(const Entity_interface &other) const;
  bool                          operator !=(const Entity_interface &other) const;
  
protected:

  void                          copy(const Entity_interface &other);
  void                          move(Entity_interface &other);
  util::generic_id              create_entity(const Core::World &world);
    
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};

} // ns


#endif // inc guard