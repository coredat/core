#include <systems/renderer_material/material.hpp>
#include <assert.h>


namespace Material_renderer {


void
create_material(Material *out_mat, const Ogl::Shader *shader)
{
  assert(out_mat && shader);
  assert(Ogl::shader_is_valid(shader));
  
//      material->shader = shd;
//      
//      // Get the texture maps
//      {
//        Ogl::shader_uniforms_get_uniform_index(&material->map_01, &uniforms, "uni_map_01");
//        Ogl::shader_uniforms_get_uniform_index(&material.map_02, &uniforms, "uni_map_02");
//        Ogl::shader_uniforms_get_uniform_index(&material.map_03, &uniforms, "uni_map_03");
//        Ogl::shader_uniforms_get_uniform_index(&material.map_04, &uniforms, "uni_map_04");
//      }
//      
//      // Get mats
//      {
//        Ogl::shader_uniforms_get_uniform_index(&material.mat_world,           &uniforms, "uni_world_mat");
//        Ogl::shader_uniforms_get_uniform_index(&material.mat_proj,            &uniforms, "uni_proj_mat");
//        Ogl::shader_uniforms_get_uniform_index(&material.mat_view_proj,       &uniforms, "uni_vp_mat");
//        Ogl::shader_uniforms_get_uniform_index(&material.mat_world_view_proj, &uniforms, "uni_wvp_mat");
//      }
  
}


} // ns