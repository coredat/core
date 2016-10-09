#include <core/common/collision.hpp>
#include <utilities/logging.hpp>


namespace Core {


Collision::Collision()
: Collision(Core::Entity_ref(), nullptr, 0)
{
}


Collision::Collision(const Entity_ref ref,
                     const Contact contact[],
                     const size_t number_of_contacts)
: m_object(ref)
, m_contacts
  {
    number_of_contacts > 0 ? contact[0] : Contact(),
    number_of_contacts > 1 ? contact[1] : Contact(),
    number_of_contacts > 2 ? contact[2] : Contact(),
    number_of_contacts > 3 ? contact[3] : Contact(),
    number_of_contacts > 4 ? contact[4] : Contact(),
    number_of_contacts > 5 ? contact[5] : Contact(),
    number_of_contacts > 6 ? contact[6] : Contact(),
    number_of_contacts > 7 ? contact[7] : Contact(),
  }
, m_number_of_contacts(math::min(number_of_contacts, Collision_detail::get_max_contacts()))
{
  
}


Entity_ref
Collision::get_entity() const
{
  return m_object;
}


size_t
Collision::get_number_of_contacts() const
{
  return m_number_of_contacts;
}


Contact
Collision::get_contact(const size_t i) const
{
  if(i > Collision_detail::get_max_contacts())
  {
    LOG_ERROR("Requesting contact that is out of range.");
  }

  return m_contacts[math::min(i, Collision_detail::get_max_contacts())];
}


const Contact*
Collision::begin() const
{
  return &m_contacts[0];
}


const Contact*
Collision::end() const
{
  return &m_contacts[m_number_of_contacts];
}


bool
Collision::has_hit() const
{
  return !!m_object;
}


Collision::operator bool() const
{
  return has_hit();
}


} // ns