#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = u_Transform * vec4(a_Position, 1.0);
}


#type frag
#version 460 core

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Texture[32];

void main()
{
    int index = int(v_TexIndex);
    vec4 texColor = texture(u_Texture[index], v_TexCoord);
    FragColor = v_Color * texColor;
}
