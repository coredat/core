
#define CAFF_MATH_SIMD 1

#include <iostream>
#include <ctime>
#include <chrono>
#include "../math/vector/vector.hpp"


const std::size_t iterations = 100000;




int main()
{
	#ifdef CAFF_MATH_SIMD

	std::cout << "Benchmark Tests [SIMD ON]" << std::endl;

	#else

	std::cout << "Benchmark Tests [SIMD OFF]" << std::endl;

	#endif

	{
		const std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

		for(std::size_t i = 0; i < iterations; ++i)
		{
			volatile int foo = 0;
			caffmath::vector4_init(1);
		}

		const std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << " - Init with single value" << std::endl;
	}


	{
		const caffmath::vector4 a = caffmath::vector4_init(1.3f);
		const caffmath::vector4 b = caffmath::vector4_init(4.4f);

		const std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

		for(std::size_t i = 0; i < iterations; ++i)
		{
			volatile int foo = 0;

			const caffmath::vector4 c = caffmath::vector4_add(a, b);
			const caffmath::vector4 d = caffmath::vector4_add(a, c);
		}

		const std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << " - Addition" << std::endl;
	}


	{
		const caffmath::vector4 a = caffmath::vector4_init(1.3f);
		const caffmath::vector4 b = caffmath::vector4_init(4.4f);

		const std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

		for(std::size_t i = 0; i < iterations; ++i)
		{
			volatile int foo = 0;

			const caffmath::vector4 c = caffmath::vector4_divide(a, b);
			const caffmath::vector4 d = caffmath::vector4_divide(a, c);
		}

		const std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << " - Divide" << std::endl;
	}	


	{
		const caffmath::vector4 a = caffmath::vector4_init(1.3f);

		const std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

		for(std::size_t i = 0; i < iterations; ++i)
		{
			volatile int foo = 0;
			
			const caffmath::vector4 b = caffmath::vector4_scale(a, 12.f);
			const caffmath::vector4 c = caffmath::vector4_scale(b, 4.4f);
		}

		const std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << " - Scale" << std::endl;
	}


	return 0;
}