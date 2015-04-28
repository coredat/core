// Test for vector4
#include "../math/vector/vector.hpp"
#include <iostream>
#include <cmath>	


bool vector4_is_equal(const caffmath::vector4 a, const caffmath::vector4 b)
{
	if(caffmath::vector4_get_x(a) == caffmath::vector4_get_x(b) &&
	   caffmath::vector4_get_y(a) == caffmath::vector4_get_y(b) &&
	   caffmath::vector4_get_z(a) == caffmath::vector4_get_z(b))
	{
		return true;
	}

	return false;
}

bool vector4_is_not_equal(const caffmath::vector4 a, const caffmath::vector4 b)
{
	return !vector4_is_equal(a, b);
}

bool vector4_components_are(const caffmath::vector4 a, const float x, const float y, const float z, const float w)
{
	if(caffmath::vector4_get_x(a) == x && caffmath::vector4_get_y(a) == y && caffmath::vector4_get_z(a) == z && caffmath::vector4_get_w(a) == w)
	{
		return true;
	}

	//std::cout << caffmath::vector4_get_x(a) << ", " << caffmath::vector4_get_y(a) << ", " << caffmath::vector4_get_z(a) << ", " << caffmath::vector4_get_w(a) << std::endl;

	return false;
}

bool vector4_components_are_near(const caffmath::vector4 a, const float x, const float y, const float z, const float w, const float error)
{
	const float x_result = caffmath::vector4_get_x(a) - x;
	const float y_result = caffmath::vector4_get_y(a) - y;
	const float z_result = caffmath::vector4_get_z(a) - z;
	const float w_result = caffmath::vector4_get_w(a) - w;

	if(std::abs(x_result) < error && std::abs(y_result) < error && std::abs(z_result) < error && std::abs(z_result) < error)
	{
		return true;
	}

	//std::cout << caffmath::vector4_get_x(a) << ", " << caffmath::vector4_get_y(a) << ", " << caffmath::vector4_get_z(a) << ", " << caffmath::vector4_get_w(a) << std::endl;

	return false;
}

void vector4_is_equal(const std::string &test, const caffmath::vector4 a, const float b)
{

}

void vector4_is_near(const std::string &test, const caffmath::vector4 a, const caffmath::vector4 b, const float error)
{

}


void assert_test(const std::string &test, const bool passed)
{
	if(!passed)
	{
		std::cout << test << " - failed!" << std::endl;
	}
}


int main()
{
	std::cout << "vector 4 tests - started" << std::endl;

	// Test Init
	{
		const caffmath::vector4 zero_vector4 = caffmath::vector4_init(0);
		assert_test("test vector4 is all zero", vector4_components_are(zero_vector4, 0,0,0,0));

		const caffmath::vector4 one_vector4 = caffmath::vector4_init(1);
		assert_test("test vector4 is all ones", vector4_components_are(one_vector4, 1,1,1,1));
		
		const caffmath::vector4 one_two_three_four_vector4 = caffmath::vector4_init(1.1f, 2.2f, 3.3f, 4.4f);
		assert_test("test components", vector4_components_are(one_two_three_four_vector4, 1.1f, 2.2f, 3.3f, 4.4f));
	}

	// Test Basic Arithmetic
	{
		const caffmath::vector4 one_vector4 = caffmath::vector4_init(1);
		const caffmath::vector4 one_two_three_four_vector4 = caffmath::vector4_init(1.1f, 2.2f, 3.3f, 4.4f);
		const float error = 0.0001f;

		const caffmath::vector4 add_result = caffmath::vector4_add(one_two_three_four_vector4, one_vector4);
		assert_test("test add", vector4_components_are_near(add_result, 2.1f, 3.2f, 4.3f, 5.4f, error));

		const caffmath::vector4 sub_result = caffmath::vector4_subtract(one_two_three_four_vector4, one_vector4);
		assert_test("test subtract", vector4_components_are_near(sub_result, 0.1f, 1.2f, 2.3f, 3.4f, error));

		const caffmath::vector4 mul_result = caffmath::vector4_multiply(one_two_three_four_vector4, one_vector4);
		assert_test("test multiply", vector4_components_are_near(mul_result, 1.1f, 2.2f, 3.3f, 4.4f, error));

		const caffmath::vector4 div_result = caffmath::vector4_divide(one_two_three_four_vector4, one_vector4);
		assert_test("test divide", vector4_components_are_near(div_result, 1.1f, 2.2f, 3.3f, 4.4f, error));
	}

	// Test Special Operations
	{
		const caffmath::vector4 one_two_three_four_vector4 = caffmath::vector4_init(1.1f, 2.2f, 3.3f, 4.4f);
		const float error = 0.0001f;

		
	}

	std::cout << "vector 4 tests - finished" << std::endl;

	return 0;
}