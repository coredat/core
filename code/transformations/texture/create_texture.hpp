#ifndef CREATE_TEXTURE_INCLUDED_76C7B3D6_2F23_4426_8FE6_580AFB4777DF
#define CREATE_TEXTURE_INCLUDED_76C7B3D6_2F23_4426_8FE6_580AFB4777DF


#include <graphics_api/texture.hpp> // TODO: Fwd dec


namespace Texture_utils {


void
create_texture_from_file(const char *filename, Ogl::Texture *texture);


} // ns


#endif // inc guard