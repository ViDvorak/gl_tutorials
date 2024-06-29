#pragma once

#include <vector>
#include "vertex.hpp"
#include "mesh_object.hpp"
#include "ogl_geometry_construction.hpp"
#include "ogl_geometry_factory.hpp"


/*
Recommended approach:
    Take the 05_shaders example and implement the particle object as a new type of scene object in a similar way as the InstancedCube was implemented
    Use instancing for particles, or just buffer of vertices rendered as GL_POINTS and generate the particles in a geometry shader
    Use pregenerated 3D texture for particle animation, or use example 07_noise to generate the texture procedurally.
*/



inline IndexedBuffer generateInstancedParticleSystemBuffers(const std::vector<VertexColor>& aPositionColorAttribs) {
	IndexedBuffer buffers{
		createVertexArray(),
	};

	buffers.vbos.push_back(createBuffer());
	buffers.vbos.push_back(createBuffer());
	buffers.vbos.push_back(createBuffer());

	std::vector<VertexNormTex> vertices;
	std::vector<unsigned int> indices;

	// Generate vertex data
	for (int i = 0; i < 3; ++i) {
		for (int direction = -1; direction < 2; direction += 2) {
			unsigned indexOffset = unsigned(vertices.size());
			for (int j = 0; j < 4; ++j) {
				vertices.push_back(VertexNormTex(
					insertDimension(unitFaceVertices[j], i, direction * 0.5f), // position
					insertDimension(glm::vec2(), i, float(direction)), // normal
					unitFaceVertices[j] + glm::vec2(0.5f, 0.5f))); // texture coordinates
			}
		}
	}

	// Generate indices
	unsigned int index = 0;
	for (auto& vertex : vertices) {
		indices.push_back(index++);
	}

	// Bind VAO 
	GL_CHECK(glBindVertexArray(buffers.vao.get()));

	// Bind VBO with vertex data
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers.vbos[0].get()));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormTex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

	// Bind EBO
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbos[1].get()));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW));

	// Vertex attributes
	// Position attribute
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex), (void*)0));
	GL_CHECK(glEnableVertexAttribArray(0));

	// Normal attribute
	GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex), (void*)(sizeof(glm::vec3))));
	GL_CHECK(glEnableVertexAttribArray(1));

	// Texture coordinate attribute
	GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex), (void*)(2 * sizeof(glm::vec3))));
	GL_CHECK(glEnableVertexAttribArray(2));

	// Instance attributes
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers.vbos[2].get()));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * aPositionColorAttribs.size(), aPositionColorAttribs.data(), GL_STATIC_DRAW));

	// Instance position attribute
	GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)(0)));
	GL_CHECK(glEnableVertexAttribArray(3));
	GL_CHECK(glVertexAttribDivisor(3, 1));

	// Instance color attribute
	GL_CHECK(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)(sizeof(glm::vec3))));
	GL_CHECK(glEnableVertexAttribArray(4));
	GL_CHECK(glVertexAttribDivisor(4, 1));

	// Unbind VAO
	GL_CHECK(glBindVertexArray(0));

	buffers.indexCount = unsigned(indices.size());
	buffers.instanceCount = unsigned(aPositionColorAttribs.size());
	buffers.mode = GL_POINTS;

	return buffers;
}




inline IndexedBuffer OLDgenerateInstancedParticleSystemBuffers(const std::vector<VertexColor>& aPositionColorAttribs) {
	IndexedBuffer buffers{
		createVertexArray(),
	};
	buffers.vbos.push_back(createBuffer());
	buffers.vbos.push_back(createBuffer());
	buffers.vbos.push_back(createBuffer());

	std::vector<VertexNormTex> vertices;
	std::vector<unsigned int> indices;

	// TODO insted of vertices pass to the VBO only centers of the particles
	// TODO in vertex shader move particles in direction of theier velocity potencialy with some randomization and lower the velocity
	// TODO in geometry shader create vertises plane where normal looks at camera
	// TODO fragment shader use particle 

	// vertex data to render generation from center points of cubes
	for (int i = 0; i < 3; ++i) {
		for (int direction = -1; direction < 2; direction += 2) {

			// VBO data definition
			unsigned indexOffset = unsigned(vertices.size());
			for (int j = 0; j < 4; ++j) {
				vertices.push_back(VertexNormTex(
					insertDimension(unitFaceVertices[j], i, direction * 0.5f), // position
					insertDimension(glm::vec2(), i, float(direction)), // normal
					unitFaceVertices[j] + glm::vec2(0.5f, 0.5f))); // texture coordinates
			}

			// old EBO data definition
			/*for (auto index : faceTriangleIndices) {
				indices.push_back(index + indexOffset);
			}*/
		}
	}
	// new EBO data definition
	unsigned int index = 0;
	for (auto vertex : vertices) {
		indices.push_back(index++);
	}

	// bind VAO 
	GL_CHECK(glBindVertexArray(buffers.vao.get()));

	// bind VBO with vertex data
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers.vbos[0].get()));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormTex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

	// bind EBO
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbos[1].get()));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW));



	// Position attribute
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex), (void*)0));
	GL_CHECK(glEnableVertexAttribArray(0));

	// Normal attribute
	GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex), (void*)(sizeof(glm::vec3))));
	GL_CHECK(glEnableVertexAttribArray(1));
	
	// Texture coordinate attribute
	GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex), (void*)(2 * sizeof(glm::vec3)))); // last parameter is offset
	GL_CHECK(glEnableVertexAttribArray(2));



	//		glVertexAtribPointer(index, size, type, normalized, stride, pointer)

	// INSTANCE ATTRIBUTES

	// that means position and color of each particle

	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers.vbos[2].get()));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * aPositionColorAttribs.size(), aPositionColorAttribs.data(), GL_STATIC_DRAW));

	GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)(0))); // vec3 position
	GL_CHECK(glEnableVertexAttribArray(3));
	GL_CHECK(glVertexAttribDivisor(3, 1)); // 1 means the attribute advances once per instance

	GL_CHECK(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)(sizeof(glm::vec3)))); // vec3 color
	GL_CHECK(glEnableVertexAttribArray(4));
	GL_CHECK(glVertexAttribDivisor(4, 1)); // 1 means the attribute advances once per instance

	// Unbind VAO
	GL_CHECK(glBindVertexArray(0));

	buffers.indexCount = unsigned(indices.size());
	buffers.instanceCount = unsigned(aPositionColorAttribs.size());
	buffers.mode = GL_POINTS;
	return buffers;
}


class InstancedParticleSystem : public MeshObject {
public:
	InstancedParticleSystem(std::vector<VertexColor> aInstanceAttributes)
		: mInstanceAttributes(std::move(aInstanceAttributes))
	{}

	virtual std::shared_ptr<AGeometry> getGeometry(GeometryFactory& aGeometryFactory, RenderStyle aRenderStyle) {
		return std::make_shared<OGLGeometry>(generateInstancedParticleSystemBuffers(mInstanceAttributes));
	}

	void prepareRenderData(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) override {
		for (auto& mode : mRenderInfos) {
			mode.second.shaderProgram = aMaterialFactory.getShaderProgram(mode.second.materialParams.mMaterialName);
			getTextures(mode.second.materialParams.mParameterValues, aMaterialFactory);
			mode.second.geometry = getGeometry(aGeometryFactory, mode.second.materialParams.mRenderStyle);
		}
	}

protected:
	std::vector<VertexColor> mInstanceAttributes;
};