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
#include <data/mesh_manager/mesh_data.hpp>
#include <data/physics_world/physics_world.hpp>
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
  
  Data::Entity data;
  
  Entity::Data world_entities(128);
  Physics::World phy_world;

  Entity_factory::create_ground(&world_entities);
  Entity_factory::create_random_cube(&world_entities);
  
  Physics::world_add_rigidbody(&phy_world, world_entities.get_collider_data()[0], &world_entities.get_rigidbody_data()[0]);
  Physics::world_add_rigidbody(&phy_world, world_entities.get_collider_data()[1], &world_entities.get_rigidbody_data()[1]);
  
  world_entities.get_texture_data()[0] = texture_data.tex[0];
  world_entities.get_texture_data()[1] = texture_data.tex[1];
  
  world_entities.get_mesh_data()[0] = mesh_data.vbo[1];
  world_entities.get_mesh_data()[1] = mesh_data.vbo[0];
  
  // *hurt* better entity loop thing.
//  for(uint32_t i = 0; i < ENTITY_POOL; ++i)
//  {
//    data.number_of_entities++;
//    
//    data.entity_id[i] = Entity_id{0,i};
//    
//    const float scale_x = (static_cast<float>(rand()%200) + 10) / 200.0f;
//    const float scale_y = (static_cast<float>(rand()%200) + 10) / 200.0f;
//    const float scale_z = (static_cast<float>(rand()%200) + 10) / 200.0f;
//
//    const float pos_x = (static_cast<float>(rand()%100) + 10) / 20.0f;
//    const float pos_y = (static_cast<float>(rand()%100)) / 20.0f;
//    const float pos_z = (static_cast<float>(rand()%100) + 10) / 20.0f;
//    
//    data.transform[i] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
//                                             math::vec3_init(scale_x, scale_y, scale_z),
//                                             math::quat_init());
//    
//    data.texture[i] = texture_data.tex[rand() % 4];
//    data.vbo[i] = mesh_data.vbo[0];
//    
//    data.collider[i].type = Physics::Collider_type::cube;
//    data.collider[i].mass = (rand() % 5 + 1);
//    data.collider[i].info.cube.extents[0] = scale_x * 0.5f;
//    data.collider[i].info.cube.extents[1] = scale_y * 0.5f;
//    data.collider[i].info.cube.extents[2] = scale_z * 0.5f;
//    //data.rigidbodies[i].motion_state.reset(new Data_detail::Motion_state(data.entity_id[i], &data));
//    
//    //Data::physics_add_rigidbody(&phy_world, data.collider[i], &data.rigidbodies[i]);
//  }
  
  //data.number_of_entities = 128;
  //data.entity_id[4] = Entity_id{1,0};
  
  //Data::physics_add_rigidbody(&phy_world, data.collider[0], &data.rigidbodies[0]);
  
  // Set ground
//  {
//    data.vbo[0] = mesh_data.vbo[1];
//    data.transform[0] = math::transform_init(math::vec3_zero(), math::vec3_init(10,10,10), math::quat_init());
//    data.collider[0].type = Physics::Collider_type::static_plane;
//    data.collider[0].info.static_plane.normal[0] = 0;
//    data.collider[0].info.static_plane.normal[1] = 1;
//    data.collider[0].info.static_plane.normal[2] = 0;
//    data.collider[0].info.static_plane.offset = 0;
//    data.collider[0].mass = 0;
//  }
  
  
  // Add rigidbodies
//  for(std::size_t i = 0; i < data.number_of_entities; ++i)
//  {
//    Physics::world_add_rigidbody(&phy_world, data.collider[i], &data.rigidbodies[i]);
//  }
  
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
    
    // Move a block
    {
//      if(input.is_key_down(SDLK_w))
//      {
//        math::transform trans;
//        Data::entity_get_transform(&data, data.number_of_entities, Entity_id{1,0}, &trans);
//        
//        const math::vec3 fwd = math::quat_rotate_point(trans.rotation, math::vec3_init(0,0,-0.1f));
//        trans.position = math::vec3_add(trans.position, fwd);
//        
//        Data::entity_set_transform(&data, data.number_of_entities, Entity_id{1,0}, &trans);
//      }
//      if(input.is_key_down(SDLK_s))
//      {
//        math::transform trans;
//        Data::entity_get_transform(&data, data.number_of_entities, Entity_id{1,0}, &trans);
//        
//        const math::vec3 fwd = math::quat_rotate_point(trans.rotation, math::vec3_init(0,0,0.1f));
//        trans.position = math::vec3_add(trans.position, fwd);
//        
//        Data::entity_set_transform(&data, data.number_of_entities, Entity_id{1,0}, &trans);
//      }
    }
    
    static float time = 4;
    //time += 0.004f;
    
    const float x = math::sin(time) * 9;
    const float z = math::cos(time) * 9;
    
    const float eye_pos[3] = {x, 3.4f, z};
    
    const math::mat4 view  = math::mat4_lookat(math::vec3_init(x,3.4f,z), math::vec3_zero(), math::vec3_init(0, 1, 0));
    
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