#ifndef COLLISION_INCLUDED_EC62C163_8A3A_4D7C_98C9_54F303F02751
#define COLLISION_INCLUDED_EC62C163_8A3A_4D7C_98C9_54F303F02751


#include <core/entity/entity_ref.hpp>
#include <core/common/contact.hpp>


namespace Core {
namespace Collision_detail {

constexpr uint32_t get_max_contacts() { return 8; }

} // ns


class Collision
{
public:

  explicit        Collision();
  explicit        Collision(const Entity_ref ref,
                            const Contact contact[],
                            const size_t number_of_contacts);
  
  Entity_ref      get_entity() const;
  
  size_t          get_number_of_contacts() const;
  Contact         get_contact(const size_t i) const;
  
  const Contact*  begin() const;
  const Contact*  end() const;
  
  bool            has_hit() const;
                  operator bool() const;

private:

  Entity_ref  m_object;
  Contact     m_contacts[Collision_detail::get_max_contacts()];
  size_t      m_number_of_contacts;

};


} // ns


#endif // inc guard