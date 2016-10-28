
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef POST_PROCESS_INCLUDED_47E959ED_C50B_4960_A9C3_ADFE4F460796
#define POST_PROCESS_INCLUDED_47E959ED_C50B_4960_A9C3_ADFE4F460796


#include <stdint.h>
#include <stddef.h>
#include <systems/renderer_post/post_shader.hpp>


namespace Data {


/*!
  Post_process Model
*/
struct Post_process_data
{
  // key //
  uint32_t *keys = nullptr;

  // name field data //
  char *field_name = nullptr;

  // post_shader field data //
  Post_renderer::Post_shader *field_post_shader = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Post_process Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Post_process_data *data);


void
data_unlock(const Post_process_data *data);


bool
post_process_create(Post_process_data *data, const size_t size_hint);


void
post_process_destroy(Post_process_data *data);


uint32_t
post_process_push(Post_process_data *data);


bool
post_process_remove(Post_process_data *data, const uint32_t key);


bool
post_process_exists(const Post_process_data *data, const uint32_t key, size_t *out_index = nullptr);


void
post_process_clear(Post_process_data *data);


bool
post_process_is_empty(const Post_process_data *data);


size_t
post_process_get_size(const Post_process_data *data);


size_t
post_process_get_capacity(const Post_process_data *data);


bool
post_process_resize_capacity(Post_process_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
post_process_get_const_name_data(const Post_process_data *data);


char*
post_process_get_name_data(Post_process_data *data);


const Post_renderer::Post_shader*
post_process_get_const_post_shader_data(const Post_process_data *data);


Post_renderer::Post_shader*
post_process_get_post_shader_data(Post_process_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
post_process_get_name(const Post_process_data *data, const uint32_t key, const char **return_value);


bool
post_process_set_name(const Post_process_data *data, const uint32_t key, const char *set_value, const size_t size);


constexpr size_t
post_process_get_name_stride() { return 32; }


bool
post_process_get_post_shader(const Post_process_data *data, const uint32_t key, Post_renderer::Post_shader *return_value);


bool
post_process_set_post_shader(const Post_process_data *data, const uint32_t key, const Post_renderer::Post_shader *set_value);




} // Data ns


#endif // inc guard
