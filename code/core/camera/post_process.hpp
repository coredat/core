#ifndef POST_PROCESS_INCLUDED_D44ABCD3_122A_49D4_B40C_801091A802D1
#define POST_PROCESS_INCLUDED_D44ABCD3_122A_49D4_B40C_801091A802D1


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Post_process
{
public:

  explicit      Post_process();
                ~Post_process();
  
  void          set_shader(const Shader &shader);
  
  void          set_input_01(const Texture &texture);
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard