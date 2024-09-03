#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec3 u_viewPos;
uniform float u_particleScale;

in vec3 g_position[];
in vec3 g_velocity[];
in float g_lifetime[];


out vec2 f_texCoord;
out vec3 f_position;

out vec3 f_position;
out vec3 f_velocity;
out float f_lifetime;

void main() {    
    f_position = g_position[0];
    f_velocity = g_velocity[0];
    f_lifetime = g_lifetime[0];

    vec3 particleToCamera = vec3(gl_in[0].gl_Position) - u_viewPos;

    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.1, 0.0, 0.0) * u_particleScale;
    f_texCoord  = vec2(0, 1);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( -0.1, -0.1, 0.0, 0.0) * u_particleScale;
    f_texCoord  = vec2(0, 0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.1, 0.0, 0.0) * u_particleScale;
    f_texCoord  = vec2(1, 1);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, -0.1, 0.0, 0.0) * u_particleScale;
    f_texCoord  = vec2(1, 0);
    EmitVertex();
    
    EndPrimitive();
}    