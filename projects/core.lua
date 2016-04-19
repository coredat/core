#!lua

core_project_data = {
  name     = "CoreEngine",
  kind     = "StaticLib",
  language = "C++",
  location = make.get_proj_root(),

  src_files = {
    make.get_proj_root() .. "../code/**.hpp",
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

  src_files_exclude_mac = {
    make.get_proj_root() .. "../code/external/utilities/utilities/directory_win.cpp",
  },

  include_directories = {
    make.get_proj_root() .. "../code/",
    make.get_proj_root() .. "../code/external/math/",
    make.get_proj_root() .. "../code/external/graphics_api/",
    make.get_proj_root() .. "../code/external/utilities/",
    make.get_proj_root() .. "../3rdparty/sdl_mixer/include/",
    make.get_proj_root() .. "../3rdparty/sdl/include/",
    make.get_proj_root() .. "../3rdparty/glew/include/",
    make.get_proj_root() .. "../3rdparty/soil/include/",
  },

  include_directories_mac = {
    "/usr/local/include/",
  },

  link_dependencies_mac = {
    "OpenGL.framework",
    "CoreFoundation.framework",
    "Foundation.framework",
    "CoreEngine",
  },
}
