#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexIndex;

uniform mat4 u_Transform;

out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = u_Transform * vec4(a_Position, 1.0);
}

#type frag
#version 460 core

in vec2 v_TexCoord;
out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture[32];
uniform int u_UseTexture;

void main()
{
    vec4 color = u_Color;
    if (u_UseTexture == 1)
    {
        int index = int(v_TexIndex);
        color *= texture(u_Texture[index], v_TexCoord);
    }
    
    FragColor = u_Color;
}