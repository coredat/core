#ifndef RESOURCES_INCLUDED_8C811BD6_4022_4F06_9C0E_FC8DFEEE4BF4
#define RESOURCES_INCLUDED_8C811BD6_4022_4F06_9C0E_FC8DFEEE4BF4


#include <data/data_fwd.hpp>
#include <cstddef>


namespace Resource {
namespace Model {


enum ENUM {

  unit_cube,
  unit_plane,
  
  size

}; // enum

} // ns


namespace Texture {

enum ENUM {
  
  dev_green,
  dev_red,
  dev_orange,
  dev_blue,
  
  size

}; // enum

} // ns


void
load_default_resources(Data::Texture_pool *texture_pool, std::size_t size_of_texture_pool, Data::Model_pool *model_pool, std::size_t size_of_model_pool);


} // ns


#endif // inc guard