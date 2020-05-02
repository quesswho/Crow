project "glad"
	kind "StaticLib"
	language "C"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}/intermediates")
	
	files
	{
		"include/**.h",
		"src/*.c"
	}
	
	includedirs 
	{ 
		"include"
	}
	
	filter "system:windows"
			staticruntime "On"
	
	filter "configurations:OGL_Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:OGL_Release"
		runtime "Release"
		optimize "on"
	filter "configurations:DX11_Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:DX11_Release"
		runtime "Release"
		optimize "on"
