#!lua

 project "UnitTest"

    kind "ConsoleApp"
    language "C++"

    files {
      "../**.hpp",
      "../**.cpp",
      "../**.h",
      "../**.c",
      "/Users/PhilCK/Developer/utils/*",
      "/Users/PhilCK/Developer/sdl_wrapper/*"
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
      "-lsimple_renderer"
    }

    links {
      "enet",
      "OpenGL.framework",
      "CoreFoundation.framework"
    }

    includedirs {
      get_proj_root_dir() .. "code/",
      "/usr/local/include/",
      "/usr/local/include/bullet/",
      get_proj_root_dir() .. "3rdparty/sdl_mixer/include/",
      get_proj_root_dir() .. "3rdparty/bullet/include/",
      get_proj_root_dir() .. "3rdparty/enet/include",
    }

    buildoptions {
      "-std=c++14",
      "-stdlib=libc++"
    } --, "-framework OpenGL", "-framework CoreFoundation"}

    configuration "Debug"
       defines { "DEBUG", "UNIT_TEST" }
       flags { "Symbols" }

    configuration "Release"
       defines { "NDEBUG" }
       flags { "Optimize" }
