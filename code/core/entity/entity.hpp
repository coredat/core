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
    
  void                          set_collider(const Core::Collider &collider); //! Set the collider of the entity.
  Core::Collider                get_collider() const;                         //! Gets the collider of the entity.

  void                          set_rigidbody_properties(const Core::Rigidbody_properties rb_props);  //!< Sets the Rigidbody properties.
  Core::Rigidbody_properties    get_rigidbody_properties() const;                                     //!< Returns the rigidbody properties.

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