
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "pending_entity_removal.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t pending_entity_removal_model_key_instance_number{0};

} // anon ns


void
data_lock(const Pending_entity_removal_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Pending_entity_removal_data *data)
{
  // Not yet impl.
}


bool
pending_entity_removal_create(Pending_entity_removal_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint);

  // Size up the capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = size_hint;
  }

  // Allocate memory
  bool all_alloc = true;
  {
    // Alloc keys
    if(all_alloc)
    {
      data->keys = new uint32_t[size_hint];
      assert(data->keys);
      if(!data->keys) { all_alloc = false; }
      else { memset(data->keys, 0, sizeof(uint32_t) * size_hint); }
    }

    // Alloc space for deleted_entity
    if(all_alloc)
    {
      data->field_deleted_entity = new uint32_t[size_hint * 1];
      assert(data->field_deleted_entity);
      if(!data->field_deleted_entity) { all_alloc = false; }
      else { memset(data->field_deleted_entity, 0, sizeof(uint32_t) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    pending_entity_removal_destroy(data);
  }

  return all_alloc;
}


void
pending_entity_removal_destroy(Pending_entity_removal_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove deleted_entity
    if(data->field_deleted_entity) { delete[] data->field_deleted_entity; }
    data->field_deleted_entity = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
pending_entity_removal_push(Pending_entity_removal_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    pending_entity_removal_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++pending_entity_removal_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
pending_entity_removal_remove(Pending_entity_removal_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(pending_entity_removal_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_deleted_entity[index_to_erase * 1], &data->field_deleted_entity[start_index * 1], size_to_end * sizeof(*data->field_deleted_entity) * 1);

    return true;
  }

  return false;
}


bool
pending_entity_removal_exists(const Pending_entity_removal_data *data, const uint32_t key, size_t *out_index)
{
  assert(data);
  assert(data != 0);

  for(size_t i = 0; i < data->size; ++i)
  {
    if(data->keys[i] == key)
    {
      if(out_index)
      {
        *out_index = i;
      }

      return true;
    }
  }

  return false;
}


void
pending_entity_removal_clear(Pending_entity_removal_data *data)
{
  assert(data);

  data->size = 0;
}


bool
pending_entity_removal_is_empty(const Pending_entity_removal_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
pending_entity_removal_get_size(const Pending_entity_removal_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
pending_entity_removal_get_capacity(const Pending_entity_removal_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
pending_entity_removal_resize_capacity(Pending_entity_removal_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Pending_entity_removal_data new_data;
  const bool created_new = pending_entity_removal_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    pending_entity_removal_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_deleted_entity, data->field_deleted_entity, sizeof(uint32_t) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    uint32_t *old_deleted_entity = data->field_deleted_entity;
    data->field_deleted_entity = new_data.field_deleted_entity;
    new_data.field_deleted_entity = old_deleted_entity;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  pending_entity_removal_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint32_t*
pending_entity_removal_get_const_deleted_entity_data(const Pending_entity_removal_data *data)
{
  assert(data);
  assert(data->field_deleted_entity);

  return data->field_deleted_entity;
}


uint32_t*
pending_entity_removal_get_deleted_entity_data(Pending_entity_removal_data *data)
{
  assert(data);
  assert(data->field_deleted_entity);

  return data->field_deleted_entity;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
pending_entity_removal_get_deleted_entity(const Pending_entity_removal_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_deleted_entity);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(pending_entity_removal_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_deleted_entity[index];

      return true;
    }
  }

  return false;
}


bool
pending_entity_removal_set_deleted_entity(const Pending_entity_removal_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_deleted_entity);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(pending_entity_removal_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_deleted_entity[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
