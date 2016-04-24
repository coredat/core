#ifndef RAY_INCLUDED_0CE26212_29A8_4A9C_BC17_51438D62F208
#define RAY_INCLUDED_0CE26212_29A8_4A9C_BC17_51438D62F208


#include <core/entity/entity.hpp>
#include <math/vec/vec3.hpp>
#include <stddef.h>
#include <vector> // Kill!


/*
  TODO
  --
  Better to make this pimpl.
  But waiting for a memory allocator to suit.
  Also get rid of std::vector.
*/


namespace Core {


class Ray final
{
public:

  enum class Search
  {
    first,
  }; // enum


  explicit              Ray(const math::vec3 from, const math::vec3 to, const Search search);
  
  bool                  has_hit() const                             { return m_results.size();          }
  uint32_t                number_of_hits() const                    { return (uint32_t)m_results.size();  }

  Entity                get_entity(const uint32_t i) const;
  math::vec3            get_position_of_hit(const uint32_t i) const   { return m_results.at(i).hit;       }
  math::vec3            get_normal_of_hit(const uint32_t i) const     { return m_results.at(i).normal;    }
  
private:
  
  struct Ray_result
  {
    Core::Entity_id id;
    math::vec3 hit;
    math::vec3 normal;
  };
  
  std::vector<Ray_result> m_results;

}; // class


} // ns


#endif // inc guard