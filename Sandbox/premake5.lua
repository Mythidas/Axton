project "Sandbox"
    location "Source"
    kind "ConsoleApp"
    language "C++"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs
    {
        "../Axton/src",
        "../Axton/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.STB}",
        "%{IncludeDir.GLM}"
    }

    links
    {
        "Axton"
    }

    filter "system:windows"
        cppdialect "C++latest"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "AX_PLAT_WINDOWS"
        }

    filter "configurations:Debug"
        defines "AX_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "AX_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "AX_DIST"
        optimize "On"