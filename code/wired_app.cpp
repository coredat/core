#include <iostream>
#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <utils/directory.hpp>
#include <utils/obj_model_loader.hpp>
#include <math/math.hpp>
#include <transform/transform.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <data/entity/entity_data.hpp>
#include <data/texture_manager/texture_data.hpp>



namespace
{
  const math::mat4 proj  = math::mat4_projection(800, 480, 0.1, 1000, math::quart_tau() * 0.6f);
}


int
main()
{
  sdl::window window(800, 480, false, "Wired");
  sdl::ogl_context ogl(window);
  
  std::cout << "Wired" << std::endl;
  
  renderer::initialize();
  Simple_renderer::initialize();
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  const std::string asset_path = util::get_resource_path() + "assets/";
  
  Data::Texture texture_data;
  const std::string filename = asset_path + "textures/dev_grid_green_512.png";
  Data::texture_add_new(&texture_data, 4, filename.c_str());
  
  // Generic model *hurt*
  const util::obj_model model  = util::load_obj(asset_path + "models/unit_cube.obj");
  const util::gl_mesh mesh     = util::convert_to_open_gl_mesh(model.meshes.front());
  renderer::vertex_buffer ground_vbo(mesh.mesh_data);
  assert(ground_vbo.is_valid());
  
  Data::Entity data;
  
  // *hurt* better entity loop thing.
  for(uint32_t i = 0; i < ENTITY_POOL; ++i)
  {
    data.entity_id[i] = Entity_id{0,0};
    data.transform[i] = math::transform_init(math::vec3_init((static_cast<float>(rand()%30) - 15) / 5.0f,
                                                             (static_cast<float>(rand()%30) - 15) / 5.0f,
                                                             (static_cast<float>(rand()%30) - 15) / 5.0f),
                                             math::vec3_init((static_cast<float>(rand()%8) + 1) / 5.0f,
                                                             (static_cast<float>(rand()%8) + 1) / 5.0f,
                                                             (static_cast<float>(rand()%8) + 1) / 5.0f),
                                             math::quat_init());
    data.vbo[i] = ground_vbo;
    data.texture[i] = texture_data.tex[0];
  }
  
  data.number_of_entities = 64;
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  renderer_nodes.resize(data.number_of_entities);

  // Copy vbo's into node. *hurt*
  for(std::size_t i = 0; i < data.number_of_entities; ++i)
  {
    renderer_nodes.at(i).vbo = data.vbo[i];
    renderer_nodes.at(i).diffuse_id = data.texture[i];
  }
  
  // Foop
  while(!window.wants_to_quit())
  {
    sdl::message_pump();
    renderer::clear();
    
    static float time = 0;
    time += 0.004f;
    
    const float x = math::sin(time) * 9;
    const float z = math::cos(time) * 9;
    
    const math::mat4 view  = math::mat4_lookat(math::vec3_init(x,3.4f,z), math::vec3_zero(), math::vec3_init(0, 1, 0));
    
    const math::mat4 view_proj = math::mat4_multiply(view, proj); // *hurt* camaera or such.
    Transform::transforms_to_wvp_mats(data.transform,
                                      data.number_of_entities,
                                      view_proj,
                                      renderer_nodes[0].wvp,
                                      renderer_nodes.size(),
                                      sizeof(Simple_renderer::Node));

    Transform::transforms_to_world_mats(data.transform,
                                        data.number_of_entities,
                                        renderer_nodes[0].world_mat,
                                        renderer_nodes.size(),
                                        sizeof(Simple_renderer::Node));
      
    Simple_renderer::render_nodes_fullbright(renderer_nodes.data(), renderer_nodes.size());
    
    window.flip_buffer();
  }

  return 0;
}