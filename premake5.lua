workspace "Velvet"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Window"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"] = "Velvet/vendor/spdlog/include"
IncludeDir["GLFW"] = "Velvet/vendor/GLFW/include"

include "Velvet/vendor/GLFW"

project "Velvet"
	location "Velvet"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.name}/bin/")

	pchheader "vlpch.h"
	pchsource "%{prj.name}/src/vlpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"VL_PLATFORM_WINDOWS",
			"VL_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Window")
		}

	filter "configurations:Debug"
		defines "VL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "VL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "VL_DIST"
		optimize "On"

project "Window"
	location "Window"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.name}/bin/")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Velvet/src",
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"Velvet"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"VL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "VL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "VL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "VL_DIST"
		optimize "On"
