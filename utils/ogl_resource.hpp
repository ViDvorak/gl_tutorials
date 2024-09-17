#pragma once

#include <glad/glad.h>
#include <functional>
#include <utility>
#include <memory>

#include "error_handling.hpp"

/// <summary>
/// Fufills the same objective as original OpenGLRecource
/// Contains an IDs of Loaded Open GL recources to locate them within the OpenGL library
/// </summary>
class Resource {
public:
	Resource()
		: mId(0)
	{}

	Resource(
		std::function<GLuint()> aCreateFunc,
		std::function<void(GLuint)> aDeleteFunc)
		: mCreateFunc(aCreateFunc)
		, mDeleteFunc(aDeleteFunc)
	{
		mId = mCreateFunc();
	}

	~Resource() {
		if (mDeleteFunc) {
			mDeleteFunc(mId);
		}
	}

	// Disable copy operations
	Resource(const Resource&) = delete;
	Resource& operator=(const Resource&) = delete;

	// Enable move operations
	Resource(Resource&& other) noexcept
		: mId(std::exchange(other.mId, 0))
		, mCreateFunc(std::move(other.mCreateFunc))
		, mDeleteFunc(std::move(other.mDeleteFunc))
	{}

	Resource& operator=(Resource&& other) noexcept {
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

/// <summary>
/// Wraps Resource within copy and move capable object
/// </summary>
class OpenGLResource {
public:
	OpenGLResource()
		: resource(std::make_shared<Resource>(Resource()))
	{}

	OpenGLResource(
		std::function<GLuint()> aCreateFunc,
		std::function<void(GLuint)> aDeleteFunc) : 
		resource( std::make_shared<Resource>(Resource(aCreateFunc, aDeleteFunc)))
	{}

	~OpenGLResource() = default;

	// Enabled copy operations
	OpenGLResource(const OpenGLResource& orig) : resource(orig.resource)
	{}

	OpenGLResource& operator=(const OpenGLResource& other) {
		if (this != &other) {
			resource = other.resource;
		}
		return *this;
	}

	// Enable move operations
	OpenGLResource(OpenGLResource&& other) noexcept
		: resource(std::move(other.resource))
	{}

	OpenGLResource& operator=(OpenGLResource&& other) noexcept {
		if (this != &other) {
			this->resource = std::move(other.resource);
		}
		return *this;
	}

	GLuint get() const { return resource->get(); }

private:
	std::shared_ptr<Resource> resource;
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

inline OpenGLResource createTransformFeedback() {
	return OpenGLResource(
		[] {
			GLuint id = 0;
			GL_CHECK(glGenTransformFeedbacks(1, &id));
			return id;
		},
		[](GLuint id) {
			GL_CHECK(glDeleteTransformFeedbacks(1, &id));
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
