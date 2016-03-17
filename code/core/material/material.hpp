#ifndef MATERIAL_INCLUDED_0898BDC5_BA5E_4FB9_80D8_90978B814B3A
#define MATERIAL_INCLUDED_0898BDC5_BA5E_4FB9_80D8_90978B814B3A


#include <stdint.h>


namespace Core {


class Material final
{
public:

  const char *          get_name() const;
  
  uint32_t              get_shader_id() const;
  uint32_t              get_texture_id() const;
  
private:

};


} // ns


#endif // inc guard