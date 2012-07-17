APPLICATION_NAME="game"

project(APPLICATION_NAME)
  kind "WindowedApp"
  language "C++"

  configuration "linux or macosx"
    targetdir "../.."

  if os.get() ~= "windows" then
    configuration { "Debug*", "x32" }
      targetsuffix "_d32"
    configuration { "Release*", "x32" }
      targetsuffix "_x32"
    configuration { "Debug*", "x64" }
      targetsuffix "_d64"
    configuration { "Release*", "x64" }
      targetsuffix "_x64"
    configuration { "Debug*", "univ" }
      targetsuffix "_d"
  end

--   os.copydir("assets_shared", "assets")
--   os.copydir("assets_pc", "assets")

  configuration { "linux", "Debug*", "x32" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/d32'", "-Wl,-rpath-link,../../lib/d32" }
  configuration { "linux", "Release*", "x32" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/x32'", "-Wl,-rpath-link,../../lib/x32" }
  configuration { "linux", "Debug*", "x64" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/d64'", "-Wl,-rpath-link,../../lib/d64" }
  configuration { "linux", "Release*", "x64" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/x64'", "-Wl,-rpath-link,../../lib/x64" }

  configuration "windows or linux"
    links { "local_SDLmain" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "linux"
    buildoptions { "-std=c++0x" }
  configuration "macosx"
    files { "../external/sdl/SDLmain/*.h",
            "../external/sdl/SDLmain/*.m" }
    links { "Cocoa.framework" }

  configuration "*"
    includedirs { ".", "../external/zenilib", "../external/zenilib/zeni_rest", "../external/zenilib/zeni_graphics", "../external/zenilib/zeni_net", "../external/zenilib/zeni_core", "../external/zenilib/zeni_audio", "../external/libvorbis/include", "../external/libogg/include", "../external/zenilib/zeni", "../external/sdl_net", "../external/sdl", "../external/tinyxml" }

--     pchheader "jni/external/zenilib/zenilib.h"
--     pchsource "jni/application/bootstrap.cpp"

    files { "**.h", "**.cpp" }
    links { "zeni_rest", "zeni_graphics", "zeni_net", "zeni_core", "zeni_audio", "zeni", "local_SDL" }