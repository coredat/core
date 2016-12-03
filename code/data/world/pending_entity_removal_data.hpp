
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef PENDING_ENTITY_REMOVAL_INCLUDED_A53129BD_5401_42AB_8785_9DD1D97BED24
#define PENDING_ENTITY_REMOVAL_INCLUDED_A53129BD_5401_42AB_8785_9DD1D97BED24


#include <stdint.h>
#include <stddef.h>


namespace Data {


/*!
  Pending_entity_removal Model
*/
struct Pending_entity_removal_data
{
  // key //
  uint32_t *keys = nullptr;

  // deleted_entity field data //
  uint32_t *field_deleted_entity = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Pending_entity_removal Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Pending_entity_removal_data *data);


void
data_unlock(const Pending_entity_removal_data *data);


bool
pending_entity_removal_create(Pending_entity_removal_data *data, const size_t size_hint);


void
pending_entity_removal_destroy(Pending_entity_removal_data *data);


uint32_t
pending_entity_removal_push(Pending_entity_removal_data *data);


bool
pending_entity_removal_remove(Pending_entity_removal_data *data, const uint32_t key);


bool
pending_entity_removal_exists(const Pending_entity_removal_data *data, const uint32_t key, size_t *out_index = nullptr);


void
pending_entity_removal_clear(Pending_entity_removal_data *data);


bool
pending_entity_removal_is_empty(const Pending_entity_removal_data *data);


size_t
pending_entity_removal_get_size(const Pending_entity_removal_data *data);


size_t
pending_entity_removal_get_capacity(const Pending_entity_removal_data *data);


bool
pending_entity_removal_resize_capacity(Pending_entity_removal_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint32_t*
pending_entity_removal_get_const_deleted_entity_data(const Pending_entity_removal_data *data);


uint32_t*
pending_entity_removal_get_deleted_entity_data(Pending_entity_removal_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
pending_entity_removal_get_deleted_entity(const Pending_entity_removal_data *data, const uint32_t key, uint32_t *return_value);


bool
pending_entity_removal_set_deleted_entity(const Pending_entity_removal_data *data, const uint32_t key, const uint32_t *set_value);




} // Data ns


#endif // inc guard
