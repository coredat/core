

/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/


#ifndef RENDERER_MESH_DATA_INCLUDED_8500FF5E_D380_495D_BA10_86F81661F721
#define RENDERER_MESH_DATA_INCLUDED_8500FF5E_D380_495D_BA10_86F81661F721


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>


namespace World_data {


struct Mesh_renderer_draw_call {
  uint32_t model;
  uint32_t texture; // Move this to the material.
  float    world_matrix[16];
};




/*!
  \brief Auto-generated structure for Renderer_mesh_data
*/
struct Renderer_mesh_data
{
  // Data Key
  util::generic_id *entity_id = nullptr;

  // Properties
  Mesh_renderer_draw_call *property_draw_call = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


/*!
  \brief Initialize the Renderer_mesh_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialize.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
renderer_mesh_data_init(Renderer_mesh_data *data, const size_t size_hint);


/*!
  \brief Not currently implimented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
renderer_mesh_data_free(Renderer_mesh_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
renderer_mesh_data_get_size(const Renderer_mesh_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
renderer_mesh_data_get_capacity(const Renderer_mesh_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implimented).
  \param data The container to lock.
*/
void
data_lock(Renderer_mesh_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implimented).
  \param The container to unlock
*/
void
data_unlock(Renderer_mesh_data *data);


/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
renderer_mesh_data_push_back(Renderer_mesh_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
renderer_mesh_data_erase(Renderer_mesh_data *data, const util::generic_id key);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
renderer_mesh_data_exists(const Renderer_mesh_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_draw_call.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
renderer_mesh_data_get_property_draw_call(const Renderer_mesh_data *data, const util::generic_id key, Mesh_renderer_draw_call **value);


/*!
  \brief Setter for property_draw_call.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
renderer_mesh_data_set_property_draw_call(Renderer_mesh_data *data,  const util::generic_id key, const Mesh_renderer_draw_call *value);


} // ns


#endif // inc guard
