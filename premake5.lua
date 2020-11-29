workspace "Hound"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "Hound/vendor/glfw/include"
includeDir["Glad"] = "Hound/vendor/glad/include"

include "Hound/vendor/glfw"
include "Hound/vendor/glad"

project "Hound"
	location "Hound"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("_bin_temp/" .. outputdir .. "/%{prj.name}")

    pchheader "hound/hnd_pch.h"
    pchsource "%{prj.name}/src/hound/hnd_pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}"
    }

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HND_PLATFORM_WINDOWS",
            "HND_BUILD_DLL",
			"HND_ENABLE_ASSERT",
			"GLFW_INCLUDE_NONE",
			"_GLFW_WIN32",
			"_GLFW_WGL",
			"_GLFW_USE_OPENGL"
		}

		--postbuildcommands
		--{
		--	("{COPY} %{cfg.buildtarget.relpath} ../_bin/" .. outputdir .. "/Sandbox")
		--}

	filter "configurations:Debug"
		defines "HND_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HND_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HND_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("_bin_temp/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hound/vendor/spdlog/include",
		"Hound/src"
	}

	links
	{
		"Hound"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HND_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HND_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HND_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HND_DIST"
		runtime "Release"
		optimize "On"