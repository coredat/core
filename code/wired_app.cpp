#include <application/resources.hpp>
#include <application/entity_factory.hpp>
#include <application/common/ids_object_tags.hpp>

#include <systems/transform/transformations.hpp>
#include <systems/entity/generic_id.hpp>
#include <systems/environment/environment.hpp>

#include <math/math.hpp>
#include <utilities/timer.hpp>
#include <core/camera/camera.hpp>
#include <iostream>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>

#include <application/engine/application_host.hpp>
#include <application/engine/application_common.hpp>
#include <application/engine/application_graphics.hpp>

#include <core/world/world.hpp>
#include <vector>

#include <graphics_api/clear.hpp>

#ifdef main
#undef main
#endif

#ifndef UNIT_TEST
int
main(int argc, char *argv[])
{
  const std::string title = "Application";
  Environment::Window window;
  Environment::window_create(&window, 800, 480, false, title.c_str());

  Core::World world(Core::World_setup{});

  Resource::load_default_resources(&world.m_world_data);
  
  Application::host_initialize(&world.m_world_data);
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  
  util::timer frame_timer;
  frame_timer.start();
  
  Graphics_api::clear_color_set(0.4f, 0.2f, 0.2f);

  Core::Transform cube_trans;
  cube_trans.set_position(math::vec3_init(0, 0, -1.f));
  cube_trans.set_scale(math::vec3_init(0.1f, 0.1f, 0.1f));

  Core::Entity cube = world.create_entity();
  cube.set_transform(cube_trans);
  cube.set_model_id(0);
  cube.set_material_id(1);
  
  Core::Entity cam_entity = world.create_entity();
  cam_entity.set_material_id(1);
  
  Core::Transform transform;
  transform.set_position(math::vec3_init(0.f, 0.5f, 1.f));
  cam_entity.set_transform(transform);
  
  Core::Camera cam;
  cam.set_attached_entity(cam_entity);
  
  // Foop
  bool run_game = true;
  
  while(run_game)
  {
    const float delta_time = static_cast<float>(frame_timer.split()) / 1000.f;

//    // ** Common ** //
    
    Application::common_think(
      &window,
      &world.m_core_data);
    
  
//
//    // ** Update World ** //
//    
//    Application::host_think(
//      &world.m_world_data,
//      delta_time);
//    
//    // ** Graphics ** //
//
    renderer_nodes.resize(world.m_world_data.entity_pool->size);
    Application::graphics_think(
        &world.m_world_data,
        renderer_nodes.data(),
        renderer_nodes.size(),
        0
      );
  }
  
  return 0;
}
#endif