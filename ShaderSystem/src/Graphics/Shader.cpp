#include "Shader.h"

#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Core/Hash.h"
#include "Core/FileSystem.h"

#include "Renderer/Renderer.h"

#include "GLSLIncluder.h"
#include "ShaderCache.h"

#include <regex>

#include <shaderc/shaderc.hpp>
#include <libshaderc_util/file_finder.h>

#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_msl.hpp>

namespace ShaderSystem
{
	namespace utils
	{
		static std::filesystem::path GetCacheDirectory()
		{
			std::string renderingAPIStr = Renderer::GetCurrentRenderingAPI()->ToString();
			std::filesystem::path p = "assets/cache/shaders/" + renderingAPIStr + "/";

			std::error_code err;
			if (!std::filesystem::create_directories(p, err))
			{
				if (std::filesystem::exists(p))
				{
					// NOTE: directory already exists.
					err.clear();
				}
			}

			if (err)
			{
				SHADER_SYSTEM_ERROR("{0}", err.message().c_str());
				err.clear();
			}

			return p;
		}

		static ShaderLanguage ShaderLanguageFromExtension(const std::string& inFileExtension)
		{
			if (inFileExtension == ".glsl")
				return ShaderLanguage::GLSL;

			if (inFileExtension == ".hlsl")
				return ShaderLanguage::HLSL;

			return ShaderLanguage::None;
		}

		static std::string ShaderDomainCachedFileExtension(ShaderDomain inDomain)
		{
			switch (inDomain)
			{
			case ShaderSystem::ShaderDomain::Vertex:
				return ".cached_opengl.vert";
			case ShaderSystem::ShaderDomain::Fragment:
				return ".cached_opengl.frag";
			case ShaderSystem::ShaderDomain::Compute:
				return ".cached_opengl.comp";
			case ShaderSystem::ShaderDomain::Geometry:
				return ".cached_opengl.geo";
			case ShaderSystem::ShaderDomain::TessControl:
				return ".cached_opengl.tessconn";
			case ShaderSystem::ShaderDomain::TessEvalulation:
				return ".cached_opengl.tesseval";
			}

			SHADER_SYSTEM_ASSERT(false);
			return "";
		}

		static std::string ShaderDomainToString(ShaderDomain inDomain)
		{
			switch (inDomain)
			{
			case ShaderSystem::ShaderDomain::Vertex:
				return "vertex";
			case ShaderSystem::ShaderDomain::Fragment:
				return "fragment";
			case ShaderSystem::ShaderDomain::Compute:
				return "compute";
			case ShaderSystem::ShaderDomain::Geometry:
				return "geometry";
			case ShaderSystem::ShaderDomain::TessControl:
				return "tess eval";
			case ShaderSystem::ShaderDomain::TessEvalulation:
				return "tess control";

			case ShaderSystem::ShaderDomain::None:
			default:
				return "undefined";
			}
		}

		static std::string ShaderDomainToMacro(ShaderDomain inDomain)
		{
			switch (inDomain)
			{
			case ShaderDomain::Vertex:          return "__VERTEX_DOMAIN__";
			case ShaderDomain::Fragment:        return "__FRAGMENT_DOMAIN__";
			case ShaderDomain::Compute:         return "__COMPUTE_DOMAIN__";
			case ShaderDomain::Geometry:		return "__GEOMETRY_DOMAIN__";
			case ShaderDomain::TessControl:		return "__TESS_CONTROL_DOMAIN__";
			case ShaderDomain::TessEvalulation:	return "__TESS_EVAL_DOMAIN__";
			}

			SHADER_SYSTEM_ASSERT(false);
			return "";
		}

