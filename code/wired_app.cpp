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
#include <data/physics/physics.hpp>
#include <data/mesh_manager/mesh_data.hpp>
#include <entity_factory.hpp>



namespace
{
  const math::mat4 proj  = math::mat4_projection(800, 480, 0.1, 1000, math::quart_tau() * 0.6f);
}


int
main()
{
  sdl::window window(800, 480, false, "Wired");
  sdl::ogl_context ogl(window);
  sdl::input input;
  
  std::cout << "Wired" << std::endl;
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  
  renderer::initialize();
  Simple_renderer::initialize();
  
  const std::string asset_path = util::get_resource_path() + "assets/";
  
  Data::Mesh mesh_data;
  Data::mesh_init_data(&mesh_data, 4);
  {
    const std::string cube_filename = asset_path + "models/unit_cube.obj";
    Data::mesh_add_new(&mesh_data, 4, cube_filename.c_str());
    
    const std::string plane_filename = asset_path + "models/unit_plane.obj";
    Data::mesh_add_new(&mesh_data, 4, plane_filename.c_str());
  }
  
  
  Data::Texture texture_data;
  Data::texture_init_data(&texture_data, 4);
  {
    const std::string green_filename  = asset_path + "textures/dev_grid_green_512.png";
    Data::texture_add_new(&texture_data, 4, green_filename.c_str());
    
    const std::string red_filename    = asset_path + "textures/dev_grid_red_512.png";
    Data::texture_add_new(&texture_data, 4, red_filename.c_str());
    
    const std::string blue_filename   = asset_path + "textures/dev_grid_blue_512.png";
    Data::texture_add_new(&texture_data, 4, blue_filename.c_str());
    
    const std::string orange_filename = asset_path + "textures/dev_grid_orange_512.png";
    Data::texture_add_new(&texture_data, 4, orange_filename.c_str());
  }
  
  // Generic model *hurt*
  const util::obj_model model  = util::load_obj(asset_path + "models/unit_cube.obj");
  const util::gl_mesh mesh     = util::convert_to_open_gl_mesh(model.meshes.front());
  renderer::vertex_buffer ground_vbo(mesh.mesh_data);
  assert(ground_vbo.is_valid());
    
  Entity::Data world_entities(1024);
  Physics::World phy_world;
  
  Entity_factory::create_ground(&world_entities);

  world_entities.get_texture_data()[0]  = texture_data.tex[0];
  world_entities.get_mesh_data()[0]     = mesh_data.vbo[1];
  
  for(std::size_t i = 1; i < 64; ++i)
  {
    Entity_factory::create_random_cube(&world_entities);
    world_entities.get_texture_data()[i] = texture_data.tex[1];
    world_entities.get_mesh_data()[i] = mesh_data.vbo[0];
  }
  
  Physics::world_add_rigidbodies(&phy_world,
                                 world_entities.get_collider_data(),
                                 world_entities.size(),
                                 world_entities.get_rigidbody_data(),
                                 world_entities.size());
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  renderer_nodes.resize(world_entities.size());

  // Copy vbo's into node. *hurt*
  for(std::size_t i = 0; i < world_entities.size(); ++i)
  {
    renderer_nodes.at(i).vbo = world_entities.get_mesh_data()[i];
    renderer_nodes.at(i).diffuse_id = world_entities.get_texture_data()[i];
  }
  
  // Foop
  while(!window.wants_to_quit())
  {
    sdl::message_pump();
    renderer::clear();
    
    
        Physics::world_step(&phy_world, 0.01);
    
    static float time = 4;
    //time += 0.004f;
    
    const float x = math::sin(time) * 9;
    const float z = math::cos(time) * 9;
    
    const float eye_pos[3] = {x, 3.4f, z};
    
    const math::mat4 view  = math::mat4_lookat(math::vec3_init(x, 3.4f, z),
                                               math::vec3_zero(),
                                               math::vec3_init(0, 1, 0));
    
    const math::mat4 view_proj = math::mat4_multiply(view, proj); // *hurt* camaera or such.
    Transform::transforms_to_wvp_mats(world_entities.get_transform_data(),
                                      world_entities.size(),
                                      view_proj,
                                      renderer_nodes[0].wvp,
                                      renderer_nodes.size(),
                                      sizeof(Simple_renderer::Node));

    Transform::transforms_to_world_mats(world_entities.get_transform_data(),
                                        world_entities.size(),
                                        renderer_nodes[0].world_mat,
                                        renderer_nodes.size(),
                                        sizeof(Simple_renderer::Node));
      
    Simple_renderer::render_nodes_fullbright(renderer_nodes.data(), renderer_nodes.size());
    //Simple_renderer::render_nodes_directional_light(renderer_nodes.data(), renderer_nodes.size(), &eye_pos[0]);
    
    window.flip_buffer();
  }

  return 0;
}