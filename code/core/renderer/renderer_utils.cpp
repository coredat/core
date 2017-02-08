#include <core/renderer/renderer_utils.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>


namespace Core {
namespace Renderer_utils {


Material_renderer
cast_to_material_renderer(const Renderer &renderer)
{
  assert(renderer.get_type() == Core::Renderer_type::material);
  
//  return Material_renderer(renderer.get_arg_01(), renderer.get_arg_02());
  return Material_renderer();
}


Text_renderer
cast_to_text_renderer(const Renderer &renderer)
{
  assert(renderer.get_type() == Core::Renderer_type::text);

  assert(false); // this is not the active path anymore.
  
  return Text_renderer();
}


} // ns
} // ns