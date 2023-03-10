#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoordinate;

out vec3 ourColor;
out vec2 TexCoord;

uniform vec4 texture_coords[4];


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 offset;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTextureCoordinate;
}