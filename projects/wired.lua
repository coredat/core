#!lua

 -- A project defines one build target
 project "Core Engine"

    kind "StaticLib"
    language "C++"

    files {
      "../code/**.hpp",
      "../code/**.cpp",
      "../code/**.mm",
      "../code/**.h",
      "../code/**.c",
      "../code/**.m",
      "/Users/PhilCK/Developer/sdl_wrapper/*",
    }

    includedirs {
      get_proj_root_dir() .. "code/",
      get_proj_root_dir() .. "code/external/math/",
      get_proj_root_dir() .. "code/external/graphics_api/",
      get_proj_root_dir() .. "code/external/utilities/",
      "/usr/local/include/",
      "/usr/local/include/bullet/",
      get_proj_root_dir() .. "3rdparty/sdl_mixer/include/",
      get_proj_root_dir() .. "3rdparty/bullet/include/",
      get_proj_root_dir() .. "3rdparty/enet/include",
    }

    buildoptions {
      "-std=c++14",
      "-stdlib=libc++",
    } --, "-framework OpenGL", "-framework CoreFoundation"}

    configuration "Debug"
       defines { "DEBUG" }
       flags { "Symbols" }

    configuration "Release"
       defines { "NDEBUG" }
       flags { "Optimize" }
