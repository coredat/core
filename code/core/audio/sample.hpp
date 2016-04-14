#ifndef SAMPLE_INCLUDED_2EE1288C_F206_4EBA_9808_69B3456E0B13
#define SAMPLE_INCLUDED_2EE1288C_F206_4EBA_9808_69B3456E0B13


#include <math/vec/vec_types.hpp>
#include <memory>


namespace Core {


class Sample final
{
public:

  explicit        Sample();
  explicit        Sample(const char *filename);
                  ~Sample();
  
                  Sample(const Sample&);
                  Sample(Sample&&);
  
  Sample&         operator=(const Sample&);
  Sample&         operator=(Sample&&);
  
  void            play(const math::vec3 position);
  void            set_volume(const float vol);
  float           get_volume() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard