#ifndef BUFFER_INCLUDED_B2578CA6_FE0E_4F8A_9CD9_5320BAD4C147
#define BUFFER_INCLUDED_B2578CA6_FE0E_4F8A_9CD9_5320BAD4C147


#include "gfx_fwd.hpp"


namespace Graphics_api {


class Buffer
{
public:

  explicit          Buffer(const Context *ctx);
  
  void              set_shader(const uint32_t shader_id);
  void              set_texture(const uint32_t texture_id);
  void              set_mesh(const uint32_t mesh_id);
  void              set_vertex_format(const uint32_t vertex_id);
  void              set_target(const uint32_t target_id);
  void              set_rasterizer(const uint32_t rasterizer_id);
  
  void              render();
  
  void              exec();
};


} // ns


#endif // inc guard