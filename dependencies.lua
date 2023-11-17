VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Axton/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Axton/vendor/glad/include"
IncludeDir["STB"] = "%{wks.location}/Axton/vendor/stb_image"
IncludeDir["GLM"] = "%{wks.location}/Axton/vendor/glm"
IncludeDir["LOG"] = "%{wks.location}/Axton/vendor/spdlog/include"
IncludeDir["IMGUI"] = "%{wks.location}/Axton/vendor/imgui"
IncludeDir["NRD"] = "%{wks.location}/Axton/vendor/nrd/include"
IncludeDir["VULKAN"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["VULKAN_SDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["VULKAN"] = "%{LibraryDir.VULKAN_SDK}/vulkan-1.lib"