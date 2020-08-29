// Simple Diffuse lighting Shader
// Vertex Shader
#version 130

// PARAMETRY WIERZCHO£KA
in vec4 vVertex; // wspó³rzêdne wierzcho³ka
in vec3 vNormal; // wektor normalny powierzchni w wierzcho³ka

// PARAMETRY OBIEKTU
uniform vec4 Light0Ambient; // sk³adowa ambient œwiat³a 0
uniform vec4 Light0Diffuse; // sk³adowa diffuse œwiat³a 0
uniform vec4 MaterialAmbient; // sk³adowa ambient materia³u
uniform vec4 MaterialDiffuse; // sk³adowa diffuse materia³u

uniform vec3 vLightPosition; // pozycja œwiat³a
uniform mat4 mvpMatrix; // macierz Model-View-Projection
uniform mat4 mvMatrix; // macierz Model-View
uniform mat3 normalMatrix; // macierz przekszta³cenia wektora normalnego

// WYJŒCIE PROGRAMU WIERZCHO£KA (varying)
smooth out vec4 vVaryingColor; // kolor wierzcho³ka

void main( void ) 
{ 
	vec3 vEyeNormal = normalMatrix * vNormal; // normalna powierzchni w uk³adzie wspó³rzêdnych oka (kamery)

	vec4 vPosition4 = mvMatrix * vVertex; // wspó³rzêdne wierzcho³ka w uk³adzie wspó³rzêdnych oka (kamery)

	vec3 vLightDir = normalize( vLightPosition - vPosition4.xyz / vPosition4.w ); // wektor œwiat³a w uk³adzie wspó³rzêdnych oka (kamery)
	
	vec3 ambientColor = Light0Ambient.rgb * MaterialAmbient.rgb;  // sk³adowa ambient

	float diffuseIntensity = max( 0.0, dot( vEyeNormal, vLightDir ) ); // intensywnoœæ oœwietlenia na podstawie prawa Lamberta
	vec3 diffuseColor = diffuseIntensity * Light0Diffuse.rgb * MaterialDiffuse.rgb; // kolor œwiat³a rozproszonego na obiekcie

	vVaryingColor.rgb =  ambientColor + diffuseColor; // kolor wierzcho³ka uwzglêdniaj¹cy oœwietlenie otaczaj¹ce oraz rozproszone
	vVaryingColor.a = MaterialDiffuse.a; // sk³adowa przezroczystoœci

	gl_Position = mvpMatrix * vVertex; // po³o¿enie wierzcho³ka w przestrzeni obcinania (przemno¿enie przez macierz rzutowania)
}