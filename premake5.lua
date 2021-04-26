workspace "Pixel"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Pixel/vendor/GLFW/include"
IncludeDir["GLAD"] = "Pixel/vendor/GLAD/include" 
IncludeDir["stb_image"] = "Pixel/vendor/stb_image"
IncludeDir["glm"] = "Pixel/vendor/glm"
IncludeDir["assimp"] = "Pixel/vendor/assimp-lib/include"
IncludeDir["imgui"] = "Pixel/vendor/imgui"
IncludeDir["lua"] = "Pixel/vendor/lua/src"

include "Pixel/vendor/GLFW"
include "Pixel/vendor/GLAD"
include "Pixel/vendor/imgui"
include "Pixel/vendor/lua"

project "Pixel"
	location "Pixel"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pixelpch.h"
	pchsource "Pixel/src/Pixel/pixelpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Pixel/src/Pixel",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.lua}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"STB_IMAGE_IMPLEMENTATION"
	}

	links 
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"ImGui",
		"assimp-vc142-mtd.lib",
		"Lua"
	}

	libdirs 
	{
		"Pixel/vendor/assimp-lib/lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PIXEL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PIXEL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PIXEL_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pixel/src/Pixel",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.lua}"
	}

	links
	{
		"Pixel"
	}

	filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		defines "PIXEL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PIXEL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PIXEL_DIST"
		runtime "Release"
		optimize "on"