#ifndef SHADER_INCLUDED_987AF2DE_4AAB_47E3_9742_51C7B5433AB0
#define SHADER_INCLUDED_987AF2DE_4AAB_47E3_9742_51C7B5433AB0


#include <memory>
#include <stdint.h>


namespace Core {


class Shader final
{
public:

  explicit                  Shader();                           //!< Creates an invalid shader.
  explicit                  Shader(const char *filename);       //!< Loads a shader from an file, must be in tagged format.
  explicit                  Shader(const char *name,
                                   const char *vs,
                                   const char *gs,
                                   const char *ps);             //!< Creates a shader from given code. gs maybe empty or null.
                            ~Shader();
  
                            Shader(const Shader &shd);
  
  bool                      is_valid() const;                   //!< Returns true if the shader loaded successfully.
                            operator bool() const;              //!< Alias for is_valid().

  uint32_t                  get_id() const;                     //!< Gets the internal id. this will be removed from public interface.
  
private:

  struct Impl;
  std::unique_ptr<Impl>     m_impl;
  
};


} // ns


#endif // inc guard