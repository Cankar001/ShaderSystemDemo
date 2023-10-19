VULKAN_SDK = os.getenv("VULKAN_SDK")
PROJECT_DIR = 'ShaderSystem'

IncludeDir = {}
IncludeDir["glfw"]                   = "%{wks.location}/%{PROJECT_DIR}/vendor/glfw/include"
IncludeDir["GLAD"]                   = "%{wks.location}/%{PROJECT_DIR}/vendor/GLAD/include"
IncludeDir["glm"]                    = "%{wks.location}/%{PROJECT_DIR}/vendor/glm"
IncludeDir["spdlog"]                 = "%{wks.location}/%{PROJECT_DIR}/vendor/spdlog/include"
IncludeDir["stb_image"]              = "%{wks.location}/%{PROJECT_DIR}/vendor/stb_image"
IncludeDir["vulkan"]     			 = "%{VULKAN_SDK}/Include"
IncludeDir["vulkan_local"] 			 = "%{wks.location}/%{PROJECT_DIR}/vendor/VulkanSDK/Include"
IncludeDir["vulkan_memory_alloc"] 	 = "%{wks.location}/%{PROJECT_DIR}/vendor/VulkanMemoryAllocator"
IncludeDir["SPIRV_Cross"] 	 	 	 = "%{wks.location}/%{PROJECT_DIR}/vendor/SPIRV-Cross"
IncludeDir["shaderc_glslc"] 	 	 = "%{wks.location}/%{PROJECT_DIR}/vendor/shaderc/glslc"
IncludeDir["shaderc_util"] 	 	 	 = "%{wks.location}/%{PROJECT_DIR}/vendor/shaderc/libshaderc_util/include"

LibDir = {}
LibDir["vulkan_sdk_folder"]  		 = "%{VULKAN_SDK}/Lib"
LibDir["vulkan_sdk"]   		 		 = "%{LibDir.vulkan_sdk_folder}/vulkan-1.lib"
LibDir["vulkan_sdk_utils"]  		 = "%{LibDir.vulkan_sdk_folder}/VKLayer_utils.lib"


LibDir["SPIRV_Tools_Debug"] 		 = "%{LibDir.vulkan_sdk_folder}/SPIRV-Toolsd.lib"
LibDir["SPIRV_Tools_Release"] 		 = "%{LibDir.vulkan_sdk_folder}/SPIRV-Tools.lib"

LibDir["SPIRV_Cross_Debug"]          = "%{wks.location}/%{PROJECT_DIR}/vendor/SPIRV-Cross/lib/spirv-cross-cored.lib"
LibDir["SPIRV_Cross_Release"]        = "%{wks.location}/%{PROJECT_DIR}/vendor/SPIRV-Cross/lib/spirv-cross-core.lib"
LibDir["SPIRV_Cross_GLSL_Debug"]     = "%{wks.location}/%{PROJECT_DIR}/vendor/SPIRV-Cross/lib/spirv-cross-glsld.lib"
LibDir["SPIRV_Cross_GLSL_Release"]   = "%{wks.location}/%{PROJECT_DIR}/vendor/SPIRV-Cross/lib/spirv-cross-glsl.lib"

LibDir["shaderc_debug"]              = "%{LibDir.vulkan_sdk_folder}/shaderc_sharedd.lib"
LibDir["shaderc_util_debug"]         = "%{LibDir.vulkan_sdk_folder}/shaderc_utild.lib"

LibDir["shaderc_release"]            = "%{LibDir.vulkan_sdk_folder}/shaderc_shared.lib"
LibDir["shaderc_util_release"]       = "%{LibDir.vulkan_sdk_folder}/shaderc_util.lib"

LibDir["dxc"]    					 = "%{LibDir.vulkan_sdk_folder}/dxcompiler.lib"
