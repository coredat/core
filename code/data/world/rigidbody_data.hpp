
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef RIGIDBODY_INCLUDED_DB466439_6B26_4D30_8918_B72A0328ADEC
#define RIGIDBODY_INCLUDED_DB466439_6B26_4D30_8918_B72A0328ADEC


#include <stdint.h>
#include <stddef.h>


namespace Data {


/*!
  Rigidbody Model
*/
struct Rigidbody_data
{
  // key //
  uint32_t *keys = nullptr;

  // collision_id field data //
  uint64_t *field_collision_id = nullptr;

  // rigidbody field data //
  uintptr_t *field_rigidbody = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Rigidbody Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Rigidbody_data *data);


void
data_unlock(const Rigidbody_data *data);


bool
rigidbody_create(Rigidbody_data *data, const size_t size_hint);


void
rigidbody_destroy(Rigidbody_data *data);


uint32_t
rigidbody_push(Rigidbody_data *data, const uint32_t key);


bool
rigidbody_insert(Rigidbody_data *data, const uint32_t key, const size_t insert_point);


bool
rigidbody_remove(Rigidbody_data *data, const uint32_t key);


bool
rigidbody_exists(const Rigidbody_data *data, const uint32_t key, size_t *out_index = nullptr);


void
rigidbody_clear(Rigidbody_data *data);


bool
rigidbody_is_empty(const Rigidbody_data *data);


size_t
rigidbody_get_size(const Rigidbody_data *data);


size_t
rigidbody_get_capacity(const Rigidbody_data *data);


bool
rigidbody_resize_capacity(Rigidbody_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint64_t*
rigidbody_get_const_collision_id_data(const Rigidbody_data *data);


uint64_t*
rigidbody_get_collision_id_data(Rigidbody_data *data);


const uintptr_t*
rigidbody_get_const_rigidbody_data(const Rigidbody_data *data);


uintptr_t*
rigidbody_get_rigidbody_data(Rigidbody_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
rigidbody_get_collision_id(const Rigidbody_data *data, const uint32_t key, uint64_t *return_value);


bool
rigidbody_set_collision_id(const Rigidbody_data *data, const uint32_t key, const uint64_t *set_value);




bool
rigidbody_get_rigidbody(const Rigidbody_data *data, const uint32_t key, uintptr_t *return_value);


bool
rigidbody_set_rigidbody(const Rigidbody_data *data, const uint32_t key, const uintptr_t *set_value);




} // Data ns


#endif // inc guard
