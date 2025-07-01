//
// Created by arunc on 01/07/2025.
//

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP
#include <glad/glad.h>
#include <functional>
#include <iostream>

class FrameBuffer {
public:
	FrameBuffer(int w, int h)
		: width(w), height(h)
	{
		Init();
	}

	~FrameBuffer() {
		Delete();
	}

	void Resize(int w, int h) {
		width = w;
		height = h;
		Init();
	}

	void Init() {
		Delete();

		glGenFramebuffers(1, &msFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, msFBO);

		glGenTextures(1, &msColorTex);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msColorTex);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA8, width, height, GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msColorTex, 0);

		glGenRenderbuffers(1, &msDepthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, msDepthRBO);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msDepthRBO);

		// === Final Framebuffer ===
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

		glGenRenderbuffers(1, &_depthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRBO);

		framebufferReady = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		if (!framebufferReady) {
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			std::cerr << "FrameBuffer initialization failed! Status: " << std::hex << status << std::endl;
			std::cerr << "Framebuffer size: " << width << "x" << height << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Render(const std::function<void()>& renderFunc) {
		if (!framebufferReady) Init();

		glBindFramebuffer(GL_FRAMEBUFFER, msFBO);
		glViewport(0, 0, width, height);
		glClearColor(0.247f, 0.247f, 0.247f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderFunc();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void BlitToTexture() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);

		glBlitFramebuffer(
			0, 0, width, height,
			0, 0, width, height,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR
		);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Delete() {
		if (_texture) glDeleteTextures(1, &_texture);
		if (_framebuffer) glDeleteFramebuffers(1, &_framebuffer);
		if (_depthRBO) glDeleteRenderbuffers(1, &_depthRBO);

		if (msColorTex) glDeleteTextures(1, &msColorTex);
		if (msFBO) glDeleteFramebuffers(1, &msFBO);
		if (msDepthRBO) glDeleteRenderbuffers(1, &msDepthRBO);

		_texture = _framebuffer = _depthRBO = 0;
		msFBO = msColorTex = msDepthRBO = 0;
	}


	GLuint _framebuffer = 0;
	GLuint _texture = 0;
	GLuint _depthRBO = 0;

	GLuint msFBO = 0;
	GLuint msColorTex = 0;
	GLuint msDepthRBO = 0;

	int width = 512;
	int height = 512;
	bool framebufferReady = false;
};

#endif //FRAMEBUFFER_HPP
