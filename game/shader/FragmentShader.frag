#version 460 core

uniform vec4 cameraPosition;
uniform vec4 lightPosition;
uniform bool flagTex;
uniform bool flagShading;

uniform sampler2D tex;


in vec4 fragColor;
in vec4 fragNormal;
in vec4 fragPosition;
in vec2 fragUV;

out vec4 fragmentColor;

void main() {

	if(flagShading)
	{
		// Phong shading
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

		fragmentColor = (ac * ambient + dc * diffusion + sc * specular) * fragColor;
	}
	else {
		fragmentColor = fragColor;
	}



	// enable texture
	if(flagTex)
		fragmentColor = texture(tex,fragUV) * fragmentColor;
	else
		fragmentColor = fragmentColor;
}

