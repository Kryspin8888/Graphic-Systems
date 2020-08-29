// Simple Diffuse lighting Shader
// Vertex Shader
#version 130

// PARAMETRY WIERZCHO�KA
in vec4 vVertex; // wsp�rz�dne wierzcho�ka
in vec3 vNormal; // wektor normalny powierzchni w wierzcho�ka

uniform vec3 vLightPosition; // pozycja �wiat�a
uniform mat4 mvpMatrix; // macierz Model-View-Projection
uniform mat4 mvMatrix; // macierz Model-View
uniform mat3 normalMatrix; // macierz przekszta�cenia wektora normalnego

// WYJ�CIE PROGRAMU WIERZCHO�KA (varying)
varying vec3 vVaryingNormal; 
varying vec3 vVaryingLightDir;

in vec4 vTexture0;
out vec2 vTexCoords;

uniform float objectHeight = 1.0;

void main( void ) 
{ 
	vTexCoords = vTexture0.st;
	
	vVaryingNormal = normalMatrix * vNormal; // normalna powierzchni w uk�adzie wsp�rz�dnych oka (kamery)

	vec4 vPosition4 = mvMatrix * vVertex; // wsp�rz�dne wierzcho�ka w uk�adzie wsp�rz�dnych oka (kamery)

	vVaryingLightDir = normalize( vLightPosition - vPosition4.xyz / vPosition4.w ); // wektor �wiat�a w uk�adzie wsp�rz�dnych oka (kamery)
	
	gl_Position = mvpMatrix * vVertex; // po�o�enie wierzcho�ka w przestrzeni obcinania (przemno�enie przez macierz rzutowania)
	gl_Position.y *= objectHeight;
}