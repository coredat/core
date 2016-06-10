#ifndef RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604
#define RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604


#include <core/renderer/renderer_type.hpp>
#include <utilities/generic_id.hpp>
#include <stdint.h>


namespace Core {


class Renderer final
{
public:

  explicit                Renderer(const Renderer_type renderer_type);
  
  Renderer_type           get_type() const;
  const char *            get_type_name() const;
  
private:

  const Renderer_type     m_renderer_type = Renderer_type::material;
  util::generic_id        m_entity_id = util::generic_id_invalid();

};


} // ns


#endif // inc guard