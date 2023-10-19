project "ShaderSystem"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
	entrypoint "mainCRTStartup"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    debugdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files
    { 
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
		"src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.vulkan}",
		"%{IncludeDir.shaderc_glslc}",
		"%{IncludeDir.shaderc_util}",
    }

    links
    {
		"opengl32.lib",
		"GLAD",
		"GLFW",
		"%{LibDir.vulkan_sdk}",
		"%{LibDir.vulkan_sdk_utils}",
		"%{LibDir.dxc}"
    }
	
	postbuildcommands
	{
	}

    filter "system:windows"
        systemversion "latest"
        disablewarnings { "5033", "4996" }

        defines
        {
            "SHADER_SYSTEM_PLATFORM_WINDOWS"
        }

	filter "system:linux"
		systemversion "latest"
		
		defines
		{
			"SHADER_SYSTEM_PLATFORM_LINUX"
		}

	filter "system:macos"
		systemversion "latest"
		
		defines
		{
			"SHADER_SYSTEM_PLATFORM_MACOS"
		}

    filter "configurations:Debug"
        defines "SHADER_SYSTEM_DEBUG"
        symbols "On"
		
		postbuildcommands
		{
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}
		
		links
		{
			"%{LibDir.shaderc_debug}",
			"%{LibDir.shaderc_util_debug}",
			
			"%{LibDir.SPIRV_Cross_Debug}",
			"%{LibDir.SPIRV_Cross_GLSL_Debug}",
			"%{LibDir.SPIRV_Tools_Debug}",
		}

    filter "configurations:Release"
        defines "SHADER_SYSTEM_RELEASE"
        optimize "On"

		postbuildcommands
		{
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_shared.dll" "%{cfg.targetdir}"'
		}
		
		links
		{
			"%{LibDir.shaderc_release}",
			"%{LibDir.shaderc_util_release}",
			
			"%{LibDir.SPIRV_Cross_Release}",
			"%{LibDir.SPIRV_Cross_GLSL_Release}",
			"%{LibDir.SPIRV_Tools_Release}",
		}
