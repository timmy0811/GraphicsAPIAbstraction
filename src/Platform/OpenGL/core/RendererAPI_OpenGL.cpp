#include "glpch.h"
#include "RendererAPI_OpenGL.h"

#include <GL/glew.h>

void OpenGL::Core::RendererAPI_OpenGL::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGL::Core::RendererAPI_OpenGL::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::Core::RendererAPI_OpenGL::Draw(const std::shared_ptr<API::Core::VertexArray>& vertexArray, size_t size)
{
	GLCall(glDrawArrays(GL_TRIANGLES, 0, size));
}

void OpenGL::Core::RendererAPI_OpenGL::DrawIndexed(const std::shared_ptr<API::Core::VertexArray>& vertexArray, const std::shared_ptr<API::Core::IndexBuffer>& indexBuffer)
{
	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void OpenGL::Core::RendererAPI_OpenGL::DrawInstanced(const std::shared_ptr<API::Core::VertexArray>& vertexArray, size_t size, unsigned int instances)
{
	GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, size, instances));
}

void OpenGL::Core::RendererAPI_OpenGL::DrawInstanced(const std::shared_ptr<API::Core::VertexArray>& vertexArray, const std::shared_ptr<API::Core::IndexBuffer>& indexBuffer, unsigned int instances)
{
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, instances));
}