#ifndef STB_IMPL_INCLUDED_179ED979_F3EE_4F44_B641_14D3C956559F
#define STB_IMPL_INCLUDED_179ED979_F3EE_4F44_B641_14D3C956559F


/*
  STB's impl's go here.
  Add as required.
*/


#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#define STBI_ONLY_TGA
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"


#endif // inc guard