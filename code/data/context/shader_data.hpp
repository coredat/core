
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef SHADER_INCLUDED_0479EC48_1F2D_497F_B456_D934A82B4C39
#define SHADER_INCLUDED_0479EC48_1F2D_497F_B456_D934A82B4C39


#include <stdint.h>
#include <stddef.h>
#include <graphics_api/ogl/ogl_shader.hpp>


namespace Data {


/*!
  Shader Model
*/
struct Shader_data
{
  // key //
  uint32_t *keys = nullptr;

  // name field data //
  char *field_name = nullptr;

  // shader field data //
  Ogl::Shader *field_shader = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Shader Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Shader_data *data);


void
data_unlock(const Shader_data *data);


bool
shader_create(Shader_data *data, const size_t size_hint);


void
shader_destroy(Shader_data *data);


uint32_t
shader_push(Shader_data *data);


bool
shader_remove(Shader_data *data, const uint32_t key);


bool
shader_exists(const Shader_data *data, const uint32_t key, size_t *out_index = nullptr);


void
shader_clear(Shader_data *data);


bool
shader_is_empty(const Shader_data *data);


size_t
shader_get_size(const Shader_data *data);


size_t
shader_get_capacity(const Shader_data *data);


bool
shader_resize_capacity(Shader_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
shader_get_const_name_data(const Shader_data *data);


char*
shader_get_name_data(Shader_data *data);


const Ogl::Shader*
shader_get_const_shader_data(const Shader_data *data);


Ogl::Shader*
shader_get_shader_data(Shader_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
shader_get_name(const Shader_data *data, const uint32_t key, const char **return_value);


bool
shader_set_name(const Shader_data *data, const uint32_t key, const char *set_value, const size_t size);


constexpr size_t
shader_get_name_stride() { return 32; }


bool
shader_get_shader(const Shader_data *data, const uint32_t key, Ogl::Shader *return_value);


bool
shader_set_shader(const Shader_data *data, const uint32_t key, const Ogl::Shader *set_value);




} // Data ns


#endif // inc guard
