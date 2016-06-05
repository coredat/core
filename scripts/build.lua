-- A solution contains projects, and defines the available configurations
solution "Vortex Application"

   configurations {
     "Debug",
     "Release"
   }

   dofile("../projects/wired.lua")
   dofile("../projects/test.lua")
