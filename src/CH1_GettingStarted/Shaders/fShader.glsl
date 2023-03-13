#version 330 core
out vec4 fragColor;

in vec2 outTexCoord;

uniform sampler2D chosenTexture;
uniform sampler2D chosenTexture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	fragColor = mix(texture(chosenTexture, outTexCoord), texture(chosenTexture2, outTexCoord), 0.2);
	
}
