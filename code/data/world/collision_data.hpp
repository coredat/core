
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef COLLISION_INCLUDED_09F6195D_5784_4FD3_9397_F17539B66F68
#define COLLISION_INCLUDED_09F6195D_5784_4FD3_9397_F17539B66F68


#include <stdint.h>
#include <stddef.h>
#include <transformations/physics/collision_point.hpp>


namespace Data {


/*!
  Collision Model
*/
struct Collision_data
{
  // key //
  uint32_t *keys = nullptr;

  // entity_pair field data //
  uint64_t *field_entity_pair = nullptr;

  // collision_point field data //
  Physics_transform::Collision_point *field_collision_point = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Collision Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Collision_data *data);


void
data_unlock(const Collision_data *data);


bool
collision_create(Collision_data *data, const size_t size_hint);


void
collision_destroy(Collision_data *data);


uint32_t
collision_push(Collision_data *data);


bool
collision_remove(Collision_data *data, const uint32_t key);


bool
collision_exists(const Collision_data *data, const uint32_t key, size_t *out_index = nullptr);


void
collision_clear(Collision_data *data);


bool
collision_is_empty(const Collision_data *data);


size_t
collision_get_size(const Collision_data *data);


size_t
collision_get_capacity(const Collision_data *data);


bool
collision_resize_capacity(Collision_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint64_t*
collision_get_const_entity_pair_data(const Collision_data *data);


uint64_t*
collision_get_entity_pair_data(Collision_data *data);


const Physics_transform::Collision_point*
collision_get_const_collision_point_data(const Collision_data *data);


Physics_transform::Collision_point*
collision_get_collision_point_data(Collision_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
collision_get_entity_pair(const Collision_data *data, const uint32_t key, uint64_t *return_value);


bool
collision_set_entity_pair(const Collision_data *data, const uint32_t key, const uint64_t *set_value);




bool
collision_get_collision_point(const Collision_data *data, const uint32_t key, Physics_transform::Collision_point *return_value);


bool
collision_set_collision_point(const Collision_data *data, const uint32_t key, const Physics_transform::Collision_point *set_value);




} // Data ns


#endif // inc guard
