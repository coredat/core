#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include <core/entity/entity_interface.hpp>


namespace Core {


/*!
  Entity represents an object on the screen.
  This is an owning object, if it goes out of scope it will
  destroy the entity.
*/
class Entity final : public Entity_interface
{
  friend class Entity_ref;
                                Entity(const Entity&) = delete;
  Entity&                       operator=(const Entity&) = delete;

public:

  explicit                      Entity();
  explicit                      Entity(Core::World &world);
  
                                ~Entity(); 
  
                                Entity(Entity&&);
  Entity&                       operator=(Entity&&);

                                operator Entity_ref() const;
  
}; // class


}; // ns


#endif // inc guard