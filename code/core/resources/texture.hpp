#ifndef TEXTURE_INCLUDED_4BAC239C_846B_4C57_9B2D_F49FB4E96062
#define TEXTURE_INCLUDED_4BAC239C_846B_4C57_9B2D_F49FB4E96062


#include <core/common/core_types.hpp>


namespace Core {


class Texture final
{
public:

  explicit            Texture();                              //!< Creates an invalid texture
  explicit            Texture(const util::generic_id id);     //!< Creates a texture from an existing id. Internal use only, this will be removed from public interface.
  explicit            Texture(const char * filepath);         //!< Loads a texture from a file name. Check exists() for the result.
  explicit            Texture(const uint32_t width,
                              const uint32_t height,
                              const void *data = nullptr);
  
                      ~Texture();
  
                      Texture(const Texture&);
                      Texture(Texture&&);
  
  Texture&            operator=(const Texture&);
  Texture&            operator=(Texture&&);
  
  
  void                update_sub_texture(const uint32_t x_offset,
                                         const uint32_t y_offset,
                                         const uint32_t width,
                                         const uint32_t height,
                                         const void *data);
  
  
  uint32_t            get_width() const;                      //!< Get the width of the loaded texture.
  uint32_t            get_height() const;                     //!< Get the height of the loaded texture.
  
  bool                exists() const;                         //!< Returns true if the texture loaded successfully.
                      operator bool() const;                  //!< Alias for exists().
  
  util::generic_id    get_id() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


} // ns


#endif // inc guard