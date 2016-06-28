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
  
  return Material_renderer(renderer.get_arg_01(), renderer.get_arg_02());
}


Text_renderer
cast_to_text_renderer(const Renderer &renderer)
{
  assert(false); // no impl yet
  return Text_renderer();
}


} // ns
} // ns