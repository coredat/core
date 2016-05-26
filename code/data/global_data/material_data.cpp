#include <data/global_data/material_data.hpp>
#include <assert.h>
#include <cstring>


namespace
{
  constexpr uint32_t max_name_size = 256;
}


namespace Resource_data {


void
lock(Material_data *data)
{
}


void
unlock(Material_data *data)
{
}


void
material_data_init(Material_data *data, const uint32_t size_hint)
{
  assert(data && size_hint);
  
  lock(data);
  
  constexpr uint32_t size = 2048;
  
  static util::generic_id mat_ids[size];
  data->material_id = mat_ids;
  #ifndef NDEBUG
  memset(data->material_id, 0, sizeof(mat_ids));
  #endif
  
  static char mat_names[size * max_name_size];
  data->material_name = mat_names;
  #ifndef NDEBUG
  memset(data->material_name, 0, sizeof(mat_names));
  #endif
  
  static Material mats[size];
  data->material = mats;
  #ifndef NDEBUG
  memset(data->material, 0, sizeof(mats));
  #endif
  
  data->size = 0;

  uint32_t *cap = const_cast<uint32_t*>(&data->capacity);
  *cap = size;
  
  unlock(data);
}


void
material_data_free(Material_data *data)
{
}


util::generic_id
material_data_add(Material_data *data,
                  const char *name,
                  const util::generic_id shader_id,
                  const util::generic_id texture_map_01)
{
  assert(data && name);
  
  lock(data);
  
  // Check to see if a material exists.
  
  
  
  // Insert new material.
  
  const util::generic_id id = data->size + 1;
  const uint32_t index = data->size;
  
  Material mat;
  mat.shader_id       = shader_id;
  mat.texture_map_01  = texture_map_01;
  
  data->material_id[index] = id;
  data->material[index] = mat;
  strlcpy(&data->material_name[index * max_name_size], name, max_name_size);
  
  ++(data->size);
  
  unlock(data);
  
  return id;
}


const char *
material_data_get_name(Material_data *data,
                       const util::generic_id id)
{
  lock(data);
  

  
  unlock(data);
  
  
}


bool
material_data_exsits(const Material_data *data,
                     const util::generic_id id,
                     size_t *out_index)
{
  assert(data && id);
  
  size_t *search_index = out_index;
  size_t dummy_index;
  
  if(data->size == 0)
  {
    return false;
  }
  
  if(search_index == nullptr)
  {
    search_index = &dummy_index;
  }
  
  const bool exists = util::generic_id_search_binary(search_index,
                                                     id,
                                                     data->material_id,
                                                     data->size);
  
  return exists;
}


} // ns