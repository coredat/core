#include <debug_gui/memory_view.hpp>
#include <3rdparty/imgui/imgui.h>
#include <math/general/general.hpp>
#include <data/global_data/memory_data.hpp>


namespace Debug_menu {


void
display_memory_useage(util::memory_pool *data)
{
  ImGui::Begin("Memory Useage");
  {
    ImGui::TextWrapped("Shows memory in use. If the item is green then it is available (this is not to scale).");
  
    ImGui::BeginChild("scrolling", ImVec2(0,45), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    const size_t num_buttons = util::memory_pool_get_number_of_chunks(data);
    for (size_t n = 0; n < num_buttons; n++)
    {
      util::memory_chunk chunk = util::memory_pool_get_chunk_by_index(data, n);
    
      if (n > 0) ImGui::SameLine();
      ImGui::PushID(n * 1000);
      ImColor color = chunk.in_use ? ImColor::HSV(0.f, 1.f, 1.f) : ImColor::HSV(0.3f, 0.8f, 0.8f);
    
      ImGui::PushStyleColor(ImGuiCol_Button, color);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
    
      constexpr float constant = 1048576.f / 200.f;
      const float width = static_cast<float>(chunk.bytes_in_chunk) / constant;
      const float final_width = math::clamp(width, 10.f, 100.f);
      
      ImGui::Button(chunk.name, ImVec2(final_width, 0.0f));
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip("%s - bytes %zu", chunk.name, chunk.bytes_in_chunk);
      }
      ImGui::PopStyleColor(3);
      ImGui::PopID();
    }
    ImGui::EndChild();
    
    
    ImGui::TextWrapped("Max use of the scratch memory.");
    {
      const float progress = static_cast<float>(Memory::_get_scratch_max_used()) / static_cast<float>(Memory::_get_scratch_bytes_total());

      char buf[32];
      sprintf(buf, "%zu/%zu", (size_t)(Memory::_get_scratch_max_used()), Memory::_get_scratch_bytes_total());
      
      ImGui::ProgressBar(progress, ImVec2(-1.f,0.f), buf);
    }
    
  }
  ImGui::End();
}


} // ns