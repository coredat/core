#include <debug_gui/stats_overlay.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_world_stats(const float dt,
                    const float dt_mul,
                    const uint32_t draw_calls,
                    const uint32_t render_passes)
{
  bool open = false;
  
  const uint32_t flags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings;
  
  ImGui::SetNextWindowPos(ImVec2(20,40));
  if(!ImGui::Begin("General Stats",
                   &open,
                   ImVec2(0,0),
                   0.3f,
                   flags))
  {
      ImGui::End();
      return;
  }
  
  static float min_dt = dt;
  static float max_dt = dt;
  float curr_dt = dt;
  
  if(min_dt > curr_dt)
  {
    min_dt = curr_dt;
  }
  
  if(max_dt < curr_dt)
  {
    max_dt = curr_dt;
  }

  ImGui::Text("Stats");
  ImGui::Separator();
  ImGui::Text("Delta-time: %.4f", dt);
  ImGui::Text("Min - Max dt: (%.2f, %.2f)", min_dt, max_dt);
  ImGui::Text("Framerate: %d", static_cast<uint32_t>(1.f / dt));
  ImGui::Text("Min - Max FPS: (%d, %d)", static_cast<uint32_t>(1.f / min_dt), static_cast<uint32_t>(1.f / max_dt));
  ImGui::Text("Delta-time multiplier: %.4f", dt_mul);
  ImGui::Text("Draw calls: %d", draw_calls);
  ImGui::Text("Render Passes: %d", render_passes);
  ImGui::End();
}


} // ns