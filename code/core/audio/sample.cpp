#include <core/audio/sample.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/audio_pool.hpp>


namespace Core {


struct Sample::Impl
{
  uint32_t audio_id = 0;
};


Sample::Sample()
: m_impl(new Impl)
{
}


Sample::Sample(const char *filename)
: m_impl(new Impl)
{
  auto pool = Resource_data::get_resources()->audio_pool;
  Resource_data::audio_pool_add_new_sample(pool, filename);
}


Sample::Sample(const Sample &other)
: m_impl(new Impl(*other.m_impl.get()))
{
}


Sample::Sample(Sample &&other)
: m_impl(new Impl(*other.m_impl.get()))
{
}


Sample&
Sample::operator=(const Sample &other)
{
  return *this;
}


Sample&
Sample::operator=(Sample &&other)
{
  return *this;
}


Sample::~Sample()
{
}


void
Sample::play()
{
  auto pool = Resource_data::get_resources()->audio_pool;
  Resource_data::audio_pool_play_sample(pool);
}


bool
Sample::exists() const
{
  return !!m_impl->audio_id;
}

  
Sample::operator bool() const
{
  return exists();
}



} // ns