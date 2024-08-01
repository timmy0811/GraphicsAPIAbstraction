#pragma once

#include "RendererCommand.h"

namespace API::Core {
	// This class can be seen as a template clas for RendererContexts that can be implemented in the user application
	// if it is nessecary.

	class DefaultRendererContext {
	public:
		static void BeginScene();
		static void EndScene();

		static void SubmitDraw(const std::shared_ptr<VertexArray>& va, size_t size);
		static void SubmitDraw(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<IndexBuffer>& ib);

		static void SubmitInstancedDraw(const std::shared_ptr<VertexArray>& va, size_t size, unsigned int instances);
		static void SubmitInstancedDraw(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<IndexBuffer>& ib, unsigned int instances);

		inline static RendererAPI::API_ENUM GetAPI() { return RendererAPI::GetAPI(); }

	public:
		static inline int FramebufferOriginId;
	};
}
