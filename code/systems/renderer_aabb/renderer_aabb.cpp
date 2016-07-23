#include "renderer_aabb.hpp"
#include <Graphics_api/ogl/ogl_shader.hpp>
#include <Graphics_api/ogl/ogl_shader_uniform.hpp>
#include <Graphics_api/ogl/ogl_vertex_buffer.hpp>
#include <Graphics_api/ogl/ogl_vertex_format.hpp>
#include <utilities/obj_model_loader.hpp>
#include <utilities/directory.hpp>
#include <assert.h>


namespace
{
  Ogl::Shader aabb_shader;
  Ogl::Vertex_buffer vertex_buffer;
  Ogl::Vertex_format vertex_format;
}


namespace Aabb_renderer {


void
initialize()
{
  if(!Ogl::shader_is_valid(&aabb_shader))
  {
    const char *vs_shader = R"(
      #version 150 core
    
      in vec3 vs_in_position;
      in vec3 vs_not_used_norm;
      in vec2 vs_not_used_uv;
    
      uniform mat4 uni_wvp_mat;
    
      void
      main()
      {
        gl_Position = uni_wvp_mat * vec4(vs_in_position, 1.0);
      }
    )";
    
    const char *ps_shader = R"(
      #version 150 core
    
      vec4 out_color;
    
      void
      main()
      {
        out_color = vec4(1,1,1,1);
      }
    
    )";
    
    Ogl::shader_create(&aabb_shader, vs_shader, nullptr, ps_shader);
    assert(Ogl::shader_is_valid(&aabb_shader));
  }
  
  if(!Ogl::vertex_buffer_is_valid(vertex_buffer))
  {
//    char filepath[2048];
//    memset(filepath, 0, sizeof(filepath));
//    
//    strcat(filepath, util::get_resource_path().c_str());
//    strcat(filepath, "assets/models/unit_cube.obj");
//    
//    util::obj_model model = util::load_obj(filepath);
    
  }

  if(!Ogl::vertex_format_is_valid(&vertex_format))
  {
    constexpr uint32_t attr_count = 1;
    
    Ogl::Attribute_desc desc[attr_count] = {
      Ogl::Attribute_desc{"in_vs_position", Ogl::Attr_type::FLOAT3},
    };
    
    Ogl::vertex_format_load(&vertex_format, desc, attr_count);
    assert(Ogl::vertex_format_is_valid(&vertex_format));
  }
}


void
render(const math::aabb boxes[], const uint32_t number_of_boxes)
{
}


} // ns