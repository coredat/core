
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "entity_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //



void
data_lock(const Entity_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Entity_data *data)
{
  // Not yet impl.
}


bool
entity_create(Entity_data *data, const size_t size_hint)
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

    // Alloc space for name
    if(all_alloc)
    {
      data->field_name = new char[size_hint * 32];
      assert(data->field_name);
      if(!data->field_name) { all_alloc = false; }
      else { memset(data->field_name, 0, sizeof(char) * size_hint * 32); }
    }

    // Alloc space for tags
    if(all_alloc)
    {
      data->field_tags = new uint32_t[size_hint * 1];
      assert(data->field_tags);
      if(!data->field_tags) { all_alloc = false; }
      else { memset(data->field_tags, 0, sizeof(uint32_t) * size_hint * 1); }
    }

    // Alloc space for components
    if(all_alloc)
    {
      data->field_components = new uint32_t[size_hint * 1];
      assert(data->field_components);
      if(!data->field_components) { all_alloc = false; }
      else { memset(data->field_components, 0, sizeof(uint32_t) * size_hint * 1); }
    }

    // Alloc space for renderer
    if(all_alloc)
    {
      data->field_renderer = new uint32_t[size_hint * 1];
      assert(data->field_renderer);
      if(!data->field_renderer) { all_alloc = false; }
      else { memset(data->field_renderer, 0, sizeof(uint32_t) * size_hint * 1); }
    }

    // Alloc space for user_data
    if(all_alloc)
    {
      data->field_user_data = new uintptr_t[size_hint * 1];
      assert(data->field_user_data);
      if(!data->field_user_data) { all_alloc = false; }
      else { memset(data->field_user_data, 0, sizeof(uintptr_t) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    entity_destroy(data);
  }

  return all_alloc;
}


void
entity_destroy(Entity_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove name
    if(data->field_name) { delete[] data->field_name; }
    data->field_name = nullptr;

    // Remove tags
    if(data->field_tags) { delete[] data->field_tags; }
    data->field_tags = nullptr;

    // Remove components
    if(data->field_components) { delete[] data->field_components; }
    data->field_components = nullptr;

    // Remove renderer
    if(data->field_renderer) { delete[] data->field_renderer; }
    data->field_renderer = nullptr;

    // Remove user_data
    if(data->field_user_data) { delete[] data->field_user_data; }
    data->field_user_data = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
entity_push(Entity_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    entity_resize_capacity(data, data->capacity << 1);
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
entity_insert(Entity_data *data, const uint32_t key, const size_t insert_point)
{
  assert(data);
  assert(data->keys);
  assert(key);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    entity_resize_capacity(data, data->capacity << 1);
  }

  // Shuffle memory up
  {
    const size_t insert_index = insert_point + 1;
    const size_t start_index = insert_point;
    const size_t size_to_end = data->size - insert_point;

    // Shuffle the data down
    memmove(&data->keys[insert_point], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_name[insert_point * 32], &data->field_name[start_index * 32], size_to_end * sizeof(*data->field_name) * 32);
    memmove(&data->field_tags[insert_point * 1], &data->field_tags[start_index * 1], size_to_end * sizeof(*data->field_tags) * 1);
    memmove(&data->field_components[insert_point * 1], &data->field_components[start_index * 1], size_to_end * sizeof(*data->field_components) * 1);
    memmove(&data->field_renderer[insert_point * 1], &data->field_renderer[start_index * 1], size_to_end * sizeof(*data->field_renderer) * 1);
    memmove(&data->field_user_data[insert_point * 1], &data->field_user_data[start_index * 1], size_to_end * sizeof(*data->field_user_data) * 1);
  }

  // Insert new data
  {
    data->keys[data->size++] = key;
  }

  return false;
}


bool
entity_remove(Entity_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(entity_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_name[index_to_erase * 32], &data->field_name[start_index * 32], size_to_end * sizeof(*data->field_name) * 32);
    memmove(&data->field_tags[index_to_erase * 1], &data->field_tags[start_index * 1], size_to_end * sizeof(*data->field_tags) * 1);
    memmove(&data->field_components[index_to_erase * 1], &data->field_components[start_index * 1], size_to_end * sizeof(*data->field_components) * 1);
    memmove(&data->field_renderer[index_to_erase * 1], &data->field_renderer[start_index * 1], size_to_end * sizeof(*data->field_renderer) * 1);
    memmove(&data->field_user_data[index_to_erase * 1], &data->field_user_data[start_index * 1], size_to_end * sizeof(*data->field_user_data) * 1);

    return true;
  }

  return false;
}


bool
entity_exists(const Entity_data *data, const uint32_t key, size_t *out_index)
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
entity_clear(Entity_data *data)
{
  assert(data);

  data->size = 0;
}


bool
entity_is_empty(const Entity_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
entity_get_size(const Entity_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
entity_get_capacity(const Entity_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
entity_resize_capacity(Entity_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Entity_data new_data;
  const bool created_new = entity_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    entity_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_name, data->field_name, sizeof(char) * data->size * 32);
    memcpy(new_data.field_tags, data->field_tags, sizeof(uint32_t) * data->size * 1);
    memcpy(new_data.field_components, data->field_components, sizeof(uint32_t) * data->size * 1);
    memcpy(new_data.field_renderer, data->field_renderer, sizeof(uint32_t) * data->size * 1);
    memcpy(new_data.field_user_data, data->field_user_data, sizeof(uintptr_t) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    char *old_name = data->field_name;
    data->field_name = new_data.field_name;
    new_data.field_name = old_name;

    uint32_t *old_tags = data->field_tags;
    data->field_tags = new_data.field_tags;
    new_data.field_tags = old_tags;

    uint32_t *old_components = data->field_components;
    data->field_components = new_data.field_components;
    new_data.field_components = old_components;

    uint32_t *old_renderer = data->field_renderer;
    data->field_renderer = new_data.field_renderer;
    new_data.field_renderer = old_renderer;

    uintptr_t *old_user_data = data->field_user_data;
    data->field_user_data = new_data.field_user_data;
    new_data.field_user_data = old_user_data;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  entity_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
entity_get_const_name_data(const Entity_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


char*
entity_get_name_data(Entity_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


const uint32_t*
entity_get_const_tags_data(const Entity_data *data)
{
  assert(data);
  assert(data->field_tags);

  return data->field_tags;
}


uint32_t*
entity_get_tags_data(Entity_data *data)
{
  assert(data);
  assert(data->field_tags);

  return data->field_tags;
}


const uint32_t*
entity_get_const_components_data(const Entity_data *data)
{
  assert(data);
  assert(data->field_components);

  return data->field_components;
}


uint32_t*
entity_get_components_data(Entity_data *data)
{
  assert(data);
  assert(data->field_components);

  return data->field_components;
}


const uint32_t*
entity_get_const_renderer_data(const Entity_data *data)
{
  assert(data);
  assert(data->field_renderer);

  return data->field_renderer;
}


uint32_t*
entity_get_renderer_data(Entity_data *data)
{
  assert(data);
  assert(data->field_renderer);

  return data->field_renderer;
}


const void**
entity_get_const_user_data_data(const Entity_data *data)
{
  assert(data);
  assert(data->field_user_data);

  return reinterpret_cast<const void**>(data->field_user_data);
}


void**
entity_get_user_data_data(Entity_data *data)
{
  assert(data);
  assert(data->field_user_data);

  return reinterpret_cast<void**>(data->field_user_data);
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
entity_get_name(const Entity_data *data, const uint32_t key, const char **return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = &data->field_name[index];

      return true;
    }
  }

  return false;
}


bool
entity_set_name(const Entity_data *data, const uint32_t key, const char *set_value, const size_t size)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))

  index = index * 32;

  {
    assert(index < data->size);
    if(index < data->size * 32)
    {
      memcpy(&data->field_name[index], set_value, sizeof(char) * size);

      return true;
    }
  }

  return false;
}


bool
entity_get_tags(const Entity_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_tags);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_tags[index];

      return true;
    }
  }

  return false;
}


bool
entity_set_tags(const Entity_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_tags);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_tags[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
entity_get_components(const Entity_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_components);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_components[index];

      return true;
    }
  }

  return false;
}


bool
entity_set_components(const Entity_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_components);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_components[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
entity_get_renderer(const Entity_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_renderer);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_renderer[index];

      return true;
    }
  }

  return false;
}


bool
entity_set_renderer(const Entity_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_renderer);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_renderer[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
entity_get_user_data(const Entity_data *data, const uint32_t key, uintptr_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_user_data);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_user_data[index];

      return true;
    }
  }

  return false;
}


bool
entity_set_user_data(const Entity_data *data, const uint32_t key, const uintptr_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_user_data);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(entity_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_user_data[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
