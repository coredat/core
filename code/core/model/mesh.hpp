#ifndef MESH_INCLUDED_3B02E00E_C938_4A7E_93EE_52E659913C99
#define MESH_INCLUDED_3B02E00E_C938_4A7E_93EE_52E659913C99


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Mesh final
{
public:

  explicit            Mesh();
                      ~Mesh();
  
                      Mesh(const Mesh&);
                      Mesh(Mesh&&);

  math::aabb          get_aabb() const;
  uint32_t            get_number_of_vertices() const;
  bool                is_valid() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


} // ns



#endif // inc guard