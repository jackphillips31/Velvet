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

project "Velvet"
	location "Velvet"
	kind "SharedLib"
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
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.19041.0"

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
		"Velvet/vendor/spdlog/include",
		"Velvet/src"
	}

	links
	{
		"Velvet"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.19041.0"

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