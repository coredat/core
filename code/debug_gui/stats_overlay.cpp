#include <debug_gui/stats_overlay.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_world_stats(const float dt,
                    const uint32_t draw_calls,
                    const uint32_t render_passes)
{
  bool open = false;
  
  ImGui::SetNextWindowPos(ImVec2(20,40));
  if(!ImGui::Begin("General Stats", &open, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
  {
      ImGui::End();
      return;
  }

  ImGui::Text("Stats");
  ImGui::Separator();
  ImGui::Text("Delta-time: %.4f", dt);
  ImGui::Text("Draw calls: %d", draw_calls);
  ImGui::Text("Render Passes: %d", render_passes);
  ImGui::End();
}


} // ns