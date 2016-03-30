#ifndef ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899
#define ENTITY_INCLUDED_EF96529C_E89E_4D4E_9DC2_9988B4774899


#include "entity_interface.hpp"


namespace Core {


/*!
  Entity is how we access the properties of an entity.
  This abstracts how all the data is handled underneath.
*/
class Entity final : public Entity_interface
{
public:

  explicit                      Entity();
  
  void                          destroy();
  

}; // class


}; // ns


#endif // inc guard