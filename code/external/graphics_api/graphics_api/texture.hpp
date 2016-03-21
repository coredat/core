#ifndef TEXTURE_INCLUDED_6B294778_F9E5_4AB0_8266_239C14EF972D
#define TEXTURE_INCLUDED_6B294778_F9E5_4AB0_8266_239C14EF972D


#include <stdint.h>


namespace Graphics_api {

enum class Dimention
{
  one,
  two,
  three,
}; // enum


int
texture_load_from_data(const Dimention dimention, const uint32_t width, const uint32_t height, const void* data);


int
texture_load_from_file(const Dimention dimention, const char* file)


} // ns


#endif // inc guard