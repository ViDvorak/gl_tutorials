#pragma once

#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <filesystem>

#include "geometry_factory.hpp"
#include "ogl_geometry_construction.hpp"


namespace fs = std::filesystem;

unsigned int currentTFBOIndex = 0;

class OGLGeometry: public AGeometry {
public:


	OGLGeometry(IndexedBuffer buff):
		buffer(std::move(buff))
	{}

	// problém je tady není definovaný copy construktor
	// problem is there is no OpenGLResource copy constructor? or it is that there is automatic OGLGeometry copy constructor

	IndexedBuffer buffer;



	void bind() const{
		if (buffer.isTransformFeedbackLoopEnabled) {
			int previusTFBO = (currentTFBOIndex + 1) % 2;

			GL_CHECK(glBindVertexArray(buffer.vaos[previusTFBO].get()));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer.vbos[previusTFBO].get()));
			GL_CHECK(glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer.vbos[currentTFBOIndex].get()));

			currentTFBOIndex = previusTFBO;
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
			std::cout << "-----" << std::endl << "draw mode " << aMode << std::endl;
			GL_CHECK(glBeginTransformFeedback(aMode));
		}

		if (buffer.instanceCount == 0) {
			GL_CHECK(glDrawElements(aMode, buffer.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0)));
		} else {
			GL_CHECK(glDrawElementsInstanced(aMode, buffer.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0), buffer.instanceCount));
		}

		if (buffer.isTransformFeedbackLoopEnabled) {
			GL_CHECK(glEndTransformFeedback());
			// TODO switch base buffer to newly populated one
			// is there a need to use vbos for the switch?
			int previusTFBO = (currentTFBOIndex + 1) % 2;

			//auto glGeometry = (const_cast<OGLGeometry*> (this));
			
			// ping-pong buffering
			// std::swap( buffer.vaos [currentTFBOIndex], buffer.vaos [previusTFBO]);
			// std::swap( buffer.tfbos[currentTFBOIndex], buffer.tfbos[previusTFBO]);
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
