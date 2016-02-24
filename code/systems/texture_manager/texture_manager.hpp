#ifndef TEXTURE_DATA_1A635A9D_89DC_4A7A_8F5A_9770961D64C8
#define TEXTURE_DATA_1A635A9D_89DC_4A7A_8F5A_9770961D64C8


#include <cstddef>
#include <stdint.h>
#include <renderer/graphics_api/ogl/ogl_fwd.hpp>
#include <string>


namespace Texture {


struct Load_texture
{
  std::uint32_t id;           //!< Index to be used for the output.
  std::string file_path;    //!< Path to load.
};


/*!
  Loads a sequence of renderer::texture.
  \param textures_to_load[] a list of Load_textures that have an id which is an index into the output buffer.
  \param number_of_textures_to_load the number of textures in the Load_texture array.
  \param output[] where to store the loaded textures.
  \param size_of_output_pool size of the output pool.
*/
void
texture_load(const Load_texture textures_to_load[],
             const std::uint32_t number_of_textures_to_load,
             Ogl::Texture output[],
             const std::uint32_t size_of_output_pool);
  
} // ns


#endif // inc guard