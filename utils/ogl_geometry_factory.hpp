#pragma once

#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <filesystem>

#include "geometry_factory.hpp"
#include "ogl_geometry_construction.hpp"


namespace fs = std::filesystem;

class OGLGeometry: public AGeometry {
public:
	OGLGeometry(IndexedBuffer buff):
		buffer(std::move(buff)), currentTFBOidx_ptr(std::make_unique<unsigned int>(0))
	{
	}

	IndexedBuffer buffer;
	std::unique_ptr<unsigned int> currentTFBOidx_ptr;

	void bind() const{
		std::cout << "buffer binding" << std::endl;
		if (buffer.isTransformFeedbackLoopEnabled) {
			int previusTFBO = (*currentTFBOidx_ptr + 1) % 2;

			GL_CHECK(glBindVertexArray(buffer.vaos[previusTFBO].get()));
			// GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer.tfbos[previusTFBO].get())); // it is already bound by VAO
			GL_CHECK(glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer.tfbos[*currentTFBOidx_ptr].get()));

			*currentTFBOidx_ptr = previusTFBO;
		}

		else {
			GL_CHECK(glBindVertexArray(buffer.vaos[0].get()));
		}
	}

	void draw() const{
		draw(buffer.mode);
	}

	void draw(GLenum aMode) const {
		if (buffer.isTransformFeedbackLoopEnabled) {
			std::cout << "-----" << std::endl << "draw mode " << aMode << std::endl; // it does not reach here
			GL_CHECK(glBeginTransformFeedback(GL_TRIANGLES));
		}

		if (buffer.instanceCount == 0) {
			GL_CHECK(glDrawElements(aMode, buffer.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0)));
		} else {
			// OpenGL error:
			// glDrawElementsInstanced(aMode, buffer.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0), buffer.instanceCount):
			// INVALID_OPERATION

			// GL_CHECK(glDrawArraysInstanced(GL_POINTS, 0, buffer.indexCount, buffer.instanceCount));
			GL_CHECK(glDrawElementsInstanced(aMode, buffer.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0), buffer.instanceCount)); // requires EBO
		}

		if (buffer.isTransformFeedbackLoopEnabled) {
			GL_CHECK(glEndTransformFeedback());
			// TODO switch base buffer to newly populated one
			// is there a need to use vbos for the switch?
			int previusTFBO = (*currentTFBOidx_ptr + 1) % 2;

			//auto glGeometry = (const_cast<OGLGeometry*> (this));
			
			// ping-pong buffering
			// std::swap( buffer.vaos [*currentTFBO_ptr], buffer.vaos [previusTFBO]);
			// std::swap( buffer.tfbos[*currentTFBO_ptr], buffer.tfbos[previusTFBO]);
		}
	}
};

class OGLGeometryFactory: public GeometryFactory {
public:
	std::shared_ptr<AGeometry> getAxisGizmo();
	std::shared_ptr<AGeometry> getCube();
	std::shared_ptr<AGeometry> getCubeOutline();
	std::shared_ptr<AGeometry> getCubeNormTex();

	std::shared_ptr<AGeometry> getPlane();
	std::shared_ptr<AGeometry> getPlaneOutline();

	std::shared_ptr<AGeometry> loadMesh(fs::path aMeshPath, RenderStyle aRenderStyle);
protected:
	std::map<std::string, std::shared_ptr<OGLGeometry>> mObjects;
};
