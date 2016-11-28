
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef LIGHT_INCLUDED_FAE9AE77_FF8A_4010_B401_034B904EAB57
#define LIGHT_INCLUDED_FAE9AE77_FF8A_4010_B401_034B904EAB57


#include <stdint.h>
#include <stddef.h>
#include <transformations/lighting/light_data.hpp>
#include <math/geometry/geometry_types.hpp>


namespace Data {


/*!
  Light Model
*/
struct Light_data
{
  // key //
  uint32_t *keys = nullptr;

  // light field data //
  Lighting::Light *field_light = nullptr;

  // bounding_box field data //
  math::aabb *field_bounding_box = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Light Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Light_data *data);


void
data_unlock(const Light_data *data);


bool
light_create(Light_data *data, const size_t size_hint);


void
light_destroy(Light_data *data);


uint32_t
light_push(Light_data *data, const uint32_t key);


bool
light_insert(Light_data *data, const uint32_t key, const size_t insert_point);


bool
light_remove(Light_data *data, const uint32_t key);


bool
light_exists(const Light_data *data, const uint32_t key, size_t *out_index = nullptr);


void
light_clear(Light_data *data);


bool
light_is_empty(const Light_data *data);


size_t
light_get_size(const Light_data *data);


size_t
light_get_capacity(const Light_data *data);


bool
light_resize_capacity(Light_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const Lighting::Light*
light_get_const_light_data(const Light_data *data);


Lighting::Light*
light_get_light_data(Light_data *data);


const math::aabb*
light_get_const_bounding_box_data(const Light_data *data);


math::aabb*
light_get_bounding_box_data(Light_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
light_get_light(const Light_data *data, const uint32_t key, Lighting::Light *return_value);


bool
light_set_light(const Light_data *data, const uint32_t key, const Lighting::Light *set_value);




bool
light_get_bounding_box(const Light_data *data, const uint32_t key, math::aabb *return_value);


bool
light_set_bounding_box(const Light_data *data, const uint32_t key, const math::aabb *set_value);




} // Data ns


#endif // inc guard
