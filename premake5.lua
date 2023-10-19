include "./Premake/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

workspace "ShaderSystem"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "ShaderSystem"

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	group "Dependencies"
		include "ShaderSystem/vendor/GLAD"
		include "ShaderSystem/vendor/glfw"
		include "ShaderSystem/vendor/glm"
		include "ShaderSystem/vendor/spdlog"
		include "ShaderSystem/vendor/stb_image"
		include "ShaderSystem/vendor/shaderc"
	group ""
	
	include "ShaderSystem"
