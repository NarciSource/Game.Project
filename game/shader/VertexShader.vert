#version 460 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


uniform vec4 cameraPosition;
uniform vec4 lightPosition;
uniform vec4 colorValue;
uniform int shadingType;

in vec4 vertexPosition;
in vec4 vertexNormal;


out vec4 fragColor;
out vec4 fragNormal;
out vec4 fragPosition;

void main () {
    fragNormal	 = vertexNormal;

    vec4 WorldPosition = modelMatrix * vertexPosition;
    fragPosition = WorldPosition;

    gl_Position  = projectionMatrix * viewMatrix * WorldPosition;



	if(shadingType == 0x01)	// Gouraud shading
	{

		vec4 L = lightPosition - fragPosition;
		vec4 V = cameraPosition - fragPosition;
		vec4 N = fragNormal;

		L.w = 0.0; L=normalize(L);
		V.w = 0.0; V=normalize(V);
		N.w = 0.0; N=normalize(N);

		float ambient = 1.0;
		float diffusion = max(0.0, dot(L, N));
		float specular = max(0.0, dot(((2.0 * dot(L,fragNormal) *N) - L), V));

		float shininess = 90.0;

		specular = pow(specular, shininess);

		float ac = 0.3;
		float dc = 0.8;
		float sc = 0.2;

		fragColor = (ac * ambient + dc * diffusion + sc * specular) * colorValue;
	}
	if(shadingType == 0x02)	// Phong shading
	{
		fragColor = colorValue;
	}
	if(shadingType == 0x03) // Don't shading
	{
		fragColor = colorValue;
	}

}
