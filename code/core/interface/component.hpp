#ifndef COMPONENT_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35
#define COMPONENT_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35


#include "interface_fwd.hpp"
#include "detail/component_detail.hpp"
#include <data/data_fwd.hpp>
#include <core/entity_id.hpp>


namespace Core {


/*!
  Component is the base class for application components.
  this means we can we can write code that hook onto engine events.
  
  TODO:
  I don't like the virtual functions much. Means we loop through
  potenitally a lot of compoentns we will mostly be calling objects for no reason.
  I'd prefer to impliement an explicit decleration.
  
  set_callbacks(start | update | end);
*/
class Component
{
public:

  explicit            Component()  {}
  virtual             ~Component() {} //!< Dtor is not guarteed to be called.
  
  virtual uint32_t    get_rtti() const = 0;
  
  // Event hooks
  virtual void        on_start()                  {} //!< Called when the object is ready.
  virtual void        on_update(const float dt)   {} //!< Called when the object is being updated.
  virtual void        on_end()                    {} //!< Called when the object is dead
  
  virtual void        on_event(const uint32_t event_id, const void *data, const size_t size_of_data) {}

  Core::Entity        get_entity() const; //! < Returns an object of the entity.
  
//protected: // TODO: Private this is because I haven't exposed enough functionality through interface, and entity  needs a setter here.

  COMPONENT_MEMBERS
  
}; // class


} // ns


#endif // inc guard