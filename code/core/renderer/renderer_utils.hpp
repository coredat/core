#ifndef RENDERER_UTILS_INCLUDED_83E186FD_02F8_4788_ABB5_E88C2C207920
#define RENDERER_UTILS_INCLUDED_83E186FD_02F8_4788_ABB5_E88C2C207920


#include <core/common/core_fwd.hpp>


namespace Core {
namespace Renderer_utils {


Material_renderer
cast_to_material_renderer(const Renderer &renderer);


Text_renderer
cast_to_text_renderer(const Renderer &renderer);


} // ns
} // ns


#endif // inc guard