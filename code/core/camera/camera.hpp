#ifndef CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D
#define CAMERA_INCLUDED_054F2215_600C_4CD3_9A40_DEC0DE5ACE0D


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


enum class Camera_type : uint32_t
{
  perspective,
  orthographic,
};


namespace Camera_clear {
enum ENUM : uint32_t
{
  color = 1 << 0,
  depth = 1 << 1,
};
} // ns


class Camera final
{
public:

  explicit              Camera();
  explicit              Camera(const uint32_t width,
                               const uint32_t height,
                               const float near_plane = 0.1f,
                               const float far_plane = 100.f,
                               const float fov = math::quart_tau() * 0.5f);
  
                        ~Camera();
  
  void                  set_tags_to_render(const uint32_t);               //!< If the bitfield is set, the camera will only render entities with those tags.
  uint32_t              get_tags_to_render() const;                       //!< Returns the bit field of rendering tags.
  
  void                  set_post_process(const Post_process &post);
  Post_process          get_post_process() const;
  
  void                  set_render_target(const Render_target &target);   //!< Sets the render target for the ouput.
  Render_target         get_render_target() const;                        //!< Returns the render target for the ouput.
  
  void                  set_priority(const uint32_t priority);            //!< The cameras render in order of priority.
  uint32_t              get_priority() const;                             //!< Returns the priority of this camera.
  
  void                  set_type(const Camera_type cam_type);             //!< Orthographic or Perspective.
  Camera_type           get_type() const;                                 //!< Returns the type of camera.
  
  void                  set_clear_flags(const uint32_t flags);            //!< When a rendering pass happens, set if the depth or color are cleared.
  uint32_t              get_clear_flags() const;                          //!< Returns the clear flag settings.
  
  void                  set_width(const uint32_t width);                  //!< Set the viewport width.
  uint32_t              get_width() const;                                //!< Get the viewport width.
  
  void                  set_height(const uint32_t height);                //!< Set the viewport height.
  uint32_t              get_height() const;                               //!< Get the viewport height.
  
  void                  set_feild_of_view(const float fov_radians);       //!< Set the FOV in radians, Best results keep under TAU/4 (90 degrees)
  float                 get_field_of_view() const;                        //!< Return the current FOV.
  
  void                  set_near_plane(const float near_plane);           //!< Sets the near clipping plane, must be greater than 0, defaults to 0.1
  float                 get_near_plane() const;                           //!< Returns the near clipping plane.
  
  void                  set_far_plane(const float far_plane);             //!< Sets the far clipping plane.
  float                 get_far_plane() const;                            //!< Returns the far clipping plane.
  
  void                  set_clear_color(const Core::Color color);         //!< If the clear color flag is set, it will be cleared with this color.
  Core::Color           get_clear_color() const;                          //!< Return the current clear color.
  
private:

  Camera_type         m_camera_type       = Camera_type::perspective;
  uint32_t            m_viewport_width    = 800;
  uint32_t            m_viewport_height   = 600;
  float               m_fov               = 0.7855f; // 45 degs
  float               m_near_plane        = 0.1f;
  float               m_far_plane         = 1000.f;
  uint32_t            m_clear_flags       = Camera_clear::color | Camera_clear::depth;
  uint32_t            m_clear_color       = 0;
  uint32_t            m_cull_mask         = -1;
  uint32_t            m_priority          = 1;

};


} // ns


#endif // inc guard