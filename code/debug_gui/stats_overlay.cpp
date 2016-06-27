#include <debug_gui/stats_overlay.hpp>
#include <3rdparty/imgui/imgui.h>
#include <limits.h>


namespace
{
  constexpr uint32_t frame_count = 60;
  uint32_t rolling_frame_stats[frame_count];
  uint32_t current_frame = 0;
  
  inline void
  add_fps_stat(uint32_t fps)
  {
    rolling_frame_stats[current_frame] = fps;
    ++current_frame;
    
    current_frame = current_frame % frame_count;
  }
  
  inline uint32_t
  get_avg_fps()
  {
    uint32_t total = 0;
    
    for(auto &f : rolling_frame_stats)
    {
      total += f;
    }
    
    return total / frame_count;
  }
  
  inline uint32_t
  get_max_fps()
  {
    uint32_t max_frame_time = 0;
    
    for(auto &f : rolling_frame_stats)
    {
      if(f > max_frame_time)
      {
        max_frame_time = f;
      }
    }
    
    return max_frame_time;
  }
  
  inline uint32_t
  get_min_fps()
  {
    uint32_t min_frame_time = UINT_MAX;
    
    for(auto &f : rolling_frame_stats)
    {
      if(f < min_frame_time)
      {
        min_frame_time = f;
      }
    }
    
    return min_frame_time;
  }
}


namespace Debug_menu {


void
display_world_stats(const float dt,
                    const float dt_mul,
                    const uint32_t draw_calls,
                    const uint32_t render_passes)
{
  // Record the stat
  {
    const uint32_t fps_time = 1 / dt;
    add_fps_stat(fps_time);
  }

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

  ImGui::Text("Stats");
  ImGui::Separator();
  ImGui::Text("Delta-time: %.4f", dt);
  ImGui::Text("Framerate: %d", get_avg_fps());
  ImGui::Text("Min - Max FPS: (%d, %d)", get_min_fps(), get_max_fps());
  ImGui::Text("Delta-time multiplier: %.4f", dt_mul);
  ImGui::Text("Draw calls: %d", draw_calls);
  ImGui::Text("Render Passes: %d", render_passes);
  ImGui::End();
}


} // ns