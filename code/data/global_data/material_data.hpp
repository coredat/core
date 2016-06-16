

/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/


#ifndef MATERIAL_DATA_INCLUDED_042BBD82_D20D_4ACC_B123_AC04C40F549D
#define MATERIAL_DATA_INCLUDED_042BBD82_D20D_4ACC_B123_AC04C40F549D


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <systems/renderer_material/material.hpp>


namespace Resource_data {



/*!
  \brief Auto-generated structure for Material_data
*/
struct Material_data
{
  // Data Key
  util::generic_id *material_id = nullptr;

  // Properties
  char *property_name = nullptr;
  Material_renderer::Material_id *property_material_hash_id = nullptr;
  Material_renderer::Material *property_material = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


/*!
  \brief Initialize the Material_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialize.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
material_data_init(Material_data *data, const size_t size_hint);


/*!
  \brief Not currently implimented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
material_data_free(Material_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
material_data_get_size(const Material_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
material_data_get_capacity(const Material_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implimented).
  \param data The container to lock.
*/
void
data_lock(const Material_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implimented).
  \param The container to unlock
*/
void
data_unlock(const Material_data *data);


/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
material_data_push_back(Material_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
material_data_erase(Material_data *data, const util::generic_id key);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
material_data_exists(const Material_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_name.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
material_data_get_property_name(const Material_data *data, const util::generic_id key, const char **value);


/*!
  \brief Setter for property_name.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
material_data_set_property_name(Material_data *data,  const util::generic_id key, const char *value);


/*!
  \brief Searches for a given value.
  \param data The container to search.
  \param value The value which to search for.
  \param out_key Optional the key for that value.
  \return Returns true if the data was found.
*/
bool
material_data_search_property_name(const Material_data *data, const char *value, util::generic_id *out_key = nullptr);


/*!
  \brief Getter for property_material_hash_id.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
material_data_get_property_material_hash_id(const Material_data *data, const util::generic_id key, Material_renderer::Material_id *value);


/*!
  \brief Setter for property_material_hash_id.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
material_data_set_property_material_hash_id(Material_data *data,  const util::generic_id key, const Material_renderer::Material_id value);


/*!
  \brief Searches for a given value.
  \param data The container to search.
  \param value The value which to search for.
  \param out_key Optional the key for that value.
  \return Returns true if the data was found.
*/
bool
material_data_search_property_material_hash_id(const Material_data *data, const Material_renderer::Material_id value, util::generic_id *out_key = nullptr);


/*!
  \brief Getter for property_material.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
material_data_get_property_material(const Material_data *data, const util::generic_id key, Material_renderer::Material **value);


/*!
  \brief Setter for property_material.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
material_data_set_property_material(Material_data *data,  const util::generic_id key, const Material_renderer::Material *value);


} // ns


#endif // inc guard
