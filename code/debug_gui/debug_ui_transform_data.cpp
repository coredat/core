

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Sun 09 Oct 2016
*/


#include <3rdparty/imgui/imgui.h>
#include <data/world/transform_data.hpp>
#include "debug_ui_transform_data.hpp"


namespace Debug_menu {


void
display_transform_data(const Data::Transform_data *data)
{
  ImGui::Begin("Transform data");
  {
    // Container Summary
    ImGui::TextWrapped("Transform data container");
    ImGui::TextWrapped("size/capacity: %zu/%zu", data->size, data->capacity);
    ImGui::Separator();

    // List contents
    for(size_t i = 0; i < data->size; ++i)
    {
      // Key
      ImGui::Text("Key: %d", data->keys[i]);


      // Get property_transform
      math::transform property_transform_value;
      Data::transform_get_transform(data, data->keys[i], &property_transform_value);
      ImGui::Text("Transform");
      ImGui::Text("position: %.3f, %.3f, %.3f", math::get_x(property_transform_value.position), math::get_y(property_transform_value.position), math::get_z(property_transform_value.position));
      ImGui::Text("rotation: %.3f, %.3f, %.3f, %.3f", math::get_x(property_transform_value.rotation), math::get_y(property_transform_value.rotation), math::get_z(property_transform_value.rotation), math::get_w(property_transform_value.rotation));
      ImGui::Text("scale: %.3f, %.3f, %.3f", math::get_x(property_transform_value.scale), math::get_y(property_transform_value.scale), math::get_z(property_transform_value.scale));


      // Get property_aabb
      math::aabb property_aabb_value;
      Data::transform_get_aabb(data, data->keys[i], &property_aabb_value);
      ImGui::Text("Aabb");
      ImGui::Text("Min: %.3f, %.3f, %.3f", math::get_x(property_aabb_value.min), math::get_y(property_aabb_value.min), math::get_z(property_aabb_value.min));
      ImGui::Text("Max: %.3f, %.3f, %.3f", math::get_x(property_aabb_value.max), math::get_y(property_aabb_value.max), math::get_z(property_aabb_value.max));

      const math::vec3 origin = math::aabb_get_origin(property_aabb_value);
      ImGui::Text("Origin: %.3f, %.3f, %.3f", math::get_x(origin), math::get_y(origin), math::get_z(origin));


      // End of data
      ImGui::Separator();
    }

  }
  ImGui::End();
}


} // ns
