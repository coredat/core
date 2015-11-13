
CC       = "clang++"
FLAGS    = "-std=c++11 -Wall -Wpedantic"
EX_FLAGS = "-Wno-missing-braces -g" #Missing braces brings up false positives.
INC_DIRS = [
  "-I./",
  "-I./test",
  "-I./unit_test"
]


def build_compile_string(files, out, coverage)

  "#{CC} #{FLAGS} #{EX_FLAGS} #{if coverage then "-coverage" end} #{INC_DIRS.join(' ')} #{files.join(' ')} -o #{out}"
end



task :ci do |t, args|

  sh build_compile_string(["test/vec2_unit_test.cpp"], "vec2", true)
  sh "./vec2"

  sh build_compile_string(["test/vec3_unit_test.cpp"], "vec3", true)
  sh "./vec3"

  sh build_compile_string(["test/vec4_unit_test.cpp"], "vec4", true)
  sh "./vec4"

  sh build_compile_string(["test/mat4_unit_test.cpp"], "mat4", true)
  sh "./mat4"

end


task :scratch do |t, args|
  
  # Copy and paste current test you are working on.
  sh build_compile_string(["test/mat4_unit_test.cpp"], "scratch_test", false)
  sh "./scratch_test"

end

task :benchmarks do |t, args|
end