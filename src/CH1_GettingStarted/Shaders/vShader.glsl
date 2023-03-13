#version 330 core
layout (location = 0) in vec3 attrb_Pos;
layout (location = 1) in vec3 attrb_Color;
layout (location = 2) in vec2 attrb_TexCoord;

out vec3 chosenColor;
out vec2 outTexCoord;

uniform mat4 transformMatrix;

void main()
{
	gl_Position = transformMatrix * vec4(attrb_Pos, 1.0);
	chosenColor = attrb_Color;
	outTexCoord = vec2(attrb_TexCoord.x, attrb_TexCoord.y);
}
