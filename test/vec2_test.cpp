// Test for vector2
#include "../math/vector/vector.hpp"
#include "../math/general/to_string.hpp"
#include "test_asserts.hpp"
#include <iostream>
#include <cmath>	


int main()
{
	std::cout << "vector 2 tests - started" << std::endl;

	// Test Constants
	{
		const caffmath::vector2 zero_vector2 = caffmath::vector2_zero();
		assert_test("test vector2 const is all zero", vector2_components_are(zero_vector2, 0,0));

		const caffmath::vector2 one_vector2 = caffmath::vector2_one();
		assert_test("test vector2 const is all ones", vector2_components_are(one_vector2, 1,1));

		const caffmath::vector2 zero_one_vector2 = caffmath::vector2_zero_one();
		assert_test("test vector2 const is 0,1", vector2_components_are(zero_one_vector2, 0,1));
	}

	// Test Init
	{
		const caffmath::vector2 zero_vector2 = caffmath::vector2_init(0);
		assert_test("test vector2 is all zero", vector2_components_are(zero_vector2, 0,0));

		const caffmath::vector2 one_vector2 = caffmath::vector2_init(1);
		assert_test("test vector2 is all ones", vector2_components_are(one_vector2, 1,1));
		
		const caffmath::vector2 one_two_vector2 = caffmath::vector2_init(1.1f, 2.2f);
		assert_test("test components", vector2_components_are(one_two_vector2, 1.1f, 2.2f));

		std::array<float, 2> my_array = {1.1f, 2.2f};
		const caffmath::vector2 init_with_c_array = caffmath::vector2_init_with_array(&my_array[0]);
		assert_test("test components", vector2_components_are(init_with_c_array, 1.1f, 2.2f));

		const caffmath::vector2 init_with_std_array = caffmath::vector2_init_with_array(my_array);
		assert_test("test components", vector2_components_are(init_with_std_array, 1.1f, 2.2f));
	}

	// Test Basic Arithmetic
	{
		const caffmath::vector2 one_vector2 = caffmath::vector2_init(1);
		const caffmath::vector2 one_two_vector2 = caffmath::vector2_init(1.1f, 2.2f);
		const float error = 0.0001f;

		const caffmath::vector2 add_result = caffmath::vector2_add(one_two_vector2, one_vector2);
		assert_test("test add", vector2_components_are_near(add_result, 2.1f, 3.2f, error));

		const caffmath::vector2 sub_result = caffmath::vector2_subtract(one_two_vector2, one_vector2);
		assert_test("test subtract", vector2_components_are_near(sub_result, 0.1f, 1.2f, error));

		const caffmath::vector2 mul_result = caffmath::vector2_multiply(one_two_vector2, one_vector2);
		assert_test("test multiply", vector2_components_are_near(mul_result, 1.1f, 2.2f, error));

		const caffmath::vector2 div_result = caffmath::vector2_divide(one_two_vector2, one_vector2);
		assert_test("test divide", vector2_components_are_near(div_result, 1.1f, 2.2f, error));
	}

	// Test Special Operations
	{
		const caffmath::vector2 one_two_vector2 = caffmath::vector2_init(1.1f, 2.2f);
		const caffmath::vector2 two_two_vector2 = caffmath::vector2_init(2.2f, 2.2f);
		const float error = 0.0001f;

		// Lerp
		{
			const caffmath::vector2 erp_start 	= caffmath::vector2_zero();
			const caffmath::vector2 erp_end 	= caffmath::vector2_one();

			const caffmath::vector2 dt_0 = vector2_lerp(erp_start, erp_end, 0.f);
			assert_test("lerp at 0", vector2_components_are_near(dt_0, 0.f, 0.f, error));

			const caffmath::vector2 dt_025 = vector2_lerp(erp_start, erp_end, 0.25f);
			assert_test("lerp at 0.25", vector2_components_are_near(dt_025, 0.25f, 0.25f, error));

			const caffmath::vector2 dt_05 = vector2_lerp(erp_start, erp_end, 0.5f);
			assert_test("lerp at 0.5", vector2_components_are_near(dt_05, 0.5f, 0.5f, error));

			const caffmath::vector2 dt_075 = vector2_lerp(erp_start, erp_end, 0.75f);
			assert_test("lerp at 0.75", vector2_components_are_near(dt_075, 0.75f, 0.75f, error));

			const caffmath::vector2 dt_1 = vector2_lerp(erp_start, erp_end, 1.f);
			assert_test("lerp at 1", vector2_components_are_near(dt_1, 1.f, 1.f, error));
		}

		// Slerp
		{
			assert_test("slerp", float_is_near(1,2,error));
		}

		const caffmath::vector2 scale_by_2_and_half = vector2_scale(one_two_vector2, 2.5f);
		assert_test("test scale", vector2_components_are_near(scale_by_2_and_half, 2.75f, 5.5f, error));

		const float length_of_vec = vector2_length(one_two_vector2);
		assert_test("test length", float_is_near(length_of_vec, 6.02495f, error));

		const caffmath::vector2 normalize_vector = vector2_normalize(one_two_vector2);
		assert_test("test normalize", vector2_components_are_near(normalize_vector, 0.182574f, 0.365148f, error));

		const float dot_prod = vector2_dot(one_two_vector2, two_two_vector2);
		assert_test("test dot product", float_is_near(dot_prod, 22.99f, error));
	}

	std::cout << "vector 2 tests - finished" << std::endl;

	return 0;
}