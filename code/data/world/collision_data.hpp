
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef COLLISION_INCLUDED_9E45E9A1_D0C1_43C8_9ACE_A3BC2F9FC7CF
#define COLLISION_INCLUDED_9E45E9A1_D0C1_43C8_9ACE_A3BC2F9FC7CF


#include <stdint.h>
#include <stddef.h>
#include <math/vec/vec_types.hpp>


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

  // penitration field data //
  float *field_penitration = nullptr;

  // position field data //
  math::vec3 *field_position = nullptr;

  // normal field data //
  math::vec3 *field_normal = nullptr;

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


const float*
collision_get_const_penitration_data(const Collision_data *data);


float*
collision_get_penitration_data(Collision_data *data);


const math::vec3*
collision_get_const_position_data(const Collision_data *data);


math::vec3*
collision_get_position_data(Collision_data *data);


const math::vec3*
collision_get_const_normal_data(const Collision_data *data);


math::vec3*
collision_get_normal_data(Collision_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
collision_get_entity_pair(const Collision_data *data, const uint32_t key, uint64_t *return_value);


bool
collision_set_entity_pair(const Collision_data *data, const uint32_t key, const uint64_t *set_value);




bool
collision_get_penitration(const Collision_data *data, const uint32_t key, float *return_value);


bool
collision_set_penitration(const Collision_data *data, const uint32_t key, const float *set_value);




bool
collision_get_position(const Collision_data *data, const uint32_t key, math::vec3 *return_value);


bool
collision_set_position(const Collision_data *data, const uint32_t key, const math::vec3 *set_value);




bool
collision_get_normal(const Collision_data *data, const uint32_t key, math::vec3 *return_value);


bool
collision_set_normal(const Collision_data *data, const uint32_t key, const math::vec3 *set_value);




} // Data ns


#endif // inc guard
