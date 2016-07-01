#!lua

core_project_data = {

  name     = "CoreEngine",
  kind     = "StaticLib",
  language = "C++",
  location = make.get_proj_root(),

  src_files = {
    make.get_proj_root() .. "../code/**.hpp",
    make.get_proj_root() .. "../code/**.inl",
    make.get_proj_root() .. "../code/**.cpp",
    make.get_proj_root() .. "../code/**.mm",
    make.get_proj_root() .. "../code/**.h",
    make.get_proj_root() .. "../code/**.c",
    make.get_proj_root() .. "../code/**.m",
    make.get_proj_root() .. "../code/**.md",
  },

  src_files_exclude_windows = {
    make.get_proj_root() .. "../code/external/utilities/utilities/directory_nix.cpp",
    make.get_proj_root() .. "../code/external/utilities/utilities/directory_mac.mm",
  },

  src_files_exclude_macosx = {
    make.get_proj_root() .. "../code/external/utilities/utilities/directory_win.cpp",
  },

  inc_dirs = {
    make.get_proj_root() .. "../code/",
    make.get_proj_root() .. "../code/external/math/",
    make.get_proj_root() .. "../code/external/graphics_api/",
    make.get_proj_root() .. "../code/external/utilities/",
    make.get_proj_root() .. "../3rdparty/sdl_mixer/include/",
    make.get_proj_root() .. "../3rdparty/sdl/include/",
    make.get_proj_root() .. "../3rdparty/glew/include/",
    make.get_proj_root() .. "../3rdparty/soil/include/",
  },

  inc_dirs_macosx = {
    "/usr/local/include/",
  },

  lib_dirs_windows = {
    make.get_proj_root() .. "../3rdparty/glew/lib/win32/",
    make.get_proj_root() .. "../3rdparty/soil/lib/win32/",
    make.get_proj_root() .. "../3rdparty/sdl/lib/win32/",
  },

  link_dependencies_macosx = {
    "OpenGL.framework",
    "CoreFoundation.framework",
    "Foundation.framework",
  },

  link_dependencies_windows = {
    "glew32.lib",
    "SDL2.lib",
    "SOIL.lib",
    "opengl32.lib",
    "shlwapi.lib",
  },

  linkoption_dependencies_macosx = {
    "-lsdl2",
    "-lsdl2_mixer",
    "-lSOIL",
  },
}
