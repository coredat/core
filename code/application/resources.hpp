#ifndef RESOURCES_INCLUDED_8C811BD6_4022_4F06_9C0E_FC8DFEEE4BF4
#define RESOURCES_INCLUDED_8C811BD6_4022_4F06_9C0E_FC8DFEEE4BF4


#include <data/world_data/world_data_fwd.hpp>
#include <stddef.h>


/*!
  Asset ids, and loading assets.
*/
namespace Resource {
namespace Model {


enum ENUM {

  unit_cube,
  unit_plane,
  test_level_01,
  
  size

}; // enum

} // ns


namespace Texture {

enum ENUM {
  
  // Speical dev textures.
  //dev_no_texture, // TODO: This crashes the system 
  
  // Temp textures for dev.
  dev_green,
  dev_red,
  dev_orange,
  dev_blue,
  dev_colored_squares,
  dev_squares,
  
  
  // Number of textures.
  size

}; // enum

} // ns


/*!
  loads the resources we will use for the game.
  \param texture_pool the desintation of the textures.
  \param size_of_texture_pool how big this pool is.
  \param model_pool the desintation of the model pool.
  \param size_of_model_pool size of the model pool.
*/
void
load_default_resources(World_data::Texture_pool *texture_pool,
                       const size_t size_of_texture_pool,
                       World_data::Model_pool *model_pool,
                       const size_t size_of_model_pool);


} // ns


#endif // inc guard