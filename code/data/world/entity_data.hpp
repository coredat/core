
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef ENTITY_INCLUDED_700F5E00_D879_4564_8358_D0E37462E5AC
#define ENTITY_INCLUDED_700F5E00_D879_4564_8358_D0E37462E5AC


#include <stdint.h>
#include <stddef.h>


namespace Data {


/*!
  Entity Model
*/
struct Entity_data
{
  // key //
  uint32_t *keys = nullptr;

  // name field data //
  char *field_name = nullptr;

  // tags field data //
  uint32_t *field_tags = nullptr;

  // components field data //
  uint32_t *field_components = nullptr;

  // renderer field data //
  uint32_t *field_renderer = nullptr;

  // user_data field data //
  uintptr_t *field_user_data = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Entity Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Entity_data *data);


void
data_unlock(const Entity_data *data);


bool
entity_create(Entity_data *data, const size_t size_hint);


void
entity_destroy(Entity_data *data);


uint32_t
entity_push(Entity_data *data, const uint32_t key);


bool
entity_insert(Entity_data *data, const uint32_t key, const size_t insert_point);


bool
entity_remove(Entity_data *data, const uint32_t key);


bool
entity_exists(const Entity_data *data, const uint32_t key, size_t *out_index = nullptr);


void
entity_clear(Entity_data *data);


bool
entity_is_empty(const Entity_data *data);


size_t
entity_get_size(const Entity_data *data);


size_t
entity_get_capacity(const Entity_data *data);


bool
entity_resize_capacity(Entity_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
entity_get_const_name_data(const Entity_data *data);


char*
entity_get_name_data(Entity_data *data);


const uint32_t*
entity_get_const_tags_data(const Entity_data *data);


uint32_t*
entity_get_tags_data(Entity_data *data);


const uint32_t*
entity_get_const_components_data(const Entity_data *data);


uint32_t*
entity_get_components_data(Entity_data *data);


const uint32_t*
entity_get_const_renderer_data(const Entity_data *data);


uint32_t*
entity_get_renderer_data(Entity_data *data);


const void**
entity_get_const_user_data_data(const Entity_data *data);


void**
entity_get_user_data_data(Entity_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
entity_get_name(const Entity_data *data, const uint32_t key, const char **return_value);


bool
entity_set_name(const Entity_data *data, const uint32_t key, const char *set_value, const size_t size);


bool
entity_get_tags(const Entity_data *data, const uint32_t key, uint32_t *return_value);


bool
entity_set_tags(const Entity_data *data, const uint32_t key, const uint32_t *set_value);


bool
entity_get_components(const Entity_data *data, const uint32_t key, uint32_t *return_value);


bool
entity_set_components(const Entity_data *data, const uint32_t key, const uint32_t *set_value);


bool
entity_get_renderer(const Entity_data *data, const uint32_t key, uint32_t *return_value);


bool
entity_set_renderer(const Entity_data *data, const uint32_t key, const uint32_t *set_value);


bool
entity_get_user_data(const Entity_data *data, const uint32_t key, uintptr_t *return_value);


bool
entity_set_user_data(const Entity_data *data, const uint32_t key, const uintptr_t *set_value);


} // Data ns


#endif // inc guard
