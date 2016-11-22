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

  util::generic_id map_01_id = 0;
  Ogl::Texture map_01;
  Ogl::Uniform map_01_index;

  util::generic_id map_02_id = 0;
  Ogl::Uniform map_02_index;

  util::generic_id map_03_id = 0;
  Ogl::Uniform map_03_index;

  util::generic_id map_04_id = 0;
  Ogl::Uniform map_04_index;

  Ogl::Uniform light_array;

  // Matrices
  Ogl::Uniform mat_world;
  Ogl::Uniform mat_proj;
  Ogl::Uniform mat_view_proj;
  Ogl::Uniform mat_world_view_proj;
  
  // Lighting
  Ogl::Uniform vec3_eye_position;
  
  // Time
  Ogl::Uniform uni_dt;
  float dt;
  
  Ogl::Uniform uni_total_time;
  float total_time;
  
  Ogl::Uniform color;
  float color_data[4] = {0.f, 0.f, 0.f, 0.f};
}; // struct


// Simple huristic to try and reduce state
// changes in the material system.
struct Material_id
{
  uint32_t material_instance : 8;
  uint32_t texture_map_01_id : 16;
  uint32_t shader_id : 8;
};


inline bool
operator < (const Material_id left, const Material_id right)
{
  const uint32_t lhs = *reinterpret_cast<const uint32_t*>(&left);
  const uint32_t rhs = *reinterpret_cast<const uint32_t*>(&right);
  
  return lhs < rhs;
}


inline bool
operator > (const Material_id left, const Material_id right)
{
  const uint32_t lhs = *reinterpret_cast<const uint32_t*>(&left);
  const uint32_t rhs = *reinterpret_cast<const uint32_t*>(&right);
  
  return lhs > rhs;
}


inline bool
operator == (const Material_id left, const Material_id right)
{
  const uint32_t lhs = *reinterpret_cast<const uint32_t*>(&left);
  const uint32_t rhs = *reinterpret_cast<const uint32_t*>(&right);
  
  return lhs == rhs;
}

/*
  Sets up a material with a valid shader input.
*/
void
create_material(Material *out_mat, const Ogl::Shader *shader);


} // ns


#endif // inc guard