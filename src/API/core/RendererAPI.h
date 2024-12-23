#pragma once

#include <vendor/glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"

namespace API::Core {
	class RendererAPI
	{
	public:
		enum class API_ENUM
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void Draw(const std::shared_ptr<VertexArray>& vertexArray, size_t size) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual void DrawInstanced(const std::shared_ptr<VertexArray>& vertexArray, size_t size, unsigned int instances) = 0;
		virtual void DrawInstanced(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer, unsigned int instances) = 0;

		inline static API_ENUM GetAPI() { return s_API; }

		virtual const std::string GetAPIVer() const = 0;
		virtual const std::string GetGPUID() const = 0;
		virtual const std::string GetPublisher() const = 0;
		virtual const std::string GetShaderLanID() const = 0;

	private:
		static API_ENUM s_API;
	};
}