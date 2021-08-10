workspace "Mandelbrot"
  architecture "x64"

  startproject "Mandelbrot"

  configurations {
    "debug",
    "release",
  }

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
indirs = {}

project "Mandelbrot"
  location "Mandelbrot"
  kind "ConsoleApp"
  language "C++"
  staticruntime "on"
  cppdialect "C++17"

  targetdir("bin/" ..outdir.. "/%{prj.name}")
  objdir("obj/" ..outdir.. "/%{prj.name}")

  files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/vendor/stb/**.h",
    "%{prj.name}/vendor/stb/**.cpp"
  }

  includedirs {
    "LambdaRenderer/include",
    "%{prj.name}/src",
    "%{prj.name}/vendor",
  }

  links {
    "LambdaRenderer",
    "GLFW",
    "glad",
    "spdlog"
  }
  
  filter "system:macosx"
    system "macosx"

    defines {
      "GL_SILENCE_DEPRECATION"
    }

    libdirs {
      -- "%{wks.location}/bin/" ..outdir.. "/%{prj.name}",
      "%{wks.location}/LambdaRenderer/macos",
    }

    links {
      "Cocoa.framework",
      "IOKit.framework",
      "CoreVideo.framework",
      "OpenGL.framework",
    }

    -- linkoptions {
    --   "-Wl,-rpath,./",
    -- }

    -- prelinkcommands {
    --   "{COPY} %{wks.location}/LambdaRenderer/macos/* %{wks.location}/bin/" ..outdir.. "/%{prj.name}"
    -- }
  
  filter "system:linux"
    system "linux"

    links {
      "pthread",
      "dl",
      "m",
      "GL",
      "X11"
    }
  
  filter "system:Windows"
    system "Windows"

    libdirs {
      "%{wks.location}/LambdaRenderer/Windows",
    }

    links {
      "OpenGL32.lib",
    }

  filter "configurations:debug"
    symbols "on"

  filter "configurations:release"
    optimize "on"
