#version 430 core

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat3 u_normalMat;
uniform vec3 u_gravityDirection;
uniform float u_elapsedTime;
uniform float u_deltaTime;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_velocity;
layout(location = 2) in float in_lifetime;

layout(location = 3) in vec3 in_initPosition;
layout(location = 4) in vec3 in_initVelocity;
layout(location = 5) in float in_particleLife;
layout(location = 6) in float in_particleDelay;

out vec3 g_position;
out vec3 g_velocity;
out float g_lifetime;

// TODO move point in space

void main(void)
{
	g_lifetime = in_lifetime + u_deltaTime;
	float elapsedLife = mod( in_lifetime - in_particleDelay, in_particleLife);

	if (elapsedLife < u_deltaTime)
	{
		// respawn particle
		g_position = in_initPosition;
		gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_initPosition, 1);

		g_velocity = in_initVelocity;
	}

	else
	{
		g_position = in_position + in_velocity * u_deltaTime;
		gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(g_position, 1);
	
		g_velocity = in_velocity - u_gravityDirection;
	}
}