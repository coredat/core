#ifndef COMPONENT_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35
#define COMPONENT_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35


#include "interface_fwd.hpp"
#include "detail/component_detail.hpp"
#include <data/world_data/world_data_fwd.hpp>
#include <core/entity_id.hpp>


#define COMPONENT_RTTI(id) static uint32_t get_rtti_id() { return id; }


namespace Core {


/*!
  Component is the base class for application components.
  this means we can we can write code that hook onto engine events.
  
  TODO:
  I don't like the virtual functions much. Means we loop through
  potenitally a lot of compoentns we will mostly be calling objects for no reason.
  I'd prefer to impliement an explicit decleration.
  
  set_callbacks(start | update | end);
  
  get_rtti() should prolly be a static. Make the interface a little cleaner.
*/
class Component
{
  // TODO: Privacy.
  // Need to do better I think.
  // This is because we manage the component memory manually.
  // Duplicating an object isn't something that should ever happen.
  // This is a good argument (weridly) to move this to scripting.
  Component(const Component&)             = delete;
  Component(Component&&)                  = delete;
  Component& operator=(const Component&)  = delete;
  Component& operator=(const Component&&) = delete;

public:

  explicit            Component()  {}
  virtual             ~Component() {}
  
  virtual uint32_t    get_rtti() const            { return 0; };
  
  // Event hooks
  virtual void        on_start()                  {} //!< Called when the object is ready.
  virtual void        on_update(const float dt)   {} //!< Called when the object is being updated.
  virtual void        on_end()                    {} //!< Called when the object is dead
  
  virtual void        on_event(const uint32_t event_id, const void *data, const uint32_t size_of_data) {}

  virtual void        on_collision(const Entity *entity) {}

  Core::Entity        get_entity() const; //! < Returns an object of the entity.
  
  virtual void        on_collision(const Core::Entity &entity) {};
  
//protected: // TODO: Private this is because I haven't exposed enough functionality through interface, and entity  needs a setter here.

  COMPONENT_MEMBERS
  
}; // class


} // ns


#endif // inc guard