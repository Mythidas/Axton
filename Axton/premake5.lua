include "dependencies.lua"

project "Axton"
    location "src"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "axpch.h"
    pchsource "src/axpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

    includedirs
    {
        "src",
        "%{IncludeDir.LOG}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.STB}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.VULKAN}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "%{Library.VULKAN}"
    }

    flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "AX_PLAT_WINDOWS",
        }

    filter "configurations:Debug"
        defines
        {
            "AX_DEBUG",
            "AX_ASSERTS_ENABLED"
        }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "AX_RELEASE"
        optimize "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "AX_DIST"
        optimize "On"
        runtime "Release"