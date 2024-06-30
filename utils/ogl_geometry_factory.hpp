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
	OGLGeometry(IndexedBuffer buff) :
		buffer(std::move(buff))
	{}
	IndexedBuffer buffer;

	void bind() const {
		GL_CHECK(glBindVertexArray(buffer.vao.get()));
		if (buffer.isTransformFeedbackLoopEnabled) {
			GL_CHECK(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, buffer.tfb.get()));
		}
	}

	void draw() const {
		draw(buffer.mode);
	}

	void draw(GLenum aMode) const {
		if (buffer.isTransformFeedbackLoopEnabled)
			GL_CHECK(glBeginTransformFeedback(aMode));

		if (buffer.instanceCount == 0) {
			GL_CHECK(glDrawElements(aMode, buffer.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0)));
		} else {
			GL_CHECK(glDrawElementsInstanced(aMode, buffer.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0), buffer.instanceCount));
		}

		if (buffer.isTransformFeedbackLoopEnabled)
			GL_CHECK(glEndTransformFeedback());
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
