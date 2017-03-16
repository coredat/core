

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Sun 09 Oct 2016
*/


#include <3rdparty/imgui/imgui.h>
#include <data/graph/graph.hpp>
#include "debug_ui_transform_data.hpp"
#include <math/transform/transform_types.hpp>
#include <math/geometry/geometry.hpp>


namespace Debug_menu {


void
display_transform_data(const Data::Graph::Graph_data *data)
{
  ImGui::Begin("Transform data");
  {
    // Container Summary
    ImGui::TextWrapped("Transform data container");
    ImGui::Separator();

    // List contents
    const size_t data_size = Data::Graph::node_count(data);
    
    const uint32_t *ids = Data::Graph::get_node_ids(data);
    const math::transform *transforms = Data::Graph::get_world_transforms(data);
    const math::aabb *aabbs = Data::Graph::get_aabbs(data);
    
    
    for(size_t i = 0; i < data_size; ++i)
    {
      // Key
      ImGui::Text("Key: %d", ids[i]);

      // Get property_transform
      ImGui::Text("Transform");
      const math::transform trans = transforms[i];
      
      ImGui::Text(
        "position: %.3f, %.3f, %.3f",
        math::get_x(trans.position),
        math::get_y(trans.position),
        math::get_z(trans.position)
      );
      
      ImGui::Text(
        "rotation: %.3f, %.3f, %.3f, %.3f",
        math::get_x(trans.rotation),
        math::get_y(trans.rotation),
        math::get_z(trans.rotation),
        math::get_w(trans.rotation)
      );
      
      ImGui::Text(
        "scale: %.3f, %.3f, %.3f",
        math::get_x(trans.scale),
        math::get_y(trans.scale),
        math::get_z(trans.scale)
      );

      // Get property_aabb
      math::aabb property_aabb_value = aabbs[i];

      ImGui::Text("Aabb");
      ImGui::Text(
        "Min: %.3f, %.3f, %.3f",
        math::get_x(property_aabb_value.min),
        math::get_y(property_aabb_value.min),
        math::get_z(property_aabb_value.min)
      );
      
      ImGui::Text(
        "Max: %.3f, %.3f, %.3f",
        math::get_x(property_aabb_value.max),
        math::get_y(property_aabb_value.max),
        math::get_z(property_aabb_value.max)
      );

      const math::vec3 origin = math::aabb_get_origin(property_aabb_value);
      
      ImGui::Text(
        "Origin: %.3f, %.3f, %.3f",
        math::get_x(origin),
        math::get_y(origin),
        math::get_z(origin)
      );

      // End of data
      ImGui::Separator();
    }

  }
  ImGui::End();
}


} // ns
