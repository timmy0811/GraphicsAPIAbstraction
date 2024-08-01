#include "glpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/core/Buffer_OpenGL.h"

API::Core::Buffer* API::Core::Buffer::Create(BufferType type, const void* data, unsigned int size)
{
	switch (API::Core::DefaultRendererContext::GetAPI())
	{
	case API::Core::RendererAPI::API_ENUM::None:    API_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case API::Core::RendererAPI::API_ENUM::OpenGL:  return new OpenGL::Core::Buffer_OpenGL(type, data, size);
	}

	API_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

API::Core::Buffer* API::Core::Buffer::Create(BufferType type, unsigned int count, size_t elementSize)
{
	switch (API::Core::DefaultRendererContext::GetAPI())
	{
	case API::Core::RendererAPI::API_ENUM::None:    API_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case API::Core::RendererAPI::API_ENUM::OpenGL:  return new OpenGL::Core::Buffer_OpenGL(type, count, elementSize);
	}

	API_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}