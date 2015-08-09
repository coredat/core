

task :ci do |t, args|

  sh "clang++ -I./test -I./unit_test/ -I./ -std=c++11 test/vec2_unit_test.cpp -o vec2"
  sh "./vec2"

  sh "clang++ -I./test -I./unit_test/ -I./ -std=c++11 test/vec3_unit_test.cpp -o vec3"
  sh "./vec3"

end