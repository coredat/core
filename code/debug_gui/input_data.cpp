#include <debug_gui/input_data.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_input_data(const Context_data::Input_pool *data)
{
  ImGui::Begin("Input Data");
  
  
  ImGui::Columns(5, "controller_col");
  ImGui::Separator();
    
  ImGui::Text("Number");    ImGui::NextColumn();
  ImGui::Text("L Axis");      ImGui::NextColumn();
  ImGui::Text("R Axis");      ImGui::NextColumn();
  ImGui::Text("L Trigger");  ImGui::NextColumn();  
  ImGui::Text("R Trigger");  ImGui::NextColumn();  
  ImGui::Separator();

  for(uint32_t i = 0; i < data->controller_count; ++i)
  {
    ImGui::Text("%02i", i); ImGui::NextColumn();
    ImGui::Text("x:y %02f:%02f", data->controllers[i].axis[0].x, data->controllers[i].axis[0].y); ImGui::NextColumn();
    ImGui::Text("x:y %02f:%02f", data->controllers[i].axis[1].x, data->controllers[i].axis[1].y); ImGui::NextColumn();
    ImGui::Text("%02f", data->controllers[i].triggers[0]); ImGui::NextColumn();
    ImGui::Text("%02f", data->controllers[i].triggers[1]); ImGui::NextColumn();
  }
     
  ImGui::End();
}


} // ns