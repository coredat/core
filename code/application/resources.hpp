#ifndef RESOURCES_INCLUDED_8C811BD6_4022_4F06_9C0E_FC8DFEEE4BF4
#define RESOURCES_INCLUDED_8C811BD6_4022_4F06_9C0E_FC8DFEEE4BF4


#include <data/world_data/world.hpp>
#include <stddef.h>


/*!
  Asset ids, and loading assets.
*/
namespace Resource {
namespace Model {
enum ENUM {

  unit_cube,
  unit_plane,
  unit_tube,

  test_aabb,
  
  // Number of models.
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


namespace Audio {
enum ENUM {

  gun_shot,
  
  // Number of audio samples.
  size

}; // enum
} // ns

} // ns


/*!
  loads the resources we will use for the game.
  \param texture_pool the desintation of the textures.
  \param size_of_texture_pool how big this pool is.
  \param model_pool the desintation of the model pool.
  \param size_of_model_pool size of the model pool.
*/
void
load_default_resources(World_data::World *world);


} // ns


#endif // inc guard