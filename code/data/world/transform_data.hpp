
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef TRANSFORM_INCLUDED_08568916_2958_4B54_88B7_D9061A8B9C90
#define TRANSFORM_INCLUDED_08568916_2958_4B54_88B7_D9061A8B9C90


#include <stdint.h>
#include <stddef.h>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>


namespace Data {


/*!
  Transform Model
*/
struct Transform_data
{
  // key //
  uint32_t *keys = nullptr;

  // transform field data //
  math::transform *field_transform = nullptr;

  // aabb field data //
  math::aabb *field_aabb = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Transform Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Transform_data *data);


void
data_unlock(const Transform_data *data);


bool
transform_create(Transform_data *data, const size_t size_hint);


void
transform_destroy(Transform_data *data);


uint32_t
transform_push(Transform_data *data, const uint32_t key);


bool
transform_insert(Transform_data *data, const uint32_t key, const size_t insert_point);


bool
transform_remove(Transform_data *data, const uint32_t key);


bool
transform_exists(const Transform_data *data, const uint32_t key, size_t *out_index = nullptr);


void
transform_clear(Transform_data *data);


bool
transform_is_empty(const Transform_data *data);


size_t
transform_get_size(const Transform_data *data);


size_t
transform_get_capacity(const Transform_data *data);


bool
transform_resize_capacity(Transform_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const math::transform*
transform_get_const_transform_data(const Transform_data *data);


math::transform*
transform_get_transform_data(Transform_data *data);


const math::aabb*
transform_get_const_aabb_data(const Transform_data *data);


math::aabb*
transform_get_aabb_data(Transform_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
transform_get_transform(const Transform_data *data, const uint32_t key, math::transform *return_value);


bool
transform_set_transform(const Transform_data *data, const uint32_t key, const math::transform *set_value);




bool
transform_get_aabb(const Transform_data *data, const uint32_t key, math::aabb *return_value);


bool
transform_set_aabb(const Transform_data *data, const uint32_t key, const math::aabb *set_value);




} // Data ns


#endif // inc guard
