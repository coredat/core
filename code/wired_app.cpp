#include <iostream>
#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <utils/directory.hpp>
#include <utils/obj_model_loader.hpp>
#include <math/math.hpp>
#include <transform/transform.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <data/entity/entity_data.hpp>
#include <data/texture_manager/texture_data.hpp>
#include <utils/timer.hpp>
#include <renderer/renderer.hpp>
#include <data/physics/physics.hpp>
#include <data/mesh_manager/mesh_data.hpp>
#include <entity_factory.hpp>
#include <data/actor/actor.hpp>
#include <resources.hpp>


namespace
{
  const math::mat4 proj = math::mat4_projection(800, 480, 0.1, 1000, math::quart_tau() * 0.6f);
}


int
main()
{
  sdl::window window(800, 480, false, "Wired");
  sdl::ogl_context ogl(window);
  sdl::input input;

  input.set_mouse_hold(true);
  
  std::cout << "Wired" << std::endl;
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  renderer::initialize();
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
  
  const std::string asset_path = util::get_resource_path() + "assets/";
  
  // Load resources
  Data::Mesh mesh_data;
  Data::mesh_init_data(&mesh_data, 2);
  {
    const std::string cube_filename = asset_path + "models/unit_cube.obj";
    Data::mesh_add_new(&mesh_data, 2, cube_filename.c_str(), Resource::Model::unit_cube);
    
    const std::string plane_filename = asset_path + "models/unit_plane.obj";
    Data::mesh_add_new(&mesh_data, 2, plane_filename.c_str(), Resource::Model::unit_plane);
  }
  
  Data::Texture texture_data;
  Data::texture_init_data(&texture_data, texture_data.size);
  {
    const std::string green_filename  = asset_path + "textures/dev_grid_green_512.png";
    Data::texture_add_new(&texture_data, 4, green_filename.c_str(), Resource::Texture::dev_green);
    
    const std::string red_filename    = asset_path + "textures/dev_grid_red_512.png";
    Data::texture_add_new(&texture_data, 4, red_filename.c_str(), Resource::Texture::dev_red);
    
    const std::string blue_filename   = asset_path + "textures/dev_grid_blue_512.png";
    Data::texture_add_new(&texture_data, 4, blue_filename.c_str(), Resource::Texture::dev_blue);
    
    const std::string orange_filename = asset_path + "textures/dev_grid_orange_512.png";
    Data::texture_add_new(&texture_data, 4, orange_filename.c_str(), Resource::Texture::dev_orange);
  }
  
  // Generic model *hurt*
  const util::obj_model model  = util::load_obj(asset_path + "models/unit_cube.obj");
  const util::gl_mesh mesh     = util::convert_to_open_gl_mesh(model.meshes.front());
  renderer::vertex_buffer ground_vbo(mesh.mesh_data);
  assert(ground_vbo.is_valid());
  
  
  Entity::Data world_entities(1024);
  Physics::World phy_world;
  Physics::world_init(&phy_world);
  
  
  Entity_factory::create_ground(&world_entities, &mesh_data, &texture_data);
//  Entity_id actor_entity = Entity_factory::create_actor(&world_entities, &mesh_data, &texture_data);
  Entity_id kine_actor = Entity_factory::create_kinematic_actor(&world_entities, &mesh_data, &texture_data);
  
  
//  for(int i = 0; i < 1; ++i)
//  {
//    auto ent = Entity_factory::create_random_cube(&world_entities, &mesh_data, nullptr);
//    world_entities.get_texture_data()[i + 2]  = texture_data.tex[1];
//    world_entities.get_mesh_data()[i + 2]     = mesh_data.vbo[0];
//  }
  
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
  
  Actor::Actor_base actor;
  
  util::timer frame_timer;
  frame_timer.start();
  
  // Foop
  while(!window.wants_to_quit())
  {
    const float delta_time = static_cast<float>(frame_timer.split()) / 1000.f;
  
    sdl::message_pump();
    renderer::clear();
    
//    /* Actor Controls */
//    {
//      math::vec3 move_force = math::vec3_zero();
//      const std::size_t actor_index = world_entities.find_index(actor_entity);
//      Physics::Rigidbody *rb = &world_entities.get_rigidbody_data()[actor_index];
//    
//      if(input.is_key_down(SDLK_w))
//      {
//        actor.move_forward(1);
//        const math::vec3 move_fwd = math::vec3_init(0,0,-1000 * delta_time);
//        move_force = math::vec3_add(move_force, move_fwd);
//      }
//      
//      if(input.is_key_down(SDLK_s))
//      {
//        actor.move_forward(-1);
//        const math::vec3 move_fwd = math::vec3_init(0,0,+1000 * delta_time);
//        move_force = math::vec3_add(move_force, move_fwd);
//      }
//      
//      if(input.is_key_down(SDLK_a))
//      {
//        actor.move_forward(-1);
//        const math::vec3 move_fwd = math::vec3_init(+1000 * delta_time, 0, 0);
//        move_force = math::vec3_add(move_force, move_fwd);
//      }
//      
//      if(input.is_key_down(SDLK_d))
//      {
//        actor.move_forward(-1);
//        const math::vec3 move_fwd = math::vec3_init(-1000 * delta_time, 0, 0);
//        move_force = math::vec3_add(move_force, move_fwd);
//      }
//      
//      if(input.is_key_down(SDLK_SPACE))
//      {
//        const math::vec3 move_fwd = math::vec3_init(0,+1000 * delta_time, 0);
//        move_force = math::vec3_add(move_force, move_fwd);
//      }
//      
//      float final_force[3];
//      math::vec3_to_array(move_force, final_force);
//      
//      Physics::rigidbody_apply_local_force(rb, final_force);
//      
//      if(input.get_mouse_delta_x())
//      {
//        actor.turn(input.get_mouse_delta_x());
//        
//        const std::size_t actor_index = world_entities.find_index(actor_entity);
//        Physics::Rigidbody *rb = &world_entities.get_rigidbody_data()[actor_index];
//        const float some_force[3] = {0, static_cast<float>(input.get_mouse_delta_x()) / 10.f, 0};
//        
//        Physics::rigidbody_apply_local_torque(rb, some_force);
//      }
//    }
//    /* Actor Controls */

    // Kinematic Controller
    {
      struct Kine_actor
      {
        bool is_grounded = false;
      } act;
    
      // Get transform of controller.
      const auto index = world_entities.find_index(kine_actor);
      const math::transform curr_trans = world_entities.get_transform_data()[index];
      
      // Cast ray downwards
      btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));
      btVector3 btTo(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - 2, math::vec3_get_z(curr_trans.position));
      btCollisionWorld::ClosestRayResultCallback feet_test(btFrom, btTo);
      
      phy_world.dynamics_world.rayTest(btFrom, btTo, feet_test);

      Renderer::debug_line(btFrom, btTo, btVector3(1, 1, 0));
      
      // If not hit anything then go downwards.
      // Gravity
      if(!feet_test.hasHit())
      {
        const math::vec3 down = math::vec3_init(0, -0.01, 0);
        
        math::transform new_trans = curr_trans;
        new_trans.position = math::vec3_add(curr_trans.position, down);
        
        world_entities.get_transform_data()[index] = new_trans;
      }
      else
      {
        act.is_grounded = true;
      }

      if(act.is_grounded)
      {
        math::vec3 accum_movement = math::vec3_zero();
        const math::transform move_trans = world_entities.get_transform_data()[index];
        
        if(input.is_key_down(SDLK_w))
        {
          const math::vec3 move_fwd = math::vec3_init(0,0,-1);
          accum_movement = math::vec3_add(accum_movement, move_fwd);
        }

        if(input.is_key_down(SDLK_s))
        {
          const math::vec3 move_fwd = math::vec3_init(0,0,+1);
          accum_movement = math::vec3_add(accum_movement, move_fwd);
        }
        
        if(input.is_key_down(SDLK_a))
        {
          const math::vec3 move_fwd = math::vec3_init(-1,0,0);
          accum_movement = math::vec3_add(accum_movement, move_fwd);
        }
        
        if(input.is_key_down(SDLK_d))
        {
          const math::vec3 move_fwd = math::vec3_init(+1,0,0);
          accum_movement = math::vec3_add(accum_movement, move_fwd);
        }
        
        if(math::vec3_length(accum_movement) != 0)
        {
          const math::vec3 norm_movement            = math::vec3_normalize(accum_movement);
          const math::vec3 rotated_movement         = math::quat_rotate_point(move_trans.rotation, norm_movement);
          const math::vec3 corrected_rotation       = math::vec3_init(math::vec3_get_x(rotated_movement), 0.f, math::vec3_get_z(rotated_movement)); // We're not interested in y movement.
          const math::vec3 norm_corrected_rotation  = math::vec3_normalize(corrected_rotation);
          const math::vec3 scaled_movement          = math::vec3_scale(norm_corrected_rotation, delta_time);
          
          world_entities.get_transform_data()[index].position = math::vec3_add(move_trans.position, scaled_movement);
        }
      }
      
      // Rotation
      {
        if(input.get_mouse_delta_x() != 0)
        {
          const math::transform rot_trans = world_entities.get_transform_data()[index];
          const float rot_rad = static_cast<float>(input.get_mouse_delta_x()) * delta_time;
          
          const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, rot_rad);
          world_entities.get_transform_data()[index].rotation = math::quat_multiply(rot_trans.rotation, rot);
        }
      }
      
      // Head
      {
        if(input.get_mouse_delta_y() != 0)
        {
          const math::transform rot_trans = world_entities.get_transform_data()[index];
          const float rot_rad = static_cast<float>(input.get_mouse_delta_y()) * delta_time;
          
          const math::vec3 head_axis = math::vec3_init(1,0,0);
          const math::vec3 adjusted_axis = math::quat_rotate_point(rot_trans.rotation, head_axis);
          
          const math::quat rot = math::quat_init_with_axis_angle(adjusted_axis, rot_rad);
          world_entities.get_transform_data()[index].rotation = math::quat_multiply(rot_trans.rotation, rot);
        }
      }
      
    }
    
    /* Ray test */
    {
      btVector3 btFrom(0, 10, 0);
      btVector3 btTo(0, -10, 0);
      btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

      phy_world.dynamics_world.rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld
      
      if(res.hasHit())
      {
        std::cout << "has_hit" << std::endl;
        res.m_collisionObject->getWorldTransform();
      }

    }
    /* Ray test */
    
    
    Physics::world_step(&phy_world, delta_time);
    
    static float time = 4;
    //time += delta_time / 2;
    
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
    
    //renderer::clear(false, true);
    
    math::mat4 wvp = math::mat4_multiply(math::mat4_id(), view, proj);
    Debug_line_renderer::render(math::mat4_get_data(wvp));
    
    input.think();
    window.flip_buffer();
  }
  
  return 0;
}