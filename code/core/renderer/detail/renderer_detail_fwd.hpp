#ifndef RENDERER_DETAIL_FWD_INCLUDED_8D061805_9559_4F60_9232_811179FF8F8F
#define RENDERER_DETAIL_FWD_INCLUDED_8D061805_9559_4F60_9232_811179FF8F8F


#include <core/renderer/renderer_fwd.hpp>


namespace Core {
namespace Renderer_utils {


Material_renderer
cast_to_material_renderer(const Renderer&);


Text_renderer
cast_to_text_renderer(const Renderer&);

  
} // ns
} // ns


namespace Core_detail {


struct Renderer_impl;


} // ns


#endif // inc guard