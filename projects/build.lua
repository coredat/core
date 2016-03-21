-- A solution contains projects, and defines the available configurations

function get_proj_root_dir()
  if(os.get() == "macosx") then
    return "$(SRCROOT)/../"
  else
    return "../"
  end
end

solution "Vortex Application"

   configurations {
     "Debug",
     "Release"
   }

   dofile("../projects/wired.lua")
   dofile("../projects/test.lua")