		static shaderc_shader_kind ShaderStageToShaderC(ShaderDomain inDomain)
		{
			switch (inDomain)
			{
			case ShaderDomain::Vertex:			return shaderc_vertex_shader;
			case ShaderDomain::Fragment:		return shaderc_fragment_shader;
			case ShaderDomain::Compute:			return shaderc_compute_shader;
			case ShaderDomain::Geometry:		return shaderc_geometry_shader;
			case ShaderDomain::TessControl:		return shaderc_tess_control_shader;
			case ShaderDomain::TessEvalulation:	return shaderc_tess_evaluation_shader;
			}

			SHADER_SYSTEM_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
		{
			switch (type.basetype)
			{
			case spirv_cross::SPIRType::Struct: return ShaderUniformType::Struct;
			case spirv_cross::SPIRType::Boolean: return ShaderUniformType::Bool;
			case spirv_cross::SPIRType::UInt: return ShaderUniformType::Uint;
			case spirv_cross::SPIRType::Int:
			{
				if (type.vecsize == 1)
					return ShaderUniformType::Int;

				if (type.vecsize == 2)
					return ShaderUniformType::IVec2;

				if (type.vecsize == 3)
					return ShaderUniformType::IVec3;

				if (type.vecsize == 4)
					return ShaderUniformType::IVec4;

				break;
			}

			case spirv_cross::SPIRType::Float:
			{
				if (type.columns == 2)
					return ShaderUniformType::Mat2;

				if (type.columns == 3)
					return ShaderUniformType::Mat3;

				if (type.columns == 4)
					return ShaderUniformType::Mat4;

				if (type.vecsize == 1)
					return ShaderUniformType::Float;

				if (type.vecsize == 2)
					return ShaderUniformType::Vec2;

				if (type.vecsize == 3)
					return ShaderUniformType::Vec3;

				if (type.vecsize == 4)
					return ShaderUniformType::Vec4;

				break;
			}
			}

			SHADER_SYSTEM_ASSERT(false);
			return ShaderUniformType::None;
		}
	}

	static std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderUniformBuffer*>> sUniformBuffers;
	static std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderStorageBuffer*>> sStorageBuffers;

	Shader::Shader(const std::filesystem::path& inFilePath, const BufferLayout &inLayout, bool inForceCompile)
		: mFilePath(inFilePath)
	{
		mGpuShader = GPUShader::Create(inLayout);
		mName = mFilePath.filename().string();
		if (mName.find('.') != std::string::npos)
		{
			mName = mName.substr(0, mName.find('.'));
		}

		mLanguage = utils::ShaderLanguageFromExtension(mFilePath.extension().string());

		if (FileSystem::FileExists(mFilePath))
		{
			std::string source = FileSystem::ReadTextFile(mFilePath);
			Load(source, inForceCompile);
		}
		else
		{
			SHADER_SYSTEM_FATAL("Could not find shader '{0}'.", mFilePath.string().c_str());
		}
	}

	Shader::Shader(const std::string& inSource, const std::string& inName, const BufferLayout &inLayout, ShaderLanguage inLanguage)
		: mName(inName), mLanguage(inLanguage), mFilePath("<unknown>")
	{
		mGpuShader = GPUShader::Create(inLayout);
		Load(inSource, true);
	}

	Shader::~Shader()
	{
		Release();
	}
	
	void Shader::Reload(bool inForceCompile)
	{
		if (!mFilePath.empty())
		{
			if (FileSystem::FileExists(mFilePath))
			{
				// Release the current resources
				Release();
			
				// do the actual reload process
				std::string source = FileSystem::ReadTextFile(mFilePath);
				Load(source, inForceCompile);
				
				// Notify the system that this has been reloaded.
				Renderer::OnShaderReloaded(GetHash());
				for (ShaderReloadedCallback callback : mReloadedCallbacks)
					callback();
			}
			else
			{
				SHADER_SYSTEM_ERROR("Reload failed: shader file {0} does not exist anymore.", mFilePath.string().c_str());
			}
		}
		else
		{
			// TODO: make a setter that lets set the shader source and then change this to also reload the new source.
			SHADER_SYSTEM_INFO("Reloading from string not supported yet.");
		}
	}
	
	void Shader::Release()
	{
		mLoaded = false;
		mGpuShader->ReleaseAllDomains();


	}
	
	uint64_t Shader::GetHash() const
	{
		return HashString(mFilePath.string());
	}
	
