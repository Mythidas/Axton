include "dependencies.lua"

workspace "Axton"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Axton/vendor/GLFW"
    include "Axton/vendor/glad"
    include "Axton/vendor/imgui"
group ""

group "Core"
    include "Axton"
group ""

group "Misc"
    include "Sandbox"
group ""