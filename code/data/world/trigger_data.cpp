
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "trigger_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //



void
data_lock(const Trigger_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Trigger_data *data)
{
  // Not yet impl.
}


bool
trigger_create(Trigger_data *data, const size_t size_hint)
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

    // Alloc space for collision_id
    if(all_alloc)
    {
      data->field_collision_id = new uint64_t[size_hint * 1];
      assert(data->field_collision_id);
      if(!data->field_collision_id) { all_alloc = false; }
      else { memset(data->field_collision_id, 0, sizeof(uint64_t) * size_hint * 1); }
    }

    // Alloc space for trigger
    if(all_alloc)
    {
      data->field_trigger = new uintptr_t[size_hint * 1];
      assert(data->field_trigger);
      if(!data->field_trigger) { all_alloc = false; }
      else { memset(data->field_trigger, 0, sizeof(uintptr_t) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    trigger_destroy(data);
  }

  return all_alloc;
}


void
trigger_destroy(Trigger_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove collision_id
    if(data->field_collision_id) { delete[] data->field_collision_id; }
    data->field_collision_id = nullptr;

    // Remove trigger
    if(data->field_trigger) { delete[] data->field_trigger; }
    data->field_trigger = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
trigger_push(Trigger_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    trigger_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = key;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}

bool
trigger_insert(Trigger_data *data, const uint32_t key, const size_t insert_point)
{
  assert(data);
  assert(data->keys);
  assert(key);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    trigger_resize_capacity(data, data->capacity << 1);
  }

  // Shuffle memory up
  {
    const size_t insert_index = insert_point + 1;
    const size_t start_index = insert_point;
    const size_t size_to_end = data->size - insert_point;

    // Shuffle the data down
    memmove(&data->keys[insert_point], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_collision_id[insert_point * 1], &data->field_collision_id[start_index * 1], size_to_end * sizeof(*data->field_collision_id) * 1);
    memmove(&data->field_trigger[insert_point * 1], &data->field_trigger[start_index * 1], size_to_end * sizeof(*data->field_trigger) * 1);
  }

  // Insert new data
  {
    data->keys[data->size++] = key;
  }

  return false;
}


bool
trigger_remove(Trigger_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(trigger_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_collision_id[index_to_erase * 1], &data->field_collision_id[start_index * 1], size_to_end * sizeof(*data->field_collision_id) * 1);
    memmove(&data->field_trigger[index_to_erase * 1], &data->field_trigger[start_index * 1], size_to_end * sizeof(*data->field_trigger) * 1);

    return true;
  }

  return false;
}


bool
trigger_exists(const Trigger_data *data, const uint32_t key, size_t *out_index)
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
trigger_clear(Trigger_data *data)
{
  assert(data);

  data->size = 0;
}


bool
trigger_is_empty(const Trigger_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
trigger_get_size(const Trigger_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
trigger_get_capacity(const Trigger_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
trigger_resize_capacity(Trigger_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Trigger_data new_data;
  const bool created_new = trigger_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    trigger_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_collision_id, data->field_collision_id, sizeof(uint64_t) * data->size * 1);
    memcpy(new_data.field_trigger, data->field_trigger, sizeof(uintptr_t) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    uint64_t *old_collision_id = data->field_collision_id;
    data->field_collision_id = new_data.field_collision_id;
    new_data.field_collision_id = old_collision_id;

    uintptr_t *old_trigger = data->field_trigger;
    data->field_trigger = new_data.field_trigger;
    new_data.field_trigger = old_trigger;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  trigger_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint64_t*
trigger_get_const_collision_id_data(const Trigger_data *data)
{
  assert(data);
  assert(data->field_collision_id);

  return data->field_collision_id;
}


uint64_t*
trigger_get_collision_id_data(Trigger_data *data)
{
  assert(data);
  assert(data->field_collision_id);

  return data->field_collision_id;
}


const uintptr_t*
trigger_get_const_trigger_data(const Trigger_data *data)
{
  assert(data);
  assert(data->field_trigger);

  return data->field_trigger;
}


uintptr_t*
trigger_get_trigger_data(Trigger_data *data)
{
  assert(data);
  assert(data->field_trigger);

  return data->field_trigger;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
trigger_get_collision_id(const Trigger_data *data, const uint32_t key, uint64_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_collision_id);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(trigger_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_collision_id[index];

      return true;
    }
  }

  return false;
}


bool
trigger_set_collision_id(const Trigger_data *data, const uint32_t key, const uint64_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_collision_id);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(trigger_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_collision_id[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
trigger_get_trigger(const Trigger_data *data, const uint32_t key, uintptr_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_trigger);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(trigger_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_trigger[index];

      return true;
    }
  }

  return false;
}


bool
trigger_set_trigger(const Trigger_data *data, const uint32_t key, const uintptr_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_trigger);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(trigger_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_trigger[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
