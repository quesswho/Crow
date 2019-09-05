workspace "Crow"
	architecture "x64"
	startproject "GameProject"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	group "Dependencies"
		include "Crow/lib/glfw"
		include "Crow/lib/glad"

	group ""
	project "Crow"
		kind "StaticLib"
		location "Crow"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}/")
		objdir ("bin/" .. outputdir .. "/%{prj.name}/intermediates/")


		files { 
			"%{prj.name}/src/**.h", 
			"%{prj.name}/src/**.cpp" 
		}

		includedirs { 
			"%{prj.name}/src",
			"%{prj.name}/lib/spdlog/include",
			"%{prj.name}/lib/glfw/include",
			"%{prj.name}/lib/glad/include",
			"%{prj.name}/lib/glm"
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

		links {
			"GLFW",
			"glad",
			"opengl32.lib"
		}

		filter "system:windows"
			systemversion "latest"

			defines {
				"CR_PLATFORM_WINDOWS",
				"CR_x64",
				"GLFW_INCLUDE_NONE"
			}

		filter "configurations:Debug"
			defines "CR_DEBUG"
			symbols "On"
	
		filter "configurations:Release"
			defines "CR_RELEASE"
			optimize "On"

	project "GameProject"
		kind "ConsoleApp"
		location "GameProject"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}/")
		objdir ("bin/" .. outputdir .. "/%{prj.name}/intermediates")

		files { 
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp" 
		}

		includedirs { 
			"Crow/src",
			"Crow/lib/spdlog/include",
			"Crow/lib/glfw/include",
			"Crow/lib/glad/include",
			"Crow/lib/glm"
		}

		links { "Crow" }

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines {
				"CR_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "CR_DEBUG"
			symbols "On"
	
		filter "configurations:Release"
			defines "CR_RELEASE"
			optimize "On"