
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef CAMERA_INCLUDED_7FC91A6E_61D6_4010_8CB0_DFA1665C8D82
#define CAMERA_INCLUDED_7FC91A6E_61D6_4010_8CB0_DFA1665C8D82


#include <stdint.h>
#include <stddef.h>
#include <systems/camera/camera_properties.hpp>
#include <utilities/generic_id.hpp>


namespace Data {


/*!
  Camera Model
*/
struct Camera_data
{
  // key //
  uint32_t *keys = nullptr;

  // priority field data //
  uint32_t *field_priority = nullptr;

  // properties field data //
  Camera_util::Camera_properties *field_properties = nullptr;

  // texture_id field data //
  util::generic_id *field_texture_id = nullptr;

  // post_process_id field data //
  util::generic_id *field_post_process_id = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Camera Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Camera_data *data);


void
data_unlock(const Camera_data *data);


bool
camera_create(Camera_data *data, const size_t size_hint);


void
camera_destroy(Camera_data *data);


uint32_t
camera_push(Camera_data *data, const uint32_t key);


bool
camera_insert(Camera_data *data, const uint32_t key, const size_t insert_point);


bool
camera_remove(Camera_data *data, const uint32_t key);


bool
camera_exists(const Camera_data *data, const uint32_t key, size_t *out_index = nullptr);


void
camera_clear(Camera_data *data);


bool
camera_is_empty(const Camera_data *data);


size_t
camera_get_size(const Camera_data *data);


size_t
camera_get_capacity(const Camera_data *data);


bool
camera_resize_capacity(Camera_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint32_t*
camera_get_const_priority_data(const Camera_data *data);


uint32_t*
camera_get_priority_data(Camera_data *data);


const Camera_util::Camera_properties*
camera_get_const_properties_data(const Camera_data *data);


Camera_util::Camera_properties*
camera_get_properties_data(Camera_data *data);


const util::generic_id*
camera_get_const_texture_id_data(const Camera_data *data);


util::generic_id*
camera_get_texture_id_data(Camera_data *data);


const util::generic_id*
camera_get_const_post_process_id_data(const Camera_data *data);


util::generic_id*
camera_get_post_process_id_data(Camera_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
camera_get_priority(const Camera_data *data, const uint32_t key, uint32_t *return_value);


bool
camera_set_priority(const Camera_data *data, const uint32_t key, const uint32_t *set_value);




bool
camera_get_properties(const Camera_data *data, const uint32_t key, Camera_util::Camera_properties *return_value);


bool
camera_set_properties(const Camera_data *data, const uint32_t key, const Camera_util::Camera_properties *set_value);




bool
camera_get_texture_id(const Camera_data *data, const uint32_t key, util::generic_id *return_value);


bool
camera_set_texture_id(const Camera_data *data, const uint32_t key, const util::generic_id *set_value);




bool
camera_get_post_process_id(const Camera_data *data, const uint32_t key, util::generic_id *return_value);


bool
camera_set_post_process_id(const Camera_data *data, const uint32_t key, const util::generic_id *set_value);




} // Data ns


#endif // inc guard
