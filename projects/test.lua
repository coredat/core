#!lua

-- A solution contains projects, and defines the available configurations
solution "Wired Unit Test"

   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "UnitTest"

      kind "ConsoleApp"
      language "C++"
      files { "../**.hpp", "../**.cpp", "../**.h", "../**.c", "/Users/PhilCK/Developer/utils/*", "/Users/PhilCK/Developer/sdl_wrapper/*" }
      libdirs { "/usr/local/lib/", "../3rdparty/bullet/lib/osx/", "../3rdparty/enet/lib/osx/", }
      linkoptions { "-lsdl2", "-lsdl2_mixer", "-lSOIL", "-lsimple_renderer"}
      links {"enet", "OpenGL.framework", "CoreFoundation.framework"}
      includedirs {"../code/", "/usr/local/include/", "/usr/local/include/bullet/", "../3rdparty/sdl_mixer/include/", "../3rdparty/bullet/include/", "../3rdparty/enet/include" }

      buildoptions {"-std=c++14", "-stdlib=libc++"} --, "-framework OpenGL", "-framework CoreFoundation"}

      configuration "Debug"
         defines { "DEBUG", "UNIT_TEST" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }
