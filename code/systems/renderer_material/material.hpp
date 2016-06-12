#ifndef MATERIAL_INCLUDED_E602C4F3_5578_42FA_B28D_D156F43D393A
#define MATERIAL_INCLUDED_E602C4F3_5578_42FA_B28D_D156F43D393A


#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <utilities/generic_id.hpp>


namespace Material_renderer {


struct Material
{
  Ogl::Shader shader;

  Ogl::Texture map_01_id;
  Ogl::Uniform map_01;

  util::generic_id map_02_id;
  Ogl::Uniform map_02;

  util::generic_id map_03_id;
  Ogl::Uniform map_03;

  util::generic_id map_04_id;
  Ogl::Uniform map_04;

  Ogl::Uniform mat_world;
  Ogl::Uniform mat_proj;
  Ogl::Uniform mat_view_proj;
  Ogl::Uniform mat_world_view_proj;
}; // struct


/*
  Sets up a material with a valid shader input.
*/
void
create_material(Material *out_mat, const Ogl::Shader *shader);


} // ns


#endif // inc guard