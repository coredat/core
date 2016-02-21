#ifndef CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D
#define CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D


#include <stdint.h>
#include <core/interface/entity.hpp>


namespace Core {


enum class Camera_type
{
  perspective,
};


namespace Camera_clear {
enum ENUM
{
  color = 1 << 0,
  depth = 1 << 1,
};
};


class Camera final
{
public:

  explicit              Camera();
                        ~Camera();
  
  void                  set_attached_entity(const Entity &entity);
  Entity                get_attached_entity() const;
  
  void                  set_priority(const uint32_t priority);
  uint32_t              get_priority() const;
  
  void                  set_peer_priority(const uint32_t peer, const uint32_t priority);
  uint32_t              get_peer_priority(const uint32_t peer) const;
  
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
  
  void                  set_clear_color(const float r, const float g, const float b);
  // TODO: Core::Color should that be a thing? if so we could set get a color here.
  
private:

  Core::Entity_id       m_attached_entity = Core::Entity_id_util::invalid_id();

};


} // ns


#endif // inc guard