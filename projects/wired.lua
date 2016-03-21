#!lua

 -- A project defines one build target
 project "Wired"

    kind "WindowedApp"
    language "C++"

    files {
      "../code/**.hpp",
      "../code/**.cpp",
      "../code/**.h",
      "../code/**.c",
      "/Users/PhilCK/Developer/sdl_wrapper/*",
    }

    libdirs {
      "/usr/local/lib/",
      "../3rdparty/bullet/lib/osx/",
      "../3rdparty/enet/lib/osx/",
    }

    linkoptions {
      "-lsdl2",
      "-lsdl2_mixer",
      "-lSOIL",
      "-lsimple_renderer",
      "-lbulletdynamics",
      "-lbulletcollision",
      "-llinearmath",
    }

    links {
      "enet",
      "OpenGL.framework",
      "CoreFoundation.framework",
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
