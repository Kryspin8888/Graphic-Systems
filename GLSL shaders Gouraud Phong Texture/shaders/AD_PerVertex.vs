// Simple Diffuse lighting Shader
// Vertex Shader
#version 130

// PARAMETRY WIERZCHO�KA
in vec4 vVertex; // wsp�rz�dne wierzcho�ka
in vec3 vNormal; // wektor normalny powierzchni w wierzcho�ka

// PARAMETRY OBIEKTU
uniform vec4 Light0Ambient; // sk�adowa ambient �wiat�a 0
uniform vec4 Light0Diffuse; // sk�adowa diffuse �wiat�a 0
uniform vec4 MaterialAmbient; // sk�adowa ambient materia�u
uniform vec4 MaterialDiffuse; // sk�adowa diffuse materia�u

uniform vec3 vLightPosition; // pozycja �wiat�a
uniform mat4 mvpMatrix; // macierz Model-View-Projection
uniform mat4 mvMatrix; // macierz Model-View
uniform mat3 normalMatrix; // macierz przekszta�cenia wektora normalnego

// WYJ�CIE PROGRAMU WIERZCHO�KA (varying)
smooth out vec4 vVaryingColor; // kolor wierzcho�ka

void main( void ) 
{ 
	vec3 vEyeNormal = normalMatrix * vNormal; // normalna powierzchni w uk�adzie wsp�rz�dnych oka (kamery)

	vec4 vPosition4 = mvMatrix * vVertex; // wsp�rz�dne wierzcho�ka w uk�adzie wsp�rz�dnych oka (kamery)

	vec3 vLightDir = normalize( vLightPosition - vPosition4.xyz / vPosition4.w ); // wektor �wiat�a w uk�adzie wsp�rz�dnych oka (kamery)
	
	vec3 ambientColor = Light0Ambient.rgb * MaterialAmbient.rgb;  // sk�adowa ambient

	float diffuseIntensity = max( 0.0, dot( vEyeNormal, vLightDir ) ); // intensywno�� o�wietlenia na podstawie prawa Lamberta
	vec3 diffuseColor = diffuseIntensity * Light0Diffuse.rgb * MaterialDiffuse.rgb; // kolor �wiat�a rozproszonego na obiekcie

	vVaryingColor.rgb =  ambientColor + diffuseColor; // kolor wierzcho�ka uwzgl�dniaj�cy o�wietlenie otaczaj�ce oraz rozproszone
	vVaryingColor.a = MaterialDiffuse.a; // sk�adowa przezroczysto�ci

	gl_Position = mvpMatrix * vVertex; // po�o�enie wierzcho�ka w przestrzeni obcinania (przemno�enie przez macierz rzutowania)
}