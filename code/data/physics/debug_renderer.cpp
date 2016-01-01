#include "debug_renderer.hpp"
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <iostream>
#include <stdint.h>


namespace Physics {


Debug_renderer::Debug_renderer()
{
	const uint32_t draw_modes(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
	setDebugMode(draw_modes);
}


Debug_renderer::~Debug_renderer()
{
}


void
Debug_renderer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
  Debug_line_renderer::Line_node node;
  
  node.position_from[0] = from.x();
  node.position_from[1] = from.y();
  node.position_from[2] = from.z();
  
  node.position_to[0] = to.x();
  node.position_to[1] = to.y();
  node.position_to[2] = to.z();
  
  node.color[0] = color.x();
  node.color[1] = color.y();
  node.color[2] = color.z();

  Debug_line_renderer::add_lines(&node, 1);
}


void
Debug_renderer::drawContactPoint(const btVector3 &point_on_b,
                                 const btVector3 &normal_on_b,
                                 btScalar distance,
                                 int lifeTime,
                                 const btVector3 &color)
{
  const float half_size_of_contact_point = 0.1f;
  const float max_x = point_on_b.x() + half_size_of_contact_point;
  const float min_x = point_on_b.x() - half_size_of_contact_point;

  const float max_y = point_on_b.y() + half_size_of_contact_point;
  const float min_y = point_on_b.y() - half_size_of_contact_point;
  
  const float max_z = point_on_b.z() + half_size_of_contact_point;
  const float min_z = point_on_b.z() - half_size_of_contact_point;
  
  auto add_line = [](Debug_line_renderer::Line_node *node,
                     const float start_x,
                     const float start_y,
                     const float start_z,
                     const float end_x,
                     const float end_y,
                     const float end_z,
                     const float color_r,
                     const float color_g,
                     const float color_b)
  {
    node->position_from[0] = start_x;
    node->position_from[1] = start_y;
    node->position_from[2] = start_z;
    
    node->position_to[0] = end_x;
    node->position_to[1] = end_y;
    node->position_to[2] = end_z;
    
    node->color[0] = color_r;
    node->color[1] = color_g;
    node->color[2] = color_b;
  };
  
  const std::size_t number_of_nodes = 12;
  Debug_line_renderer::Line_node nodes[number_of_nodes];

  // x facing lines
  add_line(&nodes[0], min_x, min_y, min_z, max_x, min_y, min_z, color.x(), color.y(), color.z());
  add_line(&nodes[1], min_x, max_y, min_z, max_x, max_y, min_z, color.x(), color.y(), color.z());
  add_line(&nodes[2], min_x, min_y, max_z, max_x, min_y, max_z, color.x(), color.y(), color.z());
  add_line(&nodes[3], min_x, max_y, max_z, max_x, max_y, max_z, color.x(), color.y(), color.z());

  // y faceing lines
  add_line(&nodes[4], min_x, min_y, min_z, min_x, max_y, min_z, color.x(), color.y(), color.z());
  add_line(&nodes[5], max_x, min_y, min_z, max_x, max_y, min_z, color.x(), color.y(), color.z());
  add_line(&nodes[6], min_x, min_y, max_z, min_x, max_y, max_z, color.x(), color.y(), color.z());
  add_line(&nodes[7], max_x, min_y, max_z, max_x, max_y, max_z, color.x(), color.y(), color.z());

  // z faceing lines
  add_line(&nodes[8],  min_x, min_y, min_z, min_x, min_y, max_z, color.x(), color.y(), color.z());
  add_line(&nodes[9],  max_x, min_y, min_z, max_x, min_y, max_z, color.x(), color.y(), color.z());
  add_line(&nodes[10], min_x, max_y, min_z, min_x, max_y, max_z, color.x(), color.y(), color.z());
  add_line(&nodes[11], max_x, max_y, min_z, max_x, max_y, max_z, color.x(), color.y(), color.z());
  
  Debug_line_renderer::add_lines(nodes, number_of_nodes);
}


void
Debug_renderer::reportErrorWarning(const char *warning_string)
{
	// TODO: DebugLog better for this.
	std::cout << "[BulletPhysics Warning]: " << warning_string << std::endl;
}


void
Debug_renderer::draw3dText(const btVector3 &location, const char *text_string)
{
}


void
Debug_renderer::setDebugMode(int debug_mode)
{
  m_debug_mode = debug_mode;
}


int
Debug_renderer::getDebugMode() const
{
  return m_debug_mode;
}


} // ns