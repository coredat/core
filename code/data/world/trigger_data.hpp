
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef TRIGGER_INCLUDED_C62B8489_87B1_44B1_8E9D_5C575C236962
#define TRIGGER_INCLUDED_C62B8489_87B1_44B1_8E9D_5C575C236962


#include <stdint.h>
#include <stddef.h>


namespace Data {


/*!
  Trigger Model
*/
struct Trigger_data
{
  // key //
  uint32_t *keys = nullptr;

  // collision_id field data //
  uint64_t *field_collision_id = nullptr;

  // trigger field data //
  uintptr_t *field_trigger = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Trigger Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Trigger_data *data);


void
data_unlock(const Trigger_data *data);


bool
trigger_create(Trigger_data *data, const size_t size_hint);


void
trigger_destroy(Trigger_data *data);


uint32_t
trigger_push(Trigger_data *data, const uint32_t key);


bool
trigger_insert(Trigger_data *data, const uint32_t key, const size_t insert_point);


bool
trigger_remove(Trigger_data *data, const uint32_t key);


bool
trigger_exists(const Trigger_data *data, const uint32_t key, size_t *out_index = nullptr);


void
trigger_clear(Trigger_data *data);


bool
trigger_is_empty(const Trigger_data *data);


size_t
trigger_get_size(const Trigger_data *data);


size_t
trigger_get_capacity(const Trigger_data *data);


bool
trigger_resize_capacity(Trigger_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint64_t*
trigger_get_const_collision_id_data(const Trigger_data *data);


uint64_t*
trigger_get_collision_id_data(Trigger_data *data);


const uintptr_t*
trigger_get_const_trigger_data(const Trigger_data *data);


uintptr_t*
trigger_get_trigger_data(Trigger_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
trigger_get_collision_id(const Trigger_data *data, const uint32_t key, uint64_t *return_value);


bool
trigger_set_collision_id(const Trigger_data *data, const uint32_t key, const uint64_t *set_value);




bool
trigger_get_trigger(const Trigger_data *data, const uint32_t key, uintptr_t *return_value);


bool
trigger_set_trigger(const Trigger_data *data, const uint32_t key, const uintptr_t *set_value);




} // Data ns


#endif // inc guard
