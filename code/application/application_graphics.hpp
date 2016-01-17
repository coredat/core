#ifndef APPLICATION_GRAPHICS_INCLUDED_8F2790BB_D7C5_49E1_B6CA_A7AE043D2C57
#define APPLICATION_GRAPHICS_INCLUDED_8F2790BB_D7C5_49E1_B6CA_A7AE043D2C57


#include <data/data_fwd.hpp>
#include <renderer/simple_renderer/simple_renderer_node.hpp>


namespace Application {


/*!
   Process what to render on the screen.
*/
void
graphics_think(const Data::Entity_pool *entities,
               const Data::Texture_pool *texture_pool,
               const Data::Model_pool *model_pool,
               Simple_renderer::Node nodes[],
               const std::size_t size_of_node_pool);


} // ns


#endif // inc guard