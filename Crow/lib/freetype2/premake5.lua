project "freetype2"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}/Intermediates")

	files
	{	
		"src/autofit/autofit.c",
        "src/base/ftbase.c",
        "src/base/ftbbox.c",
        "src/base/ftbdf.c",
        "src/base/ftbitmap.c",
        "src/base/ftcid.c",
        "src/base/ftfstype.c",
        "src/base/ftgasp.c",
        "src/base/ftglyph.c",
        "src/base/ftgxval.c",
        "src/base/ftinit.c",
        "src/base/ftmm.c",
        "src/base/ftotval.c",
        "src/base/ftpatent.c",
        "src/base/ftpfr.c",
        "src/base/ftstroke.c",
        "src/base/ftsynth.c",
        "src/base/ftsystem.c",
        "src/base/fttype1.c",
        "src/base/ftwinfnt.c",
        "src/bdf/bdf.c",
        "src/cache/ftcache.c",
        "src/cff/cff.c",
        "src/cid/type1cid.c",
        "src/gzip/ftgzip.c",
        "src/lzw/ftlzw.c",
        "src/pcf/pcf.c",
        "src/pfr/pfr.c",
        "src/psaux/psaux.c",
        "src/pshinter/pshinter.c",
        "src/psnames/psmodule.c",
        "src/raster/raster.c",
        "src/sfnt/sfnt.c",
        "src/smooth/smooth.c",
        "src/truetype/truetype.c",
        "src/type1/type1.c",
        "src/type42/type42.c",
        "src/winfonts/winfnt.c",
		"src/ftdebug.c",
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
		defines { "WIN32", "_DEBUG", "_LIB", "_CRT_SECURE_NO_WARNINGS", "FT_DEBUG_LEVEL_ERROR", "FT_DEBUG_LEVEL_TRACE", "FT2_BUILD_LIBRARY" }

	filter "configurations:OGL_Release"
		runtime "Release"
		optimize "on"
		defines { "WIN32", "_DEBUG", "_LIB", "_CRT_SECURE_NO_WARNINGS", "FT_DEBUG_LEVEL_ERROR", "FT_DEBUG_LEVEL_TRACE", "FT2_BUILD_LIBRARY" }
	filter "configurations:DX11_Debug"
		runtime "Debug"
		symbols "on"
		defines { "WIN32", "_DEBUG", "_LIB", "_CRT_SECURE_NO_WARNINGS", "FT_DEBUG_LEVEL_ERROR", "FT_DEBUG_LEVEL_TRACE", "FT2_BUILD_LIBRARY" }

	filter "configurations:DX11_Release"
		runtime "Release"
		optimize "on"
		defines { "WIN32", "_DEBUG", "_LIB", "_CRT_SECURE_NO_WARNINGS", "FT_DEBUG_LEVEL_ERROR", "FT_DEBUG_LEVEL_TRACE", "FT2_BUILD_LIBRARY" }
