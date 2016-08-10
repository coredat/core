

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Wed 10 Aug 2016
*/


#ifndef TEXT_MESH_DATA_INCLUDED_A67940BE_9041_4897_B97B_F924308F19C1
#define TEXT_MESH_DATA_INCLUDED_A67940BE_9041_4897_B97B_F924308F19C1


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <stdint.h>
#include <graphics_api/mesh.hpp>


namespace Resource_data {


/*!
  \brief Auto-generated structure for Text_mesh_data
*/
struct Text_mesh_data
{
  // Data Key
  util::generic_id *text_mesh_id = nullptr;

  // Properties
  char *property_text = nullptr;
  uint32_t *property_text_size = nullptr;
  util::generic_id *property_font_id = nullptr;
  Graphics_api::Mesh *property_mesh = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  util::generic_id key_count = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();
}; // struct


/*!
  \brief initialise the Text_mesh_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialise.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
text_mesh_data_init(Text_mesh_data *data, const size_t size_hint);


/*!
  \brief Not currently implemented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
text_mesh_data_free(Text_mesh_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
text_mesh_data_get_size(const Text_mesh_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
text_mesh_data_get_capacity(const Text_mesh_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implemented).
  \param data The container to lock.
*/
void
data_lock(const Text_mesh_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implemented).
  \param The container to unlock
*/
void
data_unlock(const Text_mesh_data *data);

/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns the id of the data.
*/
util::generic_id
text_mesh_data_push_back(Text_mesh_data *data, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
text_mesh_data_erase(Text_mesh_data *data, const util::generic_id key);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
text_mesh_data_exists(const Text_mesh_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_text.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
text_mesh_data_get_property_text(const Text_mesh_data *data, const util::generic_id key, const char **value);


/*!
  \brief Setter for property_text.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
text_mesh_data_set_property_text(Text_mesh_data *data,  const util::generic_id key, const char *value);


/*!
  \brief Searches for a given value.
  \param data The container to search.
  \param value The value which to search for.
  \param out_key Optional the key for that value.
  \return Returns true if the data was found.
*/
bool
text_mesh_data_search_property_text(const Text_mesh_data *data, const char *value, util::generic_id *out_key = nullptr);


/*!
  \brief Getter for property_text_size.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
text_mesh_data_get_property_text_size(const Text_mesh_data *data, const util::generic_id key, uint32_t *value);


/*!
  \brief Setter for property_text_size.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
text_mesh_data_set_property_text_size(Text_mesh_data *data,  const util::generic_id key, const uint32_t value);


/*!
  \brief Getter for property_font_id.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
text_mesh_data_get_property_font_id(const Text_mesh_data *data, const util::generic_id key, util::generic_id *value);


/*!
  \brief Setter for property_font_id.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
text_mesh_data_set_property_font_id(Text_mesh_data *data,  const util::generic_id key, const util::generic_id value);


/*!
  \brief Getter for property_mesh.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
text_mesh_data_get_property_mesh(const Text_mesh_data *data, const util::generic_id key, Graphics_api::Mesh *value);


/*!
  \brief Setter for property_mesh.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
text_mesh_data_set_property_mesh(Text_mesh_data *data,  const util::generic_id key, const Graphics_api::Mesh value);


} // ns


#endif // inc guard
