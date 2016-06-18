

/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/


#ifndef MESH_DATA_INCLUDED_3A7EBC9B_3721_495F_96E9_E5A45B6030D1
#define MESH_DATA_INCLUDED_3A7EBC9B_3721_495F_96E9_E5A45B6030D1


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <graphics_api/mesh.hpp>
#include <math/geometry/geometry_types.hpp>


namespace Resource_data {



/*!
  \brief Auto-generated structure for Mesh_data
*/
struct Mesh_data
{
  // Data Key
  util::generic_id *mesh_id = nullptr;

  // Properties
  char *property_name = nullptr;
  Graphics_api::Mesh *property_mesh = nullptr;
  math::aabb *property_aabb = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;


  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


/*!
  \brief Initialize the Mesh_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialize.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
mesh_data_init(Mesh_data *data, const size_t size_hint);


/*!
  \brief Not currently implimented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
mesh_data_free(Mesh_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
mesh_data_get_size(const Mesh_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
mesh_data_get_capacity(const Mesh_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implimented).
  \param data The container to lock.
*/
void
data_lock(const Mesh_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implimented).
  \param The container to unlock
*/
void
data_unlock(const Mesh_data *data);

/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
mesh_data_push_back(Mesh_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
mesh_data_erase(Mesh_data *data, const util::generic_id key);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
mesh_data_exists(const Mesh_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_name.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
mesh_data_get_property_name(const Mesh_data *data, const util::generic_id key, const char **value);


/*!
  \brief Setter for property_name.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
mesh_data_set_property_name(Mesh_data *data,  const util::generic_id key, const char *value);


/*!
  \brief Searches for a given value.
  \param data The container to search.
  \param value The value which to search for.
  \param out_key Optional the key for that value.
  \return Returns true if the data was found.
*/
bool
mesh_data_search_property_name(const Mesh_data *data, const char *value, util::generic_id *out_key = nullptr);


/*!
  \brief Getter for property_mesh.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
mesh_data_get_property_mesh(const Mesh_data *data, const util::generic_id key, Graphics_api::Mesh *value);


/*!
  \brief Setter for property_mesh.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
mesh_data_set_property_mesh(Mesh_data *data,  const util::generic_id key, const Graphics_api::Mesh value);


/*!
  \brief Getter for property_aabb.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
mesh_data_get_property_aabb(const Mesh_data *data, const util::generic_id key, math::aabb *value);


/*!
  \brief Setter for property_aabb.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
mesh_data_set_property_aabb(Mesh_data *data,  const util::generic_id key, const math::aabb value);


} // ns


#endif // inc guard
