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
IncludeDir["GLAD"] = "Velvet/vendor/GLAD/include"
IncludeDir["ImGui"] = "Velvet/vendor/imgui"

include "Velvet/vendor/GLFW"
include "Velvet/vendor/GLAD"
include "Velvet/vendor/imgui"

project "Velvet"
	location "Velvet"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/%{prj.name}/bin/")

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"VL_PLATFORM_WINDOWS",
			"VL_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Window")
		}

	filter "configurations:Debug"
		defines "VL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "VL_DIST"
		runtime "Release"
		symbols "Off"
		optimize "On"

project "Window"
	location "Window"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/%{prj.name}/bin/")

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
		systemversion "latest"

		defines
		{
			"VL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "VL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "VL_DIST"
		runtime "Release"
		symbols "Off"
		optimize "On"
