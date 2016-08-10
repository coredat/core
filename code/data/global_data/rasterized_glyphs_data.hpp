

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Wed 10 Aug 2016
*/


#ifndef RASTERIZED_GLYPHS_DATA_INCLUDED_5BA7512D_1B9A_4B0E_A755_2E32D5583993
#define RASTERIZED_GLYPHS_DATA_INCLUDED_5BA7512D_1B9A_4B0E_A755_2E32D5583993


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <systems/text/character.hpp>


namespace Resource_data {


/*!
  \brief Auto-generated structure for Rasterized_glyphs_data
*/
struct Rasterized_glyphs_data
{
  // Data Key
  util::generic_id *rasterized_glyphs_id = nullptr;

  // Properties
  Text::Character *property_character = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;


  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();
}; // struct


/*!
  \brief initialise the Rasterized_glyphs_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialise.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
rasterized_glyphs_data_init(Rasterized_glyphs_data *data, const size_t size_hint);


/*!
  \brief Not currently implemented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
rasterized_glyphs_data_free(Rasterized_glyphs_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
rasterized_glyphs_data_get_size(const Rasterized_glyphs_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
rasterized_glyphs_data_get_capacity(const Rasterized_glyphs_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implemented).
  \param data The container to lock.
*/
void
data_lock(const Rasterized_glyphs_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implemented).
  \param The container to unlock
*/
void
data_unlock(const Rasterized_glyphs_data *data);

/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
rasterized_glyphs_data_push_back(Rasterized_glyphs_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
rasterized_glyphs_data_erase(Rasterized_glyphs_data *data, const util::generic_id key);


/*!
  \brief Inserts a new element shuffling down any data that exists in the array. (Does not take a lock).
  \param data The container to insert into.
  \param key The key which is used to remove the data.
  \param insert_point Where to insert. If bigger then the size it will push back.
  \return Returns true if it was successful.
*/
bool
rasterized_glyphs_data_insert(Rasterized_glyphs_data *data, const util::generic_id key, const size_t insert_index);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
rasterized_glyphs_data_exists(const Rasterized_glyphs_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_character.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
rasterized_glyphs_data_get_property_character(const Rasterized_glyphs_data *data, const util::generic_id key, Text::Character *value);


/*!
  \brief Setter for property_character.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
rasterized_glyphs_data_set_property_character(Rasterized_glyphs_data *data,  const util::generic_id key, const Text::Character value);


} // ns


#endif // inc guard
