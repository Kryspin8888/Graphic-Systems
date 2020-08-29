// Simple Diffuse lighting Shader
// Vertex Shader
#version 130

// PARAMETRY WIERZCHO�KA
in vec4 vVertex; // wsp�rz�dne wierzcho�ka
in vec3 vNormal; // wektor normalny powierzchni w wierzcho�ka

uniform vec4 Light0Ambient; // sk�adowa ambient �wiat�a 0
uniform vec4 Light0Diffuse; // sk�adowa diffuse �wiat�a 0
uniform vec4 Light0Specular;
uniform vec4 MaterialAmbient; // sk�adowa ambient materia�u
uniform vec4 MaterialDiffuse; // sk�adowa diffuse materia�u
uniform vec4 MaterialSpecular;

uniform vec3 vLightPosition; // pozycja �wiat�a
uniform mat4 mvpMatrix; // macierz Model-View-Projection
uniform mat4 mvMatrix; // macierz Model-View
uniform mat3 normalMatrix; // macierz przekszta�cenia wektora normalnego

// WYJ�CIE PROGRAMU WIERZCHO�KA (varying)

in vec4 vTexture0;
out vec2 vTexCoords;
smooth out vec4 vVaryingColor;

out float diffuseIntensity;
varying vec3 vVaryingNormal;
out vec3 vReflection;

void main( void ) 
{ 
	vTexCoords = vTexture0.st;
	
	vVaryingNormal = normalMatrix * vNormal; // normalna powierzchni w uk�adzie wsp�rz�dnych oka (kamery)

	vec4 vPosition4 = mvMatrix * vVertex; // wsp�rz�dne wierzcho�ka w uk�adzie wsp�rz�dnych oka (kamery)

	vec3 vVaryingLightDir = normalize( vLightPosition - vPosition4.xyz / vPosition4.w ); // wektor �wiat�a w uk�adzie wsp�rz�dnych oka (kamery)
	
	vec3 ambientColor = Light0Ambient.rgb * MaterialAmbient.rgb;  // sk�adowa ambient

	diffuseIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize( vVaryingLightDir ) ) ); // intensywno�� o�wietlenia na podstawie prawa Lamberta
	vec3 diffuseColor = diffuseIntensity * Light0Diffuse.rgb * MaterialDiffuse.rgb; // kolor �wiat�a rozproszonego na obiekcie

	vReflection = normalize( reflect( -vVaryingLightDir, vVaryingNormal ) );

	vVaryingColor.rgb =  ambientColor + diffuseColor;
	vVaryingColor.a = MaterialDiffuse.a;

	gl_Position = mvpMatrix * vVertex; // po�o�enie wierzcho�ka w przestrzeni obcinania (przemno�enie przez macierz rzutowania)
}