	void Shader::Bind() const
	{
		mGpuShader->Bind();
	}
	
	void Shader::Unbind()
	{
		mGpuShader->Unbind();
	}

	uint32_t Shader::GetRendererID()
	{
		return mGpuShader->GetRendererID();
	}
	
	void Shader::AddShaderReloadedCallback(const ShaderReloadedCallback& inCallback)
	{
		mReloadedCallbacks.push_back(inCallback);
	}
	
	void Shader::SetMacro(const std::string& inName, const std::string& inValue)
	{
		mMacros[inName] = inValue;
	}
	
	Ref<Shader> Shader::LoadFromFile(const std::filesystem::path& inFilePath, const BufferLayout &inLayout, bool inForceCompile)
	{
		return MakeRef<Shader>(inFilePath, inLayout, inForceCompile);
	}
	
	Ref<Shader> Shader::LoadFromString(const std::string& inSource, const std::string& inName, const BufferLayout &inLayout, ShaderLanguage inLanguage)
	{
		return MakeRef<Shader>(inSource, inName, inLayout, inLanguage);
	}

	void Shader::Load(const std::string& inSource, bool inForceCompile)
	{
		SHADER_SYSTEM_TRACE("Loading shader '{0}'...", mName.c_str());

		mShaderSources = PreProcess(inSource);

		bool shaderCacheHasChanged = ShaderCache::HasChanged(mFilePath, inSource);
		std::unordered_map<ShaderDomain, std::vector<uint32_t>> shaderData;

		// TODO: make the content general.
		CompileOrGetShaderBinary(shaderData, inForceCompile || shaderCacheHasChanged);

		LoadAndCreateShaders(shaderData);
		ReflectAllShaderStages(shaderData);

		SHADER_SYSTEM_INFO("Shader '{0}' loaded", mFilePath.string().c_str());
		mLoaded = true;
	}
	
	std::unordered_map<ShaderDomain, std::string> Shader::PreProcess(const std::string& inSource)
	{
		std::unordered_map<ShaderDomain, std::string> result;

		switch (mLanguage)
		{
		case ShaderLanguage::GLSL:
			result = PreProcessGLSL(inSource);
			break;
		case ShaderLanguage::HLSL:
			result = PreProcessHLSL(inSource);
			break;
		}

		return result;
	}
	
	std::unordered_map<ShaderDomain, std::string> Shader::PreProcessGLSL(const std::string& inSource)
	{
		std::unordered_map<ShaderDomain, std::string> result = ShaderPreProcessor::PreprocessShader<ShaderLanguage::GLSL>(inSource, mAcknowledgedMacros);
		shaderc::Compiler compiler;

		// NOTE: If any shader stage is compute and the shader source count is 1, 
		// the whole shader is considered a compute shader, because commonly a compute shader would be the only shader stage in a shader file.
		if (result.size() == 1)
		{
			for (auto& [domain, shaderSource] : result)
			{
				if (domain == ShaderDomain::Compute)
				{
					mIsCompute = true;
					break;
				}
			}
		}

		SHADER_SYSTEM_TRACE("Pre-processing GLSL: {0}", mFilePath.string().c_str());

		shaderc_util::FileFinder fileFinder;
		fileFinder.search_path().emplace_back("assets/shaders/Include/GLSL/");
		fileFinder.search_path().emplace_back("assets/shaders/Include/Shared/");

		for (auto& [domain, shaderSource] : result)
		{
			shaderc::CompileOptions options;

			if (mLanguage == ShaderLanguage::GLSL)
				options.AddMacroDefinition("__GLSL__");
			else if (mLanguage == ShaderLanguage::HLSL)
				options.AddMacroDefinition("__HLSL__");

			options.AddMacroDefinition("__OPENGL__");
			options.AddMacroDefinition(utils::ShaderDomainToMacro(domain).c_str());

			for (const auto& [name, value] : mMacros)
				options.AddMacroDefinition(name.c_str(), value.c_str());

			GLSLIncluder* includer = new GLSLIncluder(&fileFinder);
			options.SetIncluder(std::unique_ptr<GLSLIncluder>(includer));

			shaderc::PreprocessedSourceCompilationResult preProcessingResult = compiler.PreprocessGlsl(shaderSource.c_str(), utils::ShaderStageToShaderC(domain), mFilePath.string().c_str(), options);
			if (preProcessingResult.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				SHADER_SYSTEM_ERROR("Failed to pre-process Shader {0} with error {1}", mFilePath.string().c_str(), preProcessingResult.GetErrorMessage().c_str());
				SHADER_SYSTEM_ASSERT(false);
			}

			mStagesMetaData[domain].HashValue = HashString(shaderSource);
			mStagesMetaData[domain].Headers = std::move(includer->GetIncludeData());
			mAcknowledgedMacros.merge(includer->GetParsedSpecialMacros());

			result[domain] = std::string(preProcessingResult.begin(), preProcessingResult.end());
		}

		return result;
	}
	
