VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Axton/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Axton/vendor/glad/include"
IncludeDir["STB"] = "%{wks.location}/Axton/vendor/stb_image"
IncludeDir["GLM"] = "%{wks.location}/Axton/vendor/glm"
IncludeDir["LOG"] = "%{wks.location}/Axton/vendor/spdlog/include"
IncludeDir["IMGUI"] = "%{wks.location}/Axton/vendor/imgui"
IncludeDir["VULKAN"] = "%{VULKAN_SDK}/Include"
IncludeDir["NRD"] = "%{wks.location}/Axton/vendor/nrd/include"

LibraryDir = {}
LibraryDir["VULKAN_SDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["NRD_SDK"] = "%{wks.location}/Axton/vendor/nrd/Lib"

Library = {}
Library["VULKAN"] = "%{LibraryDir.VULKAN_SDK}/vulkan-1.lib"
Library["NRD"] = "%{LibraryDir.NRD_SDK}/%{cfg.buildcfg}/NRD.lib"