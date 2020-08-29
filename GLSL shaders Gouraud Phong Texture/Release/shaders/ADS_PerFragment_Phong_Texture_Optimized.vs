// Simple Diffuse lighting Shader
// Vertex Shader
#version 130

// PARAMETRY WIERZCHO£KA
in vec4 vVertex; // wspó³rzêdne wierzcho³ka
in vec3 vNormal; // wektor normalny powierzchni w wierzcho³ka

uniform vec4 Light0Ambient; // sk³adowa ambient œwiat³a 0
uniform vec4 Light0Diffuse; // sk³adowa diffuse œwiat³a 0
uniform vec4 Light0Specular;
uniform vec4 MaterialAmbient; // sk³adowa ambient materia³u
uniform vec4 MaterialDiffuse; // sk³adowa diffuse materia³u
uniform vec4 MaterialSpecular;

uniform vec3 vLightPosition; // pozycja œwiat³a
uniform mat4 mvpMatrix; // macierz Model-View-Projection
uniform mat4 mvMatrix; // macierz Model-View
uniform mat3 normalMatrix; // macierz przekszta³cenia wektora normalnego

// WYJŒCIE PROGRAMU WIERZCHO£KA (varying)

in vec4 vTexture0;
out vec2 vTexCoords;
smooth out vec4 vVaryingColor;

out float diffuseIntensity;
varying vec3 vVaryingNormal;
out vec3 vReflection;

void main( void ) 
{ 
	vTexCoords = vTexture0.st;
	
	vVaryingNormal = normalMatrix * vNormal; // normalna powierzchni w uk³adzie wspó³rzêdnych oka (kamery)

	vec4 vPosition4 = mvMatrix * vVertex; // wspó³rzêdne wierzcho³ka w uk³adzie wspó³rzêdnych oka (kamery)

	vec3 vVaryingLightDir = normalize( vLightPosition - vPosition4.xyz / vPosition4.w ); // wektor œwiat³a w uk³adzie wspó³rzêdnych oka (kamery)
	
	vec3 ambientColor = Light0Ambient.rgb * MaterialAmbient.rgb;  // sk³adowa ambient

	diffuseIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize( vVaryingLightDir ) ) ); // intensywnoœæ oœwietlenia na podstawie prawa Lamberta
	vec3 diffuseColor = diffuseIntensity * Light0Diffuse.rgb * MaterialDiffuse.rgb; // kolor œwiat³a rozproszonego na obiekcie

	vReflection = normalize( reflect( -vVaryingLightDir, vVaryingNormal ) );

	vVaryingColor.rgb =  ambientColor + diffuseColor;
	vVaryingColor.a = MaterialDiffuse.a;

	gl_Position = mvpMatrix * vVertex; // po³o¿enie wierzcho³ka w przestrzeni obcinania (przemno¿enie przez macierz rzutowania)
}