	std::unordered_map<ShaderDomain, std::string> Shader::PreProcessHLSL(const std::string& inSource)
	{
		// TODO: implement
		std::unordered_map<ShaderDomain, std::string> result;

		return result;
	}
	
	void Shader::ReflectAllShaderStages(const std::unordered_map<ShaderDomain, std::vector<uint32_t>>& inShaderData)
	{
		mResources.clear();

		for (const auto& [stage, data] : inShaderData)
		{
			Reflect(stage, data);
		}
	}
	
	void Shader::Reflect(ShaderDomain shaderStage, const std::vector<uint32_t>& inShaderData)
	{
		SHADER_SYSTEM_TRACE("===============================");
		SHADER_SYSTEM_TRACE(" {0} Shader Reflection ", Renderer::GetCurrentRenderingAPI()->ToString());
		SHADER_SYSTEM_TRACE("===============================");

		spirv_cross::Compiler compiler(inShaderData);
		auto& resources = compiler.get_shader_resources();

		SHADER_SYSTEM_TRACE("Uniform Buffers: {0}", resources.uniform_buffers.size());
		for (const auto& resource : resources.uniform_buffers)
		{
			auto& activeBuffers = compiler.get_active_buffer_ranges(resource.id);
			if (activeBuffers.size())
			{
				const auto& name = compiler.get_name(resource.id); // this is necessary to get the instance name instead of the structure name
				const auto& uniformBlockName = compiler.get_name(resource.base_type_id);
				auto& bufferType = compiler.get_type(resource.base_type_id);
				uint32_t memberCount = (uint32_t)bufferType.member_types.size();
				uint32_t binding = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationBinding);
				uint32_t descriptorSet = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
				uint32_t size = (uint32_t)compiler.get_declared_struct_size(bufferType);
				uint32_t bufferOffset = 0;

				if (descriptorSet >= mShaderDescriptorSets.size())
					mShaderDescriptorSets.resize(descriptorSet + 1);

				ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];
				if (sUniformBuffers[descriptorSet].find(binding) == sUniformBuffers[descriptorSet].end())
				{
					ShaderUniformBuffer* uniformBuffer = new ShaderUniformBuffer();
					uniformBuffer->BindingPoint = binding;
					uniformBuffer->Size = size;
					uniformBuffer->Name = name;
					sUniformBuffers.at(descriptorSet)[binding] = uniformBuffer;
				}
				else
				{
					ShaderUniformBuffer* uniformBuffer = sUniformBuffers.at(descriptorSet).at(binding);
					if (size > uniformBuffer->Size)
						uniformBuffer->Size = size;
				}

				shaderDescriptorSet.UniformBuffers[binding] = sUniformBuffers.at(descriptorSet).at(binding);

				ShaderBuffer& shaderBuffer = mBuffers[uniformBlockName];
				shaderBuffer.Name = uniformBlockName;
				shaderBuffer.Size = size - bufferOffset;

				for (uint32_t i = 0; i < memberCount; ++i)
				{
					auto& memberType = compiler.get_type(bufferType.member_types[i]);
					const auto& memberName = compiler.get_member_name(bufferType.self, i);
					uint32_t memberSize = (uint32_t)compiler.get_declared_struct_member_size(bufferType, i);
					uint32_t memberOffset = (uint32_t)compiler.type_struct_member_offset(bufferType, i) - bufferOffset;

					const std::string& uniformName = fmt::format("{}.{}", name, memberName);
					shaderBuffer.Uniforms[uniformName] = ShaderUniform(uniformName, binding, utils::SPIRTypeToShaderUniformType(memberType), memberSize, memberOffset);
				}

				SHADER_SYSTEM_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
				SHADER_SYSTEM_TRACE("  Member Count: {0}", memberCount);
				SHADER_SYSTEM_TRACE("  Size: {0} bytes", size);
				SHADER_SYSTEM_TRACE("  Members:");
				for (uint32_t i = 0; i < memberCount; ++i)
				{
					const auto& memberName = compiler.get_member_name(bufferType.self, i);
					uint32_t memberSize = (uint32_t)compiler.get_declared_struct_member_size(bufferType, i);
					uint32_t memberOffset = (uint32_t)compiler.type_struct_member_offset(bufferType, i) - bufferOffset;
					const std::string& uniformName = fmt::format("{}.{}", name, memberName);

					SHADER_SYSTEM_TRACE("    Name: {0}, Full name: {1}", memberName, uniformName.c_str());
					SHADER_SYSTEM_TRACE("    Size: {0} bytes", memberSize);
					SHADER_SYSTEM_TRACE("    Offset: {0} bytes", memberOffset);
					SHADER_SYSTEM_TRACE("");
				}

				SHADER_SYSTEM_TRACE("-------------------");
			}
		}

