#include <iostream>
#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <utils/directory.hpp>
#include <utils/obj_model_loader.hpp>
#include <math/math.hpp>
#include <transform/transform.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>


#define ENTITY_POOL 128


namespace
{

  struct Entity_data
  {
    math::transform transform[ENTITY_POOL];
    renderer::vertex_buffer vbo[ENTITY_POOL];
    
    std::size_t number_of_entities = 0;
  };


  const math::mat4 proj  = math::mat4_projection(800, 480, 0.1, 1000, math::quart_tau());
  const math::mat4 view  = math::mat4_lookat(math::vec3_one(), math::vec3_zero(), math::vec3_init(0, 1, 0));
  const math::mat4 world = math::mat4_id();
}

int
main()
{
  sdl::window window(800, 480, false, "Wired");
  sdl::ogl_context ogl(window);
  
  renderer::initialize();
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  
  Entity_data data;
  
  for(uint32_t i = 0; i < ENTITY_POOL; ++i)
  {
    data.transform[i] = math::transform_init(math::vec3_zero(), math::vec3_one(), math::quat_init());
  }
  
  data.number_of_entities = 2;
  
  // Transform data
  std::vector<float> wvps;
  wvps.resize(data.number_of_entities * 16);
  
  Transform::transforms_to_wvp(data.transform, data.number_of_entities, proj, view, wvps.data(), wvps.size() / 16);
  
  const std::string asset_path = util::get_resource_path() + "assets/";
  const auto code = renderer::shader_utils::get_shader_code_from_tagged_file(asset_path + "shaders/basic_fullbright.ogl");
  renderer::shader dir_light(code);
  assert(dir_light.is_valid());

  renderer::vertex_format vertex_fmt({
    renderer::attr_format_desc{"in_vs_position",      renderer::attr_type::FLOAT3},
    renderer::attr_format_desc{"in_vs_texture_coord", renderer::attr_type::FLOAT2},
    renderer::attr_format_desc{"in_vs_normal",        renderer::attr_type::FLOAT3},
  });
  assert(vertex_fmt.is_valid());
  
  const util::obj_model model  = util::load_obj(asset_path + "models/unit_plane.obj");
  const util::gl_mesh mesh     = util::convert_to_open_gl_mesh(model.meshes.front());
  renderer::vertex_buffer ground_vbo(mesh.mesh_data);
  assert(ground_vbo.is_valid());
  
  //const math::vec3 light = math::vec3_init(0.707, 0.707, 0);
  
  // Foop
  while(!window.wants_to_quit())
  {
    sdl::message_pump();
    renderer::clear();
    renderer::reset();
    
    const math::mat4 wvp = math::mat4_multiply(world, view, proj);
    
    dir_light.set_raw_data("wvp", wvps.data(), sizeof(wvp));
    
//    dir_light.set_raw_data("worldMat", &world, sizeof(world));
//    dir_light.set_raw_data("viewMat", &view, sizeof(view));
//    dir_light.set_raw_data("projMat", &proj, sizeof(proj));
//    dir_light.set_raw_data("gDirectionalLight", &light, sizeof(light));
    
    renderer::draw(dir_light, vertex_fmt, ground_vbo);
    
    window.flip_buffer();
  }

  return 0;
}