#ifndef LOGIC_BASE_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35
#define LOGIC_BASE_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35


#include <iostream>
#include <systems/entity/entity_id.hpp>
#include <data/data_fwd.hpp> // TODO: Stink! This isn't nice.
#include <systems/physics/physics.hpp>
#include <math/transform/transform.hpp>


namespace Logic {


class Base
{
public:


  explicit Base() { std::cout << "ctor" << std::endl; }
  virtual ~Base() {}
  
  virtual void on_start() { std::cout << "start" << std::endl; }
  virtual void on_update(const float dt) {}
  virtual void on_end() {}
  
  math::transform get_transform() const;
  void set_transform(math::transform &trans);
  
  
  void                set_entity(const Entity::Entity_id entity) { m_entity = entity; };
  Entity::Entity_id   get_entity() const { return m_entity; }
  
  void                set_entity_data(Data::Entity_pool *data) { m_data_pool = data; }
  void                set_physics_data(Physics::World *world) { m_world = world; }


  Physics::World *m_world; // TODO: Move
  
private:

  Entity::Entity_id m_entity = Entity::invalid_id();
  Data::Entity_pool *m_data_pool = nullptr;



}; // class


} // ns


#endif // inc guard