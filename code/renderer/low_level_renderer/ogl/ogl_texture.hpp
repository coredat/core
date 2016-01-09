#ifndef OGL_TEXTURE_INCLUDED_530B56CF_2269_430E_ACC6_04D56756ACC4
#define OGL_TEXTURE_INCLUDED_530B56CF_2269_430E_ACC6_04D56756ACC4


#include "ogl_common.hpp"
#include "ogl_pixel_format.hpp"
#include <stdint.h>
#include <assert.h>


namespace Ogl {


/*!
  Texture Object.
*/
struct Texture
{
  GLuint          texture_id  = 0;
  uint32_t        width       = 0;
  uint32_t        height      = 0;
  GLenum          format      = GL_RGBA;
}; // struct


/*!
  Creates a texture from the given data.
  \param out_texture The texture to generate.
  \param width The width of the texture data.
  \param height The height of the texture data.
  \param Pixel_format The desired format of the pixels.
  \param data Pointer to the raw data in memory.
  \param log Optional logging stream.
*/
void
texture_create_2d(Texture *out_texture,
                  const uint32_t width,
                  const uint32_t height,
                  const GLenum format,
                  const void *data,
                  std::ostream *log = nullptr);



/*!
  Update the data in a texture.
  \param update_texture The target to update.
  \param offset_x Starting point of the update.
  \param offset_y Starting point of the update.
  \param width Width of the update.
  \param height Height of the update.
  \param data Pointer to the raw data in memory.
  \param log Optional logging stream.
*/
void
texture_update_texture_2d(Texture *update_texture,
                          const uint32_t offset_x,
                          const uint32_t offset_y,
                          const uint32_t width,
                          const uint32_t height,
                          const void *data,
                          std::ostream *log = nullptr);


} // ns


#endif // inc guard