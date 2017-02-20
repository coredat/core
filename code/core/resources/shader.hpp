#ifndef SHADER_INCLUDED_987AF2DE_4AAB_47E3_9742_51C7B5433AB0
#define SHADER_INCLUDED_987AF2DE_4AAB_47E3_9742_51C7B5433AB0


#include <memory>
#include <stdint.h>


namespace Core {


class Shader final
{
public:

  explicit                  Shader();
  explicit                  Shader(const char *filename);
  explicit                  Shader(const char *name,
                                   const char *vs,
                                   const char *gs,
                                   const char *ps);
                            ~Shader();
  
                            Shader(const Shader &shd);
  
  Shader&                   operator=(const Shader &other);
  
  bool                      is_valid() const;
                            operator bool() const;

  uint32_t                  get_id() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl>     m_impl;
  
};


} // ns


#endif // inc guard