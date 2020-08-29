// Simple Diffuse lighting Shader
// Vertex Shader
#version 130

// PARAMETRY WIERZCHO£KA
in vec4 vVertex; // wspó³rzêdne wierzcho³ka
in vec3 vNormal; // wektor normalny powierzchni w wierzcho³ka

uniform vec3 vLightPosition; // pozycja œwiat³a
uniform mat4 mvpMatrix; // macierz Model-View-Projection
uniform mat4 mvMatrix; // macierz Model-View
uniform mat3 normalMatrix; // macierz przekszta³cenia wektora normalnego

// WYJŒCIE PROGRAMU WIERZCHO£KA (varying)
varying vec3 vVaryingNormal; 
varying vec3 vVaryingLightDir;

in vec4 vTexture0;
out vec2 vTexCoords;

uniform float objectHeight = 1.0;

void main( void ) 
{ 
	vTexCoords = vTexture0.st;
	
	vVaryingNormal = normalMatrix * vNormal; // normalna powierzchni w uk³adzie wspó³rzêdnych oka (kamery)

	vec4 vPosition4 = mvMatrix * vVertex; // wspó³rzêdne wierzcho³ka w uk³adzie wspó³rzêdnych oka (kamery)

	vVaryingLightDir = normalize( vLightPosition - vPosition4.xyz / vPosition4.w ); // wektor œwiat³a w uk³adzie wspó³rzêdnych oka (kamery)
	
	gl_Position = mvpMatrix * vVertex; // po³o¿enie wierzcho³ka w przestrzeni obcinania (przemno¿enie przez macierz rzutowania)
	gl_Position.y *= objectHeight;
}