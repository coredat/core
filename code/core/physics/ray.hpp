#ifndef RAY_INCLUDED_0CE26212_29A8_4A9C_BC17_51438D62F208
#define RAY_INCLUDED_0CE26212_29A8_4A9C_BC17_51438D62F208


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


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


  explicit              Ray(Core::World &world, // TODO: Const
                            const math::vec3 from,
                            const math::vec3 to,
                            const Search search = Search::first);
  
                        ~Ray();
  
  bool                  has_hit() const;
  uint32_t              number_of_hits() const;

  Entity_ref            get_entity(const uint32_t i) const;
  
private:
  
  struct Impl;
  std::unique_ptr<Impl>   m_impl;

}; // class


} // ns


#endif // inc guard