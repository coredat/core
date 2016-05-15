#ifndef MATERIAL_RENDERER_INCLUDED_8DD3647B_EE2F_4233_95CC_3533DB86CC5E
#define MATERIAL_RENDERER_INCLUDED_8DD3647B_EE2F_4233_95CC_3533DB86CC5E


#include <graphics_api/mesh.hpp>
#include <graphics_api/ogl/ogl_shader.hpp>
#include <math/mat/mat_types.hpp>
#include <stdint.h>


namespace Material_renderer {


struct Material
{
  Ogl::Shader shader;
  int index_01;
};


struct Draw_call
{
  float               world_matrix[16];
  Graphics_api::Mesh  mesh;
};


void
initialize();


void
reset();


void
render(const math::mat4 view_proj_mat[16],
       const Material material,
       const Draw_call calls[],
       const uint32_t number_of_calls);


} // ns


#endif // inc guard