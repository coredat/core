#include <data/world_data/renderer_mesh_data.hpp>
#include <string.h>
#include <assert.h>


namespace World_data {
  

void
mesh_renderer_init(Mesh_renderer_data *data)
{
  static util::generic_id ents[2048];
  data->entity_id = ents;
  
  static uint32_t model_ids[2048];
  data->model = model_ids;
  
  static uint32_t texture_ids[2048];
  data->texture = texture_ids;
  
  data->size = 0;
}


void
mesh_renderer_add(Mesh_renderer_data *data,
                  const util::generic_id id,
                  const uint32_t model_id,
                  const uint32_t texture_id)
{
  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    assert(false); // already exsits, maybe just update the detail with log msg.
    return;
  }
  
  data->entity_id[data->size] = id;
  data->model[data->size] = model_id;
  data->texture[data->size] = texture_id;
  ++(data->size);
}


void
mesh_renderer_update_model(Mesh_renderer_data *data,
                           const util::generic_id id,
                           const uint32_t model_id)
{
  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    data->model[index] = model_id;
  }
}


void
mesh_renderer_update_texture(Mesh_renderer_data *data,
                             const util::generic_id id,
                             const uint32_t texture_id)
{
  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    data->texture[index] = texture_id;
  }
}


void
mesh_renderer_remove(Mesh_renderer_data *data,
                     const util::generic_id id)
{
  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    const size_t start_move = index + 1;
    const size_t end_move = data->size - index - 1;
    --(data->size);
    
    memmove(&data->entity_id[index],  &data->entity_id[start_move],  end_move * sizeof(*data->entity_id));
    memmove(&data->model[index],      &data->model[start_move],      end_move * sizeof(*data->model));
    memmove(&data->texture[index],    &data->texture[start_move],    end_move * sizeof(*data->texture));
  }
  else
  {
    assert(false); // change to warning.
  }
}
  
  
bool
mesh_renderer_exists(Mesh_renderer_data *data,
                     const util::generic_id id,
                     size_t *index)
{
  size_t *search_index = index;
  size_t dummy_index;
  
  if(data->size == 0)
  {
    return false;
  }
  
  if(search_index == nullptr)
  {
    search_index = &dummy_index;
  }
  
  return util::generic_id_search_binary(search_index, id, data->entity_id, data->size);
}
  
  
} // ns