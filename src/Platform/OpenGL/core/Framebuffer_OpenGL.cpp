#include "glpch.h"
#include "Framebuffer_OpenGL.h"

OpenGL::Core::Framebuffer_OpenGL::Framebuffer_OpenGL(const glm::ivec2& size, bool attachDepth, DepthBufferType depthType)
{
	m_Width = size.x;
	m_Height = size.y;

	m_Attachements.reserve(MAX_BUF);

	GLCall(glGenFramebuffers(1, &m_IdFBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_IdFBO));

	if (attachDepth) {
		switch (depthType)
		{
		case DepthBufferType::WRITE_ONLY:
			glGenRenderbuffers(1, &m_RBODepth);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBODepth);

			break;
		case DepthBufferType::WRITE_READ:
			GLCall(glGenTextures(1, &m_RBODepth));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RBODepth));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_IdFBO));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_RBODepth, 0));

			GLCall(glDrawBuffer(GL_NONE));
			GLCall(glReadBuffer(GL_NONE));

			break;
		default:
			break;
		}
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[OpenGL Error] (" << std::to_string(status) << ")" << std::endl;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

OpenGL::Core::Framebuffer_OpenGL::Framebuffer_OpenGL(const glm::ivec2& size)
{
	m_Width = size.x;
	m_Height = size.y;

	m_Attachements.reserve(MAX_BUF);

	GLCall(glGenFramebuffers(1, &m_IdFBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_IdFBO));

	m_Attachements.push_back(GL_COLOR_ATTACHMENT0);

	// Color attachment
	glGenTextures(1, &m_Attachements[0]);
	glBindTexture(GL_TEXTURE_2D, m_Attachements[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Attachements[0], 0);

	// Depth attachment
	glGenRenderbuffers(1, &m_RBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBODepth);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[OpenGL Error] (" << std::to_string(status) << ")" << std::endl;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int OpenGL::Core::Framebuffer_OpenGL::BindDepthTexture(const unsigned int slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RBODepth));
	m_BoundPort = static_cast<int>(slot);
	return m_BoundPort;
}

void OpenGL::Core::Framebuffer_OpenGL::Bind(unsigned int Framebuffer)
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Framebuffer));
}

void OpenGL::Core::Framebuffer_OpenGL::Bind() const
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IdFBO));
	GLCall(glViewport(0, 0, m_Width, m_Height));
}

void OpenGL::Core::Framebuffer_OpenGL::BindAndClear()
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IdFBO));
	GLCall(glViewport(0, 0, m_Width, m_Height));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGL::Core::Framebuffer_OpenGL::Unbind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGL::Core::Framebuffer_OpenGL::BindTextures(const unsigned int startSlot)
{
	for (int i = 0; i < m_Buffers.size(); i++) {
		GLCall(glActiveTexture(GL_TEXTURE0 + startSlot + i));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Buffers[i]));
	}
}

void OpenGL::Core::Framebuffer_OpenGL::BindTexture(int index, const unsigned int startSlot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + startSlot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Buffers[index]));
}

