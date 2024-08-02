#pragma once

#include "API/core/RendererAPI.h"

namespace OpenGL::Core {
	class RendererAPI_OpenGL : public API::Core::RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void Draw(const std::shared_ptr<API::Core::VertexArray>& vertexArray, size_t size) override;
		virtual void DrawIndexed(const std::shared_ptr<API::Core::VertexArray>& vertexArray, const std::shared_ptr<API::Core::IndexBuffer>& ib) override;

		virtual void DrawInstanced(const std::shared_ptr<API::Core::VertexArray>& vertexArray, size_t size, unsigned int instances) override;
		virtual void DrawInstanced(const std::shared_ptr<API::Core::VertexArray>& vertexArray, const std::shared_ptr<API::Core::IndexBuffer>& indexBuffer, unsigned int instances) override;

		virtual const std::string GetAPIVer() const;
		virtual const std::string GetGPUID() const;
		virtual const std::string GetPublisher() const;
		virtual const std::string GetShaderLanID() const;
	};
}