

task :ci do |t, args|

  sh "clang++ -I./test -I./unit_test/ -I./ -std=c++11 test/vec2_unit_test.cpp -o vec2"
  sh "./vec2"

  sh "clang++ -I./test -I./unit_test/ -I./ -std=c++11 test/vec3_unit_test.cpp -o vec3"
  sh "./vec3"

  sh "clang++ -I./test -I./unit_test/ -I./ -std=c++11 test/vec4_unit_test.cpp -o vec4"
  sh "./vec4"  

end