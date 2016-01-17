#ifndef APPLICATION_GRAPHICS_INCLUDED_
#define APPLICATION_GRAPHICS_INCLUDED_


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