#pragma once

#include <glad/glad.h>
#include <functional>
#include <utility>

#include "error_handling.hpp"

class OpenGLResource {
public:
	OpenGLResource()
		: mId(0)
	{}

	OpenGLResource(
		std::function<GLuint()> aCreateFunc,
		std::function<void(GLuint)> aDeleteFunc)
		: mCreateFunc(aCreateFunc)
		, mDeleteFunc(aDeleteFunc)
	{
		mId = mCreateFunc();
	}

	~OpenGLResource() {
		if (mDeleteFunc) {
			mDeleteFunc(mId);
		}
	}

	// Disable copy operations
	OpenGLResource(const OpenGLResource&) = delete;
	OpenGLResource& operator=(const OpenGLResource&) = delete;

	// Enable move operations
	OpenGLResource(OpenGLResource&& other) noexcept
		: mId(std::exchange(other.mId, 0))
		, mCreateFunc(std::move(other.mCreateFunc))
		, mDeleteFunc(std::move(other.mDeleteFunc))
	{}

	OpenGLResource& operator=(OpenGLResource&& other) noexcept {
		if (this != &other) {
			if (mDeleteFunc) {
				mDeleteFunc(mId);
			}
			mId = std::exchange(other.mId, 0);
			mCreateFunc = std::move(other.mCreateFunc);
			mDeleteFunc = std::move(other.mDeleteFunc);
		}
		return *this;
	}

	GLuint get() const { return mId; }

private:
	GLuint mId = 0; // OpenGL resource ID
	std::function<GLuint(void)> mCreateFunc;
	std::function<void(GLuint)> mDeleteFunc;
};

inline OpenGLResource createVertexArray() {
	return OpenGLResource(
		[]{
			GLuint id = 0;
			GL_CHECK(glGenVertexArrays(1, &id));
			return id;
		},
		[](GLuint id){
			glDeleteVertexArrays(1, &id);
		});
}

inline OpenGLResource createBuffer() {
	return OpenGLResource(
		[]{
			GLuint id = 0;
			GL_CHECK(glGenBuffers(1, &id));
			return id;
		},
		[](GLuint id){
			glDeleteBuffers(1, &id);
		});
}

inline OpenGLResource createQuery() {
	return OpenGLResource(
		[]{
			GLuint id = 0;
			GL_CHECK(glGenQueries(1, &id));
			return id;
		},
		[](GLuint id){
			glDeleteQueries(1, &id);
		});
}


inline OpenGLResource createRenderBuffer() {
	return OpenGLResource(
		[]{
			GLuint id = 0;
			GL_CHECK(glGenRenderbuffers(1, &id));
			return id;
		},
		[](GLuint id){
			glDeleteRenderbuffers(1, &id);
		});
}

inline OpenGLResource createFramebuffer() {
	return OpenGLResource(
		[]{
			GLuint id = 0;
			GL_CHECK(glGenFramebuffers(1, &id));
			return id;
		},
		[](GLuint id){
			glDeleteFramebuffers(1, &id);
		});
}

inline OpenGLResource createShader(GLenum aShaderType) {
	return OpenGLResource(
		[aShaderType]{
			return glCreateShader(aShaderType);
		},
		[](GLuint id){
			glDeleteShader(id);
		});
}

inline OpenGLResource createShaderProgram() {
	return OpenGLResource(
		[]{
			return glCreateProgram();
		},
		[](GLuint id){
			glDeleteProgram(id);
		});
}

inline OpenGLResource createTexture() {
	return OpenGLResource(
		[]{
			GLuint id = 0;
			GL_CHECK(glGenTextures(1, &id));
			return id;
		},
		[](GLuint id){
			glDeleteTextures(1, &id);
		});
}

inline OpenGLResource createSampler() {
	return OpenGLResource(
		[]{
			GLuint id = 0;
			GL_CHECK(glGenSamplers(1, &id));
			return id;
		},
		[](GLuint id){
			glDeleteSamplers(1, &id);
		});
}
