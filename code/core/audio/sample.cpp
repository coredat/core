#include <core/audio/sample.hpp>
#include <data/resource_data/resource_data.hpp>
#include <data/resource_data/audio_pool.hpp>


namespace Core {


struct Sample::Impl
{
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
Sample::play(const math::vec3 position)
{
  auto pool = Resource_data::get_resources()->audio_pool;
  Resource_data::audio_pool_play_sample(pool);
}


} // ns