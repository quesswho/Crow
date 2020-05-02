workspace "Crow"
	architecture "x64"
	startproject "GameProject"
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	configurations
	{
		"OGL_Debug",
		"OGL_Release",
		"DX11_Debug",
		"DX11_Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	group "Dependencies"
		include "Crow/lib/glfw"
		include "Crow/lib/glad"
		include "Crow/lib/freetype2"

	group ""
	project "Crow"
		kind "StaticLib"
		location "Crow"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		configuration "OGL_Debug"
			targetdir ("bin/OGL/Debug-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/OGL/Debug-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "OGL_Release"
			targetdir ("bin/OGL/Release-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/OGL/Release-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "DX11_Debug"
			targetdir ("bin/DX11/Debug-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/DX11/Debug-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "DX11_Release"
			targetdir ("bin/DX11/Release-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/DX11/Release-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "*"

		files { 
			"%{prj.name}/src/**.h", 
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/src/**.glsl",
			"%{prj.name}/lib/glm/glm/**.hpp",
			"%{prj.name}/lib/glm/glm/**.inl",
			"%{prj.name}/lib/stb_image/**.*",
			"%{prj.name}/lib/d3dx12/d3dx12.h",
		}

		includedirs { 
			"%{prj.name}/src"
		}
		
		sysincludedirs { 
			"%{prj.name}/lib/spdlog/include",
			"%{prj.name}/lib/glfw/include",
			"%{prj.name}/lib/glad/include",
			"%{prj.name}/lib/freetype2/include",
			"%{prj.name}/lib/glm",
			"%{prj.name}/lib/stb_image/",
			"%{prj.name}/lib/d3dx12/"
			
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

		links {
			"GLFW",
			"glad",
			"freetype2",
			"opengl32.lib"
		}
	
		filter "system:windows"
			systemversion "latest"

			defines {
				"CR_PLATFORM_WINDOWS",
				"CR_x64",
				"GLFW_INCLUDE_NONE"
			}

		filter "configurations:OGL_Debug"
			defines {
				"CR_DEBUG",
				"CROW_OGL"
			}
			symbols "On"
	
		filter "configurations:OGL_Release"
			defines {
				"CR_RELEASE",
				"CROW_OGL"
			}
			optimize "On"
			
		filter "configurations:DX11_Debug"
			defines {
				"CR_DEBUG",
				"CROW_DX11"
			}
			symbols "On"
			
		filter "configurations:DX11_Release"
			defines {
				"CR_RELEASE",
				"CROW_DX11"
			}
			optimize "On"

	project "GameProject"
		kind "ConsoleApp"
		location "GameProject"
		language "C++"
		
		configuration "OGL_Debug"
			targetdir ("bin/OGL/Debug-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/OGL/Debug-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "OGL_Release"
			targetdir ("bin/OGL/Release-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/OGL/Release-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "DX11_Debug"
			targetdir ("bin/DX11/Debug-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/DX11/Debug-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "DX11_Release"
			targetdir ("bin/DX11/Release-%{cfg.architecture}/%{prj.name}/")
			objdir ("bin/DX11/Release-%{cfg.architecture}/%{prj.name}/intermediates/")
		configuration "*"
		
		files { 
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp" 
		}

		includedirs { 
			"Crow/src"
		}
		
		sysincludedirs {
			"Crow/lib/spdlog/include",
			"Crow/lib/glfw/include",
			"Crow/lib/glad/include",
			"Crow/lib/freetype2/include",
			"Crow/lib/glm",
			"Crow/lib/stb_image/",
			"Crow/lib/d3dx12/"
		}

		links { 
			"Crow"
		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines {
				"CR_PLATFORM_WINDOWS",
				"FT2_BUILD_LIBRARY"
			}

		filter "configurations:OGL_Debug"
			defines {
				"CR_DEBUG",
				"CROW_OGL"
			}
			symbols "On"
	
		filter "configurations:OGL_Release"
			defines {
				"CR_RELEASE",
				"CROW_OGL"
			}
			optimize "On"
			
		filter "configurations:DX11_Debug"
			defines {
				"CR_DEBUG",
				"CROW_DX11"
			}
			symbols "On"
			
		filter "configurations:DX11_Release"
			defines {
				"CR_RELEASE",
				"CROW_DX11"
			}
			optimize "On"