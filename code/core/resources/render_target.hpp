#ifndef RENDER_TARGET_INCLUDED_25C23B59_492F_42FD_BAA0_FC3B0157DE0C
#define RENDER_TARGET_INCLUDED_25C23B59_492F_42FD_BAA0_FC3B0157DE0C


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <graphics_api/pixel_format.hpp>


namespace Core {


class Render_target
{
public:


  explicit        Render_target();
  explicit        Render_target(const uint32_t width,
                                const uint32_t height,
                                const Graphics_api::Pixel_format format);
  
                  ~Render_target();
  
                  Render_target(const Render_target &render_target);
                  Render_target(Render_target &&render_target);
  
  Render_target&  operator=(const Render_target &render_target);
  Render_target&  operator=(Render_target &&render_target);
  
  
  Texture         get_texture() const;
  
  bool            is_valid() const;
                  operator bool() const; // alias for is_valid();
  
  
  uint32_t        get_id() const;
  
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard