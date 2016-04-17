#!lua

 -- A project defines one build target
 project "CoreEngine"
    location("./")

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
      get_solution_root_dir() .. "code/",
      get_solution_root_dir() .. "code/external/math/",
      get_solution_root_dir() .. "code/external/graphics_api/",
      get_solution_root_dir() .. "code/external/utilities/",
      "/usr/local/include/",
      get_solution_root_dir() .. "3rdparty/sdl_mixer/include/",
      get_solution_root_dir() .. "3rdparty/SDL/include/",
      get_solution_root_dir() .. "3rdparty/GLEW/include/",
      get_solution_root_dir() .. "3rdparty/SOIL/include/",
    }

    buildoptions {
      "-std=c++14", -- Clang/GCC only?
      "-stdlib=libc++", -- Clang/GCC only?
    } --, "-framework OpenGL", "-framework CoreFoundation"}

    configuration "Debug"
       defines { "DEBUG" }
       flags { "Symbols", "Unicode" }

    configuration "Release"
       defines { "NDEBUG" }
       flags { "Optimize", "Unicode" }
