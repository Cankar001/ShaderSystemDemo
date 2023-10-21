#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace ShaderSystem
{
	namespace utils
	{
		static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;

				// OpenGL has problems with uploading Int4s on the GPU unless
				// they are defined as GL_FLOAT.
			case ShaderDataType::Int4:     return GL_FLOAT;

			case ShaderDataType::Bool:     return GL_BOOL;
			}

			return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		// NOTE: only one global vao
		glCreateVertexArrays(1, &mRendererID);
		glBindVertexArray(mRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
	}
	
	void OpenGLVertexArray::Bind(const BufferLayout &inLayout) const
	{
		uint32_t attribIndex = 0;
		for (const auto &element : inLayout)
		{
			GLenum glBaseType = utils::ShaderDataTypeToOpenGLBaseType(element.Type);
			glEnableVertexAttribArray(attribIndex);

			if (glBaseType == GL_INT)
			{
				glVertexAttribIPointer(attribIndex,
									   element.GetComponentCount(),
									   glBaseType,
									   inLayout.GetStride(),
									   (const void *)(intptr_t)element.Offset);
			}
			else
			{
				glVertexAttribPointer(attribIndex,
									  element.GetComponentCount(),
									  glBaseType,
									  element.Normalized,
									  inLayout.GetStride(),
									  (const void *)(intptr_t)element.Offset);
			}
			++attribIndex;
		}
	}
	
	void OpenGLVertexArray::Unbind()
	{
		// NOTE: Nothing to do for this renderer backend.
	}
}
