#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_Transform * vec4(a_Position, 1.0);
}

#type frag
#version 330 core

in vec2 v_TexCoord;
out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_UseTexture;

void main()
{
    if (u_UseTexture == 1)
        FragColor = texture(u_Texture, v_TexCoord) * u_Color;
    else
        FragColor = u_Color;
}