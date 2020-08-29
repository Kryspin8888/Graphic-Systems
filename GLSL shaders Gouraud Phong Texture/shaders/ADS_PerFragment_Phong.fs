// Simple Diffuse lighting Shader
// Fragment Shader
#version 130

// INTERPOLOWANE WEJŒCIE z VS (varying)
smooth in vec4 vVaryingColor;

uniform vec4 Light0Ambient; // sk³adowa ambient œwiat³a 0
uniform vec4 Light0Diffuse; // sk³adowa diffuse œwiat³a 0
uniform vec4 Light0Specular;
uniform vec4 MaterialAmbient; // sk³adowa ambient materia³u
uniform vec4 MaterialDiffuse; // sk³adowa diffuse materia³u
uniform vec4 MaterialSpecular;

in vec3 vVaryingNormal; 
in vec3 vVaryingLightDir;

// WYJŒCIE
out vec4 vFragColor; // kolor fragmentu

void main( void )
{ 
	vec3 ambientColor = Light0Ambient.rgb * MaterialAmbient.rgb;  // sk³adowa ambient

	float diffuseIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize( vVaryingLightDir ) ) ); // intensywnoœæ oœwietlenia na podstawie prawa Lamberta
	vec3 diffuseColor = diffuseIntensity * Light0Diffuse.rgb * MaterialDiffuse.rgb; // kolor œwiat³a rozproszonego na obiekcie

   vFragColor.rgb =  ambientColor + diffuseColor; 
   vFragColor.a = MaterialDiffuse.a;

   vec3 vReflection = normalize( reflect( -vVaryingLightDir, vVaryingNormal ) );
	float specularIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize ( vReflection ) ) );
	float fSpecular = pow(specularIntensity, 128.0);
	vec3 specularColor = fSpecular * Light0Specular.rgb * MaterialSpecular.rgb;

	if(diffuseIntensity > 0.0)
		vFragColor.rgb += specularColor;

}