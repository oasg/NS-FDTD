#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
uniform float aspect_ratio;



void main()
{
    gl_Position = vec4((aPos.x * aspect_ratio-0.3)*1.5, aPos.y*1.5, aPos.z, 1.0);
    TexCoord = aTexCoord;
}