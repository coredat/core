#ifndef CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D
#define CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D


#include <core/camera/camera_fwd.hpp>
#include <core/entity/entity_fwd.hpp>
#include <core/color/color_fwd.hpp>
#include <stdint.h>
#include <memory>


namespace Core {


class Camera final
{

                        Camera(const Camera&) = delete;
  Camera&               operator=(const Camera&) = delete;

public:

  explicit              Camera();
                        ~Camera();
  
                        Camera(Camera &&);
  Camera&               operator=(Camera&&);
  
  void                  set_attached_entity(const Entity_ref entity);
  Entity_ref            get_attached_entity() const;
  
  void                  set_priority(const uint32_t priority);
  uint32_t              get_priority() const;
  
  void                  set_peer_priority(const uint32_t peer, const uint32_t priority);
  uint32_t              get_peer_priority(const uint32_t peer) const;
  
  void                  set_type(const Camera_type cam_type);
  Camera_type           get_type() const;
  
  void                  set_clear_flags(const uint32_t flags);
  uint32_t              get_clear_flags() const;
  bool                  will_clear_color() const;
  bool                  will_clear_depth() const;
  
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