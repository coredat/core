#ifndef RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604
#define RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604


#include <core/common/core_types.hpp>
#include <core/common/core_fwd.hpp>
#include <core/renderer/renderer_type.hpp>
#include <core/renderer/detail/renderer_detail_fwd.hpp>
#include <core/entity/detail/entity_detail_fwd.hpp>


namespace Core {


/*
  This is an interchange class.
  You should use the specific renderer types.
*/
class Renderer final
{
public:

  explicit        Renderer();
  explicit        Renderer(const uint32_t id); // private
  
  uint32_t        get_type() const;

  
private:

  uint32_t        m_entity_id;

};


} // ns


#endif // inc guard