bool OpenGL::Core::Framebuffer_OpenGL::PushColorAttribute(const char channel, BufferDataType dataType, const void* data)
{
	BufferFormat formatIntern{};
	BufferFormat format{};

	switch (channel) {
	case(1):
		format = BufferFormat::R;

		switch (dataType)
		{
		case _FLOAT:
			formatIntern = BufferFormat::R32F; break;
		case _FLOAT16:
			formatIntern = BufferFormat::R16F;
			dataType = BufferDataType::_FLOAT; break;
		case _DOUBLE:
			return false;
		case _BYTE_UNSIGNED:
			formatIntern = BufferFormat::R; break;
		case _BYTE:
			formatIntern = BufferFormat::R8I; break;
		case _SHORT:
			formatIntern = BufferFormat::R16I; break;
		case _SHORT_UNSIGNED:
			formatIntern = BufferFormat::R16UI; break;
		case _INT_UNSIGNED:
			formatIntern = BufferFormat::R32UI; break;
		case _INT:
			formatIntern = BufferFormat::R32I; break;
		default:
			return false;
		}
		break;

	case(2):
		format = BufferFormat::RG;

		switch (dataType)
		{
		case _FLOAT:
			formatIntern = BufferFormat::RG32F; break;
		case _FLOAT16:
			formatIntern = BufferFormat::RG16F;
			dataType = BufferDataType::_FLOAT; break;
		case _DOUBLE:
			return false;
		case _BYTE_UNSIGNED:
			formatIntern = BufferFormat::RG; break;
		case _BYTE:
			formatIntern = BufferFormat::RG8I; break;
		case _SHORT:
			formatIntern = BufferFormat::RG16I; break;
		case _SHORT_UNSIGNED:
			formatIntern = BufferFormat::RG16UI; break;
		case _INT_UNSIGNED:
			formatIntern = BufferFormat::RG32UI; break;
		case _INT:
			formatIntern = BufferFormat::RG32I; break;
		default:
			return false;
		}
		break;

	case(3):
		format = BufferFormat::RGB;

		switch (dataType)
		{
		case _FLOAT:
			formatIntern = BufferFormat::RGB32F; break;
		case _FLOAT16:
			formatIntern = BufferFormat::RGB16F;
			dataType = BufferDataType::_FLOAT; break;
		case _DOUBLE:
			return false;
		case _BYTE_UNSIGNED:
			formatIntern = BufferFormat::RGB; break;
		case _BYTE:
			formatIntern = BufferFormat::RGB8I; break;
		case _SHORT:
			formatIntern = BufferFormat::RGB16I; break;
		case _SHORT_UNSIGNED:
			formatIntern = BufferFormat::RGB16UI; break;
		case _INT_UNSIGNED:
			formatIntern = BufferFormat::RGB32UI; break;
		case _INT:
			formatIntern = BufferFormat::RGB32I; break;
		default:
			return false;
		}
		break;

	case(4):
		format = BufferFormat::RGBA;

		switch (dataType)
		{
		case _FLOAT:
			formatIntern = BufferFormat::RGBA32F; break;
		case _FLOAT16:
			formatIntern = BufferFormat::RGBA16F;
			dataType = BufferDataType::_FLOAT; break;
		case _DOUBLE:
			return false;
		case _BYTE_UNSIGNED:
			formatIntern = BufferFormat::RGBA; break;
		case _BYTE:
			formatIntern = BufferFormat::RGBA8I; break;
		case _SHORT:
			formatIntern = BufferFormat::RGBA16I; break;
		case _SHORT_UNSIGNED:
			formatIntern = BufferFormat::RGBA16UI; break;
		case _INT_UNSIGNED:
			formatIntern = BufferFormat::RGBA32UI; break;
		case _INT:
			formatIntern = BufferFormat::RGBA32I; break;
		default:
			return false;
		}
		break;

	default:
		return false;
	}

	size_t size = m_Buffers.size();
	m_Buffers.push_back(0);

	GLCall(glGenTextures(1, &(m_Buffers[size])));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Buffers[size]));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, formatIntern, m_Width, m_Height, 0, format, dataType, data));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (int)size, GL_TEXTURE_2D, m_Buffers[size], 0));

	m_Attachements.push_back(GL_COLOR_ATTACHMENT0 + (int)m_Attachements.size());
	glDrawBuffers((int)m_Attachements.size(), &m_Attachements[0]);

	return true;
}

bool OpenGL::Core::Framebuffer_OpenGL::PushColorAttribute(unsigned int internalFormat, unsigned int format, unsigned int dataType, const void* data)
{
	size_t size = m_Buffers.size();
	m_Buffers.push_back(0);

	GLCall(glGenTextures(1, &(m_Buffers[size])));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Buffers[size]));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, dataType, data));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (int)size, GL_TEXTURE_2D, m_Buffers[size], 0));

	m_Attachements.push_back(GL_COLOR_ATTACHMENT0 + (int)m_Attachements.size());
	glDrawBuffers((int)m_Attachements.size(), &m_Attachements[0]);

	return true;
}

inline unsigned int OpenGL::Core::Framebuffer_OpenGL::GetColorAttachmentTextureID(unsigned int index)
{
	return m_Attachements[index];
}

inline bool OpenGL::Core::Framebuffer_OpenGL::Validate() const
{
	Bind();
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[OpenGL Error] (" << std::to_string(status) << ")" << std::endl;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return false;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	return true;
}