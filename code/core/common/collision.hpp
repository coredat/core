#ifndef COLLISION_INCLUDED_EC62C163_8A3A_4D7C_98C9_54F303F02751
#define COLLISION_INCLUDED_EC62C163_8A3A_4D7C_98C9_54F303F02751


#include <core/entity/entity_ref.hpp>
#include <core/common/contact.hpp>


namespace Core {
namespace Core_detail {

constexpr uint32_t get_max_contacts() { return 4; }

} // ns


class Collision
{
public:

  explicit        Collision(const Entity_ref ref,
                            const Contact contact[],
                            const size_t number_of_contacts);
  
  Entity_ref      get_entity() const;
  
  size_t          get_number_of_contacts() const;
  Contact         get_contact(const size_t i) const;
  
  bool            has_hit() const;
                  operator bool() const;

private:

  const Entity_ref  m_object;
  const Contact     m_contacts[Core_detail::get_max_contacts()];
  const size_t      m_number_of_contacts;

};


} // ns


#endif // inc guard