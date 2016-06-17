#ifndef CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D
#define CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Camera final
{

                        Camera(const Camera&) = delete;
  Camera&               operator=(const Camera&) = delete;

public:

  explicit              Camera();
  explicit              Camera(Core::World &world);
                        ~Camera();
  
                        Camera(Camera &&);
  Camera&               operator=(Camera&&);

  void                  set_tags_to_render(const uint32_t);
  uint32_t              get_tags_to_render() const;
  
  void                  set_post_process(const Post_process &post);
  Post_process          get_post_process() const;
  
  void                  set_attached_entity(Entity_ref entity);
  Entity_ref            get_attached_entity() const;
  
  void                  set_render_target(const Render_target target);
  Render_target         get_render_target() const;
  
  void                  set_priority(const uint32_t priority);
  uint32_t              get_priority() const;
  
  void                  set_type(const Camera_type cam_type);
  Camera_type           get_type() const;
  
  void                  set_clear_flags(const uint32_t flags);
  uint32_t              get_clear_flags() const;
  
  void                  set_width(const uint32_t width);
  uint32_t              get_width() const;
  
  void                  set_height(const uint32_t height);
  uint32_t              get_height() const;
  
  void                  set_feild_of_view(const float fov_radians);
  float                 get_field_of_view() const;
  
  void                  set_near_plane(const float near_plane);
  float                 get_near_plane() const;
  
  void                  set_far_plane(const float far_plane);
  float                 get_far_plane() const;
  
  void                  set_clear_color(const Core::Color color);
  Core::Color           get_clear_color() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard