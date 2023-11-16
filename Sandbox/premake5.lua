project "Sandbox"
    location "Source"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/Axton/src",
        "%{wks.location}/Axton/vendor",
        "%{IncludeDir.LOG}",
        "%{IncludeDir.GLM}",
    }

    links
    {
        "Axton"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "AX_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "AX_RELEASE"
        optimize "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "AX_DIST"
        optimize "On"
        runtime "Release"