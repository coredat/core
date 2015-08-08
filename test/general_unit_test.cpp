#include <unit_test/unit_test.hpp>
#include <math/general/general.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  const float error = 0.0001f;
}


TEST(constants)
{
  const float pi = 3.1415927f;

  CHECK(test::is_near<float>(caffmath::pi(), pi, error));
  CHECK(test::is_near<float>(caffmath::two_pi(), 2.f * pi, error));
  CHECK(test::is_near<float>(caffmath::half_pi(), 0.5f * pi, error));

  CHECK(test::is_near<float>(caffmath::tau(), caffmath::two_pi(), error));
  CHECK(test::is_near<float>(caffmath::half_tau(), caffmath::pi(), error));
  CHECK(test::is_near<float>(caffmath::quart_tau(), 0.5f * caffmath::pi(), error));

  // TODO: g-ratio

  CHECK(test::is_near<float>(caffmath::root_two(), std::sqrt(2.f), error));
  CHECK(test::is_near<float>(caffmath::root_three(), std::sqrt(3.f), error));
}


TEST(trig)
{

}


TEST(rads_degrees)
{

}


TEST(general)
{

}


int main()
{
  test::run_tests();

  return 0;
}