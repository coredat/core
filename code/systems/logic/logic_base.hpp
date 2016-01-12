#ifndef LOGIC_BASE_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35
#define LOGIC_BASE_INCLUDED_4D9CAF99_66F7_47AA_8460_AEDF08D1BB35


#include <iostream>


namespace Logic {


class Base
{
public:


  explicit Base() { std::cout << "ctor" << std::endl; }
  virtual ~Base() {}
  
  virtual void on_start()   { std::cout << "start" << std::endl; }
  virtual void on_update(const float dt)  {}
  virtual void on_end()     {}
  
  void get_transform() const {
//    world_entities->get_transform(entity_id);
  };


}; // class


} // ns


#endif // inc guard