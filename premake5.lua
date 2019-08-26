workspace "Crow"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


	include "Crow/lib/glfw"


	project "Crow"
		kind "SharedLib"
		location "Crow"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}/")
		objdir ("bin/" .. outputdir .. "/%{prj.name}/intermediates/")


		files { 
			"%{prj.name}/src/**.h", 
			"%{prj.name}/src/**.cpp" 
		}

		includedirs { 
			"%{prj.name}/src",
			"%{prj.name}/lib/spdlog/include",
			"%{prj.name}/lib/glfw/include"
		}

		links {
			"GLFW",
			"opengl32.lib"
		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines {
				"CR_PLATFORM_WINDOWS",
				"CR_BUILD_DLL",
				"CR_x64"
			}

			postbuildcommands {
				("{COPY} ../bin/" .. outputdir .. "/Crow/Crow.dll ../bin/" .. outputdir .. "/GameProject")
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
			"Crow/lib/GLFW/include"
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