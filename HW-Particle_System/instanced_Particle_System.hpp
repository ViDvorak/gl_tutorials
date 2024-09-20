#pragma once

#include <vector>
#include "vertex.hpp"
#include "mesh_object.hpp"
#include "ogl_geometry_construction.hpp"
#include "ogl_geometry_factory.hpp"
#include "ogl_material_factory.hpp"


/*
Recommended approach:
    Take the 05_shaders example and implement the particle object as a new type of scene object in a similar way as the InstancedCube was implemented
    Use instancing for particles, or just buffer of vertices rendered as GL_POINTS and generate the particles in a geometry shader
    Use pregenerated 3D texture for particle animation, or use example 07_noise to generate the texture procedurally.
*/


inline IndexedBuffer generateInstancedParticleSystemBuffers(const std::vector<VertexVelocityInitLife>& aPositionColorAttribs, unsigned int aParticleCount, OpenGLResource&& tfb) {
	IndexedBuffer buffers(
		{ createVertexArray(), createVertexArray() },
		{ createBuffer(), createBuffer() }
	);
	buffers.vbos.push_back(createBuffer());
	buffers.vbos.push_back(createBuffer());
	buffers.vbos.push_back(createBuffer());

	buffers.isTransformFeedbackLoopEnabled = true;


	std::vector<VertexVelocityLife> vertices;
	std::vector<GLuint> indices;

	
	for (VertexVelocityInitLife vertex : aPositionColorAttribs) {
		// load changing parameters 4x because of transform feedback captures data after the geometry shader per EmitedVertex.
		vertices.emplace_back(std::move((VertexVelocityLife)vertex));
		vertices.emplace_back(std::move((VertexVelocityLife)vertex));
		vertices.emplace_back(std::move((VertexVelocityLife)vertex));
		vertices.emplace_back(std::move((VertexVelocityLife)vertex));
	}

	// new EBO data definition
	//for (unsigned int i = 0; i < aParticleCount; ++i) {
	//	 indices.push_back(i);
	//}
	indices.push_back(0);


	std::cout << "buffers size: " << buffers.vaos.size() << std::endl;
	std::cout.flush();

	for (int i = 0; i < 2; ++i) {
		// bind VAO
		GL_CHECK(glBindVertexArray(buffers.vaos[i].get()));

		// bind VBO with vertex data
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers.tfbos[i].get()));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexVelocityLife) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

		// bind EBO
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbos[i].get()));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW));



		// asociated with current VBO [1]
		// current position
		GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexVelocityLife) * 4, (void*)0));
		GL_CHECK(glEnableVertexAttribArray(0));
		GL_CHECK(glVertexAttribDivisor(0, 1));

		// current velocity
		GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexVelocityLife) * 4, (void*)(sizeof(glm::vec3))));
		GL_CHECK(glEnableVertexAttribArray(1));
		GL_CHECK(glVertexAttribDivisor(1, 1));
	
		// elapsed timeOfLife of the particel
		GL_CHECK(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(VertexVelocityLife) * 4, (void*)(2 * sizeof(glm::vec3)))); // last parameter is offset
		GL_CHECK(glEnableVertexAttribArray(2));
		GL_CHECK(glVertexAttribDivisor(2, 1));




		// INSTANCE ATTRIBUTES
		// position and color of each particle

		// Bind VBO
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers.vbos[2].get()));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexVelocityInitLife) * aPositionColorAttribs.size(), aPositionColorAttribs.data(), GL_STATIC_DRAW));

		// initial position
		GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexVelocityInitLife), (void*)(0)));
		GL_CHECK(glEnableVertexAttribArray(3));
		GL_CHECK(glVertexAttribDivisor(3, 1));

		// initial velocity
		GL_CHECK(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexVelocityInitLife), (void*)(sizeof(glm::vec3))));
		GL_CHECK(glEnableVertexAttribArray(4));
		GL_CHECK(glVertexAttribDivisor(4, 1));

		// time of a particle life
		GL_CHECK(glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(VertexVelocityInitLife), (void*)(2 * sizeof(glm::vec3))));
		GL_CHECK(glEnableVertexAttribArray(5));
		GL_CHECK(glVertexAttribDivisor(5, 1));

		// initial lifeDelay
		GL_CHECK(glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(VertexVelocityInitLife), (void*)(2 * (sizeof(glm::vec3) + sizeof(float) )) ));
		GL_CHECK(glEnableVertexAttribArray(6));
		GL_CHECK(glVertexAttribDivisor(6, 1));

		// Unbind VAO
		GL_CHECK(glBindVertexArray(0));

	}


	// TODO Settup transform feedback buffer
	// bind transform feedback buffer
	// GL_CHECK(glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffers.vbos[3].get()));
	// GL_CHECK(glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(VertexVelocityLife) * vertices.size(), nullptr, GL_STATIC_READ));
	// GL_CHECK(glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffers.vbos[3].get()));



	buffers.indexCount = unsigned(1);
	buffers.instanceCount = unsigned(aParticleCount);
	buffers.mode = GL_POINTS;
	return buffers;
}


class InstancedParticleSystem : public MeshObject {
public:
	InstancedParticleSystem(std::vector<VertexVelocityInitLife> aInstanceAttributes, unsigned int aParticleCount)
		: mInstanceAttributes(std::move(aInstanceAttributes)), mParticleCount(aParticleCount)
	{}

	virtual std::shared_ptr<AGeometry> getGeometry(GeometryFactory& aGeometryFactory, RenderStyle aRenderStyle) {
		return std::make_shared<OGLGeometry>(generateInstancedParticleSystemBuffers(mInstanceAttributes, mParticleCount, std::move(tfb)));
	}

	void prepareRenderData(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) override {
		for (auto& mode : mRenderInfos) {
			mode.second.shaderProgram = aMaterialFactory.getShaderProgram(mode.second.materialParams.mMaterialName);

			getTextures(mode.second.materialParams.mParameterValues, aMaterialFactory);
			mode.second.geometry = getGeometry(aGeometryFactory, mode.second.materialParams.mRenderStyle);

			// Unbind TFO
			GL_CHECK(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0));
		}
	}

	void setFeedbackParameters( std::vector<GLchar*>&& parameters) {
		mFeedbackParameterNames = std::move(parameters);
	}

protected:
	std::vector<VertexVelocityInitLife> mInstanceAttributes;
	std::vector<GLchar*> mFeedbackParameterNames;
	OpenGLResource tfb = createTransformFeedback();
	unsigned int mParticleCount;
};