#!lua

 -- A project defines one build target
 project "CoreEngine"

    kind "StaticLib"
    language "C++"

    files {
      "../code/**.hpp",
      "../code/**.cpp",
      "../code/**.mm",
      "../code/**.h",
      "../code/**.c",
      "../code/**.m",
      "../code/**.md",
    }

    includedirs {
      get_proj_root_dir() .. "code/",
      get_proj_root_dir() .. "code/external/math/",
      get_proj_root_dir() .. "code/external/graphics_api/",
      get_proj_root_dir() .. "code/external/utilities/",
      "/usr/local/include/",
      get_proj_root_dir() .. "3rdparty/sdl_mixer/include/",
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
