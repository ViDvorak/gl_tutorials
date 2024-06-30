#pragma once


#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
};


struct VertexNorm {
	glm::vec3 position;
	glm::vec3 normal;
};

struct VertexColor {
	glm::vec3 position;
	glm::vec3 color;
};


struct VertexTex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

struct VertexNormTex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};



struct VertexVelocityLife {
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
};

struct VertexVelocityInitLife {
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
	float lifeDelay;

	explicit operator VertexVelocityLife() const {
		return VertexVelocityLife(position, velocity, lifetime);
	}
};


struct VertexVelocity {
	glm::vec3 position;
	glm::vec3 velocity;
};