		SHADER_SYSTEM_TRACE("Storage Buffers: {0}", resources.storage_buffers.size());

		for (const auto& resource : resources.storage_buffers)
		{
			auto& activeBuffers = compiler.get_active_buffer_ranges(resource.id);
			if (activeBuffers.size())
			{
				const auto& name = resource.name;
				auto& bufferType = compiler.get_type(resource.base_type_id);
				uint32_t memberCount = (uint32_t)bufferType.member_types.size();
				uint32_t binding = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationBinding);
				uint32_t descriptorSet = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
				uint32_t size = (uint32_t)compiler.get_declared_struct_size(bufferType);

				if (descriptorSet >= mShaderDescriptorSets.size())
					mShaderDescriptorSets.resize(descriptorSet + 1);

				ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];
				if (sStorageBuffers[descriptorSet].find(binding) == sStorageBuffers[descriptorSet].end())
				{
					ShaderStorageBuffer* storageBuffer = new ShaderStorageBuffer();
					storageBuffer->Name = name;
					storageBuffer->Size = size;
					storageBuffer->BindingPoint = binding;
					sStorageBuffers.at(descriptorSet)[binding] = storageBuffer;
				}
				else
				{
					ShaderStorageBuffer* storageBuffer = sStorageBuffers.at(descriptorSet).at(binding);
					if (size > storageBuffer->Size)
						storageBuffer->Size = size;
				}

				shaderDescriptorSet.StorageBuffers[binding] = sStorageBuffers.at(descriptorSet).at(binding);

				SHADER_SYSTEM_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
				SHADER_SYSTEM_TRACE("  Member Count: {0}", memberCount);
				SHADER_SYSTEM_TRACE("  Size: {0} bytes", size);
				SHADER_SYSTEM_TRACE("  Members:");
				for (uint32_t i = 0; i < memberCount; ++i)
				{
					const auto& memberName = compiler.get_member_name(bufferType.self, i);
					uint32_t memberSize = (uint32_t)compiler.get_declared_struct_member_size(bufferType, i);
					uint32_t memberOffset = (uint32_t)compiler.type_struct_member_offset(bufferType, i);
					const std::string& uniformName = fmt::format("{}.{}", name, memberName);

					SHADER_SYSTEM_TRACE("    Name: {0}, Full name: {1}", memberName, uniformName.c_str());
					SHADER_SYSTEM_TRACE("    Size: {0} bytes", memberSize);
					SHADER_SYSTEM_TRACE("    Offset: {0} bytes", memberOffset);
					SHADER_SYSTEM_TRACE("");
				}

