#include "OpenGLShader.h"

#include "Core/Logger.h"

namespace ShaderSystem
{
	namespace utils
	{
		static GLenum ShaderDomainToGLStage(ShaderDomain inDomain)
		{
			switch (inDomain)
			{
			case ShaderDomain::Vertex:
				return GL_VERTEX_SHADER;

			case ShaderDomain::Fragment:
				return GL_FRAGMENT_SHADER;

			case ShaderDomain::Compute:
				return GL_COMPUTE_SHADER;

			case ShaderDomain::Geometry:
				return GL_GEOMETRY_SHADER;

			case ShaderDomain::TessControl:
				return GL_TESS_CONTROL_SHADER;

			case ShaderDomain::TessEvalulation:
				return GL_TESS_EVALUATION_SHADER;

			case ShaderDomain::None:
			default:
				SHADER_SYSTEM_ERROR("Unknown shader domain '{0}'!", inDomain);
				return 0;
			}
		}
	}

	OpenGLShader::OpenGLShader()
	{
		mProgramID = glCreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		ReleaseAllDomains();
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mProgramID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::ReleaseAllDomains()
	{
		for (const auto& [d, s] : mShaderHandles)
		{
			glDeleteShader(s);
		}

		glDeleteProgram(mProgramID);
		mProgramID = 0;
		mShaderHandles.clear();
		mTextDomains.clear();
		mBinaryDomains.clear();
	}

	bool OpenGLShader::LinkAllShaders()
	{
		for (const auto& [d, s] : mShaderHandles)
			glAttachShader(mProgramID, s);

		glLinkProgram(mProgramID);

		GLint isLinked = 0;
		glGetProgramiv(mProgramID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxInfoLength = 0;
			glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &maxInfoLength);

			std::vector<GLchar> infoLog(maxInfoLength);
			if (!infoLog.empty())
			{
				glGetProgramInfoLog(mProgramID, maxInfoLength, &maxInfoLength, &infoLog[0]);
				SHADER_SYSTEM_ERROR("Shader Linking failed:\n{0} [-]", &infoLog[0]);

				for (const auto& [d, s] : mShaderHandles)
					glDeleteShader(s);

				glDeleteProgram(mProgramID);
				mProgramID = 0;
				return false;
			}
		}

		for (const auto &[d, s]: mShaderHandles)
			glDetachShader(mProgramID, s);

		return true;
	}
	
	void OpenGLShader::AddShaderDomain(const std::string& inSourceString, ShaderDomain inDomain)
	{
		 const char* c_str = inSourceString.c_str();
		 GLint c_str_size = (GLint)inSourceString.size();

		 GLuint shaderId = glCreateShader(utils::ShaderDomainToGLStage(inDomain));
		 glShaderSource(shaderId, 1, &c_str, &c_str_size);

		 glCompileShader(shaderId);

		 GLint isCompiled = 0;
		 glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
		 if (isCompiled == GL_FALSE)
		 {
			 GLint maxInfoLength = 0;
			 glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxInfoLength);

			 std::vector<GLchar> infoLog(maxInfoLength);
			 glGetShaderInfoLog(shaderId, maxInfoLength, &maxInfoLength, &infoLog[0]);
			 SHADER_SYSTEM_ERROR("Shader Compilation failed:\n{0} [-]", &infoLog[0]);

			 glDeleteShader(shaderId);
			 for (const auto& [d, s] : mShaderHandles)
			 {
				 glDeleteShader(s);
			 }

			 glDeleteProgram(mProgramID);
		 }

		 mTextDomains[inDomain] = inSourceString;
		 mShaderHandles[inDomain] = shaderId;
	}
	
	const std::string& OpenGLShader::GetShaderDomainSource(ShaderDomain inDomain) const
	{
		for (const auto& [d, s] : mTextDomains)
		{
			if (d == inDomain)
			{
				return s;
			}
		}

		return std::string();
	}
	
	void OpenGLShader::AddShaderDomain(const std::vector<uint32_t>& inSourceBinary, ShaderDomain inDomain)
	{
		GLuint shaderId = glCreateShader(utils::ShaderDomainToGLStage(inDomain));
		glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, inSourceBinary.data(), (GLsizei)(inSourceBinary.size() * sizeof(uint32_t)));
		glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);

		GLint isCompiled = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxInfoLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxInfoLength);

			std::vector<GLchar> infoLog(maxInfoLength);
			glGetShaderInfoLog(shaderId, maxInfoLength, &maxInfoLength, &infoLog[0]);
			SHADER_SYSTEM_ERROR("Shader Compilation failed:\n{0} [-]", &infoLog[0]);

			glDeleteShader(shaderId);
			for (const auto& [d, s] : mShaderHandles)
			{
				glDeleteShader(s);
			}

			glDeleteProgram(mProgramID);
		}

		mBinaryDomains[inDomain] = inSourceBinary;
		mShaderHandles[inDomain] = shaderId;
	}
	
	const std::vector<uint32_t>& OpenGLShader::GetShaderDomainBinary(ShaderDomain inDomain) const
	{
		for (const auto& [d, b] : mBinaryDomains)
		{
			if (d == inDomain)
			{
				return b;
			}
		}

		return std::vector<uint32_t>();
	}
}
