#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec3 u_viewPos;
uniform float u_particleScale;

in vec3 g_position[];
//in vec3 g_normal[];
//in vec3 g_color[];

//out vec3 f_normal;
//out vec3 f_color;
out vec3 f_position;
out vec2 f_texCoord;

void main() {    
    vec3 particleToCamera = vec3(gl_in[0].gl_Position) - u_viewPos;
    
    
    //f_normal = g_normal[0];
    f_position = g_position[0];
    //f_color = g_color[0];

    
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