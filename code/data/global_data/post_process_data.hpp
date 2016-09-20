

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Tue 20 Sep 2016
*/


#ifndef POST_PROCESS_DATA_INCLUDED_2FF09CCF_EDA9_4A3E_9135_97E2AE2774C0
#define POST_PROCESS_DATA_INCLUDED_2FF09CCF_EDA9_4A3E_9135_97E2AE2774C0


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <systems/renderer_post/post_shader.hpp>


namespace Resource_data {


/*!
  \brief Auto-generated structure for Post_process_data
*/
struct Post_process_data
{
  // Data Key
  util::generic_id *post_process_id = nullptr;

  // Properties
  char *property_name = nullptr;
  Post_renderer::Post_shader *property_post_shader = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;


  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();
}; // struct


/*!
  \brief initialise the Post_process_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialise.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
post_process_data_init(Post_process_data *data, const size_t size_hint);


/*!
  \brief Not currently implemented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
post_process_data_free(Post_process_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
post_process_data_get_size(const Post_process_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
post_process_data_get_capacity(const Post_process_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implemented).
  \param data The container to lock.
*/
void
data_lock(const Post_process_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implemented).
  \param The container to unlock
*/
void
data_unlock(const Post_process_data *data);

/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
post_process_data_push_back(Post_process_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
post_process_data_erase(Post_process_data *data, const util::generic_id key);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
post_process_data_exists(const Post_process_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_name.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
post_process_data_get_property_name(const Post_process_data *data, const util::generic_id key, const char **value);


/*!
  \brief Setter for property_name.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
post_process_data_set_property_name(Post_process_data *data,  const util::generic_id key, const char *value);


/*!
  \brief Searches for a given value.
  \param data The container to search.
  \param value The value which to search for.
  \param out_key Optional the key for that value.
  \return Returns true if the data was found.
*/
bool
post_process_data_search_property_name(const Post_process_data *data, const char *value, util::generic_id *out_key = nullptr);


/*!
  \brief Getter for property_post_shader.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
post_process_data_get_property_post_shader(const Post_process_data *data, const util::generic_id key, Post_renderer::Post_shader **value);


/*!
  \brief Setter for property_post_shader.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
post_process_data_set_property_post_shader(Post_process_data *data,  const util::generic_id key, const Post_renderer::Post_shader *value);


} // ns


#endif // inc guard