				SHADER_SYSTEM_TRACE("-------------------");
			}
		}

		SHADER_SYSTEM_TRACE("Sampled Images: {0}", resources.sampled_images.size());
		for (const auto& resource : resources.sampled_images)
		{
			const auto& name = resource.name;
			auto& baseType = compiler.get_type(resource.base_type_id);
			auto& type = compiler.get_type(resource.type_id);
			uint32_t binding = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32_t arraySize = (uint32_t)type.array[0];

			if (arraySize == 0)
				arraySize = 1;

			if (descriptorSet >= mShaderDescriptorSets.size())
				mShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];

			auto& imageSampler = shaderDescriptorSet.ImageSamplers[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.ArraySize = arraySize;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;

			mResources[name] = ShaderResourceDeclaration(name, binding, 1);

			SHADER_SYSTEM_TRACE("  {0} (descriptor set: {1}, binding: {2})", name.c_str(), descriptorSet, binding);
		}

		SHADER_SYSTEM_TRACE("Separate Images: {0}", resources.separate_images.size());

		for (const auto& resource : resources.separate_images)
		{
			const auto& name = resource.name;
			auto& baseType = compiler.get_type(resource.base_type_id);
			auto& type = compiler.get_type(resource.type_id);
			uint32_t binding = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32_t arraySize = (uint32_t)type.array[0];

			if (arraySize == 0)
				arraySize = 1;

			if (descriptorSet >= mShaderDescriptorSets.size())
				mShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];

			auto& imageSampler = shaderDescriptorSet.SeparateTextures[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.ArraySize = arraySize;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;

			mResources[name] = ShaderResourceDeclaration(name, binding, 1);

			SHADER_SYSTEM_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
		}

		SHADER_SYSTEM_TRACE("Separate Samplers: {0}", resources.separate_samplers.size());

		for (const auto& resource : resources.separate_samplers)
		{
			const auto& name = resource.name;
			auto& baseType = compiler.get_type(resource.base_type_id);
			auto& type = compiler.get_type(resource.type_id);
			uint32_t binding = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32_t arraySize = (uint32_t)type.array[0];

			if (arraySize == 0)
				arraySize = 1;

			if (descriptorSet >= mShaderDescriptorSets.size())
				mShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];

			auto& imageSampler = shaderDescriptorSet.SeparateSamplers[binding];
			imageSampler.ArraySize = arraySize;
			imageSampler.Name = name;
			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;

			mResources[name] = ShaderResourceDeclaration(name, binding, 1);

			SHADER_SYSTEM_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
		}

		SHADER_SYSTEM_TRACE("Storage Images: {0}", resources.storage_images.size());

		for (const auto& resource : resources.storage_images)
		{
			const auto& name = resource.name;
			auto& type = compiler.get_type(resource.type_id);
			uint32_t binding = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32_t arraySize = (uint32_t)type.array[0];

			if (arraySize == 0)
				arraySize = 1;

			if (descriptorSet >= mShaderDescriptorSets.size())
				mShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];

			auto& imageSampler = shaderDescriptorSet.StorageImages[binding];
			imageSampler.ArraySize = arraySize;
			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;

			mResources[name] = ShaderResourceDeclaration(name, binding, 1);

			SHADER_SYSTEM_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
		}

		SHADER_SYSTEM_TRACE("Special macros: {0}", mAcknowledgedMacros.size());
		for (const auto& macro : mAcknowledgedMacros)
		{
			SHADER_SYSTEM_TRACE("{0}", macro.c_str());
		}

		SHADER_SYSTEM_TRACE("===========================");

		spirv_cross::CompilerGLSL glsl(inShaderData);
		ParseConstantBuffers(glsl);
	}
	
	void Shader::LoadCachedReflectionData()
	{
		// TODO: implement
	}
	
	std::string Shader::Compile(std::unordered_map<ShaderDomain, std::vector<uint32_t>>& outOutputBinary, const ShaderDomain inDomain) const
	{
		const std::string& stageSource = mShaderSources.at(inDomain);

		if (mLanguage == ShaderLanguage::GLSL)
		{
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetWarningsAsErrors();
			options.SetGenerateDebugInfo();

			bool optimize = false;
#ifdef SHADER_SYSTEM_RELEASE
			optimize = true;
#endif // SHADER_SYSTEM_RELEASE

			if (Renderer::GetCurrentRenderingAPIType() == RenderingAPIType::OpenGL)
			{
				options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
			}
			else if (Renderer::GetCurrentRenderingAPIType() == RenderingAPIType::Vulkan)
			{
				options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
			}

			if (optimize)
			{
				options.SetOptimizationLevel(shaderc_optimization_level_performance);
			}

			// Compile shader
			const shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(stageSource.c_str(), utils::ShaderStageToShaderC(inDomain), mFilePath.string().c_str(), options);

			if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				return fmt::format("Shader compilation error: {} ({})", result.GetErrorMessage(), mFilePath.string().c_str());
			}

			outOutputBinary[inDomain] = std::vector<uint32_t>(result.begin(), result.end());
			return std::string();
		}
		else if (mLanguage == ShaderLanguage::HLSL)
		{
			// TODO: compile code with dxcompiler and enable spv backend in the compiler
		}

		return "unknown Language!";
	}

	void Shader::CompileOrGetShaderBinary(std::unordered_map<ShaderDomain, std::vector<uint32_t>>& outBinary, bool inForceCompile)
	{
		std::filesystem::path cacheDirectory = utils::GetCacheDirectory();

		for (const auto& [domain, source] : mShaderSources)
		{
			if (source.empty())
			{
				continue;
			}

			const std::string& extension = utils::ShaderDomainCachedFileExtension(domain);
			if (!inForceCompile)
			{
				TryGetShaderCachedBinary(cacheDirectory, extension, outBinary, domain);
			}

			if (outBinary[domain].empty())
			{
				std::string error = Compile(outBinary, domain);
				if (error.empty())
				{
					// Compile success
					std::filesystem::path p = cacheDirectory / (mFilePath.filename().string() + extension);

					FILE* f = nullptr;
					errno_t fileError = fopen_s(&f, p.string().c_str(), "wb");
					if (fileError || !f)
					{
						SHADER_SYSTEM_ERROR("Failed to cache shader binary!");
						return;
					}

					fwrite(outBinary[domain].data(), sizeof(uint32_t), outBinary[domain].size(), f);
					fclose(f);
				}
				else
				{
					SHADER_SYSTEM_ERROR("{0}", error.c_str());

					TryGetShaderCachedBinary(cacheDirectory, extension, outBinary, domain);
					if (outBinary[domain].empty())
					{
						SHADER_SYSTEM_ASSERT(false, "Failed to compile shader and could not find any cached binary!");
						SHADER_SYSTEM_ERROR("Failed to compile shader and could not find any cached binary!");
					}
					else
					{
						SHADER_SYSTEM_ERROR("Failed to compile {0}:{1}, so loaded a cached shader binary instead. [-]", mFilePath.string().c_str(), utils::ShaderDomainToString(domain).c_str());
					}
				}
			}
		}
	}
	
	void Shader::TryGetShaderCachedBinary(const std::filesystem::path& inCacheDirectory, const std::string& inExtension, std::unordered_map<ShaderDomain, std::vector<uint32_t>>& outBinary, ShaderDomain inDomain) const
	{
		std::filesystem::path p = inCacheDirectory / (mFilePath.filename().string() + inExtension);

		FILE* f = nullptr;
		errno_t err = fopen_s(&f, p.string().c_str(), "rb");
		if (err || !f)
			return;

		fseek(f, 0, SEEK_END);
		uint64_t size = ftell(f);
		fseek(f, 0, SEEK_SET);

		outBinary[inDomain] = std::vector<uint32_t>(size / sizeof(uint32_t));
		fread(outBinary[inDomain].data(), sizeof(uint32_t), outBinary[inDomain].size(), f);
		fclose(f);
	}
	
	void Shader::LoadAndCreateShaders(const std::unordered_map<ShaderDomain, std::vector<uint32_t>>& inShaderData)
	{
		for (const auto& [domain, binary] : inShaderData)
		{
			SHADER_SYSTEM_ASSERT(binary.size() > 0);

			// DirectX11 and DirectX12 need HLSL binaries, so we need to cross compile the binaries.
			if (Renderer::GetCurrentRenderingAPIType() == RenderingAPIType::DirectX11 || Renderer::GetCurrentRenderingAPIType() == RenderingAPIType::DirectX12)
			{
				spirv_cross::CompilerHLSL hlslCompiler(binary);
				spirv_cross::CompilerHLSL::Options compileOptions;
				compileOptions.shader_model = 500;
				hlslCompiler.set_hlsl_options(compileOptions);
				std::string compilationResult = hlslCompiler.compile();

				// HACK: Remove space qualifiers, as they are not supported currently.
				std::regex pattern(", space[0-9]");

				std::string validCompilationResult = std::regex_replace(compilationResult, pattern, "");
				SHADER_SYSTEM_INFO("Cross-compilation result:\n{0}", validCompilationResult.c_str());

				mGpuShader->AddShaderDomain(validCompilationResult, domain);
			}
			// Metal needs MSL binaries
			else if (Renderer::GetCurrentRenderingAPIType() == RenderingAPIType::Metal)
			{
				spirv_cross::CompilerMSL mslCompiler(binary);
				spirv_cross::CompilerMSL::Options compileOptions;
				mslCompiler.set_msl_options(compileOptions);
				std::string compilationResult = mslCompiler.compile();

				mGpuShader->AddShaderDomain(compilationResult, domain);
			}
			// OpenGL and Vulkan can understand the spirv binary directly.
			else
			{
				mGpuShader->AddShaderDomain(binary, domain);
			}
		}

		if (!mGpuShader->LinkAllShaders())
		{
			SHADER_SYSTEM_ERROR("Failed to link shader!");
		}
	}

	void Shader::ParseConstantBuffers(const spirv_cross::CompilerGLSL& compiler)
	{
		// Push constant ranges
		spirv_cross::ShaderResources res = compiler.get_shader_resources();
		for (const spirv_cross::Resource &resource : res.push_constant_buffers)
		{
			const std::string &name = resource.name;
			auto &type = compiler.get_type(resource.base_type_id);
			auto bufferSize = (uint32_t)compiler.get_declared_struct_size(type);
			uint32_t binding = (uint32_t)compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t memberCount = (uint32_t)type.member_types.size();
			uint32_t bufferOffset = 0;

			if (mPushConstantRanges.size())
				bufferOffset = mPushConstantRanges.back().Offset + mPushConstantRanges.back().Size;

			auto &pushConstantRange = mPushConstantRanges.emplace_back();
			pushConstantRange.Size = bufferSize - bufferOffset;
			pushConstantRange.Offset = bufferOffset;

			if (name.empty())
			{
				mConstantBufferOffset += bufferSize;
				continue;
			}

			ShaderBuffer &buffer = mBuffers[name];
			buffer.Name = name;
			buffer.Size = bufferSize - bufferOffset;

			for (uint32_t i = 0; i < memberCount; ++i)
			{
				auto memberType = compiler.get_type(type.member_types[i]);
				const std::string &memberName = compiler.get_member_name(type.self, i);
				uint32_t size = (uint32_t)compiler.get_declared_struct_member_size(type, i);
				uint32_t offset = compiler.type_struct_member_offset(type, i) - bufferOffset;

				std::string uniformName = fmt::format("{}.{}", name.c_str(), memberName.c_str());

				SHADER_SYSTEM_INFO("Registering push_constant with uniform name {0}", uniformName.c_str());

				buffer.Uniforms[uniformName] = ShaderUniform(uniformName, binding, utils::SPIRTypeToShaderUniformType(type), size, offset);
			}

			mConstantBufferOffset += bufferSize;
		}
	}
}
