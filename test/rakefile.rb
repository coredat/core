

TEST_NAMES = {
	:vec2 => "vec2_test.cpp",
	:vec3 => "vec3_test.cpp",
	:vec4 => "vec4_test.cpp",
	:mat4 => "mat4_test.cpp"
}

desc "Run a test 'rake run_test[test_name]'"
task :run_test, [:test_name] do |t, args|

	test_name = TEST_NAMES[args.test_name.to_sym]

	sh "clang++ #{test_name} -std=c++11 -o test && ./test"

end
