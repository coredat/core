#ifndef ENTITY_REF_INCLUDED_6EB9BAB7_7AB4_4092_9FBC_BD003D9C8F83
#define ENTITY_REF_INCLUDED_6EB9BAB7_7AB4_4092_9FBC_BD003D9C8F83


#include <core/entity/entity_interface.hpp>


namespace Core {


/*!
  Entity_ref is a non owning entity.
  -- 
  - Ref will eventually become const only access.
*/
class Entity_ref final : public Entity_interface
{
  friend class Core::Entity;

public:

  explicit                      Entity_ref();
  explicit                      Entity_ref(const Entity &entity);
  explicit                      Entity_ref(const Core_detail::Entity_id id);
  explicit                      Entity_ref(const uint32_t id);
  
                                ~Entity_ref();
  
                                Entity_ref(const Entity_ref &entity);
  Entity_ref&                   operator=(const Entity_ref &entity);

private:
  
}; // class


} // ns


#endif // inc guard