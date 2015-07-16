#include <unit_test/unit_test.hpp>
#include <math/transform/transform.hpp>
#include <test_asserts.hpp>


namespace
{
  const caffmath::transform test_data_01;
  const caffmath::transform test_data_02 = caffmath::transform_init(
      caffmath::vector3_init(10.f, 10.f, 10.f),
      caffmath::vector3_init(2.f, 2.f, 2.f),
      caffmath::quaternion_init_with_axis_angle(0, 1, 0, caffmath::quart_tau())
    );
const caffmath::transform test_data_03 = caffmath::transform_init(
      caffmath::vector3_init(15.f, 15.f, 15.f),
      caffmath::vector3_init(3.f, 3.f, 3.f),
      caffmath::quaternion_init_with_axis_angle(0, 1, 0, caffmath::half_tau())
    );
}


TEST(transform_inherited)
{

}


int main()
{


  return 0;
}