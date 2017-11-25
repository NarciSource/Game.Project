#version 460 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform sampler2D tex;

uniform vec4 cameraPosition;
uniform vec4 lightPosition;
uniform vec4 colorValue;

uniform bool flagTex;

in vec4 vertexPosition;
in vec4 vertexNormal;
in vec2 vertexUV;


out vec4 fragColor;
out vec4 fragNormal;
out vec4 fragPosition;
out vec2 fragUV;

void main () {
    fragNormal	= vertexNormal;
	fragColor	= colorValue;
	fragUV		= vertexUV;

    vec4 WorldPosition	= modelMatrix * vertexPosition;
    fragPosition		= WorldPosition;
	
    gl_Position  = projectionMatrix * viewMatrix * WorldPosition;
}
