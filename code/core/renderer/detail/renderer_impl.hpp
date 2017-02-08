#ifndef RENDERER_IMPL_INCLUDED_554F5441_EA7A_453E_8E65_4D42E424EFF5
#define RENDERER_IMPL_INCLUDED_554F5441_EA7A_453E_8E65_4D42E424EFF5


#include <string>
#include <stdint.h>


namespace Core_detail {


struct Renderer_impl
{
  uint32_t renderer_type;
  
  union
  {
    struct Material_impl
    {
      uint32_t model_id;
      uint32_t material_id;
    };
    
    struct Text_impl
    {
      std::string text;
    };
  };
};


} // ns


#endif // inc guard