#pragma once

#include "RendererAPI.h"

namespace API::Core {
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void Draw(const std::shared_ptr<VertexArray>& vertexArray, size_t size)
		{
			s_RendererAPI->Draw(vertexArray, size);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexBuffer);
		}

		inline static void DrawInstanced(const std::shared_ptr<VertexArray>& vertexArray, size_t size, unsigned int instances)
		{
			s_RendererAPI->DrawInstanced(vertexArray, size, instances);
		}

		inline static void DrawInstanced(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer, unsigned int instances)
		{
			s_RendererAPI->DrawInstanced(vertexArray, indexBuffer, instances);
		}

		inline static const std::string GetAPIVer() { return s_RendererAPI->GetAPIVer(); }
		inline static const std::string GetGPUID() { return s_RendererAPI->GetGPUID(); }
		inline static const std::string GetPublisher() { return s_RendererAPI->GetPublisher(); }
		inline static const std::string GetShaderLanID() { return s_RendererAPI->GetShaderLanID(); }

	private:
		static RendererAPI* s_RendererAPI;
	};
}