// Simple Diffuse lighting Shader
// Fragment Shader
#version 130

// INTERPOLOWANE WEJ�CIE z VS (varying)
smooth in vec4 vVaryingColor;

uniform vec4 Light0Ambient; // sk�adowa ambient �wiat�a 0
uniform vec4 Light0Diffuse; // sk�adowa diffuse �wiat�a 0
uniform vec4 Light0Specular;
uniform vec4 MaterialAmbient; // sk�adowa ambient materia�u
uniform vec4 MaterialDiffuse; // sk�adowa diffuse materia�u
uniform vec4 MaterialSpecular;

in vec3 vVaryingNormal; 
in vec3 vVaryingLightDir;

// WYJ�CIE
out vec4 vFragColor; // kolor fragmentu

void main( void )
{ 
	vec3 ambientColor = Light0Ambient.rgb * MaterialAmbient.rgb;  // sk�adowa ambient

	float diffuseIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize( vVaryingLightDir ) ) ); // intensywno�� o�wietlenia na podstawie prawa Lamberta
	vec3 diffuseColor = diffuseIntensity * Light0Diffuse.rgb * MaterialDiffuse.rgb; // kolor �wiat�a rozproszonego na obiekcie

   vFragColor.rgb =  ambientColor + diffuseColor; 
   vFragColor.a = MaterialDiffuse.a;

   vec3 vReflection = normalize( reflect( -vVaryingLightDir, vVaryingNormal ) );
	float specularIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize ( vReflection ) ) );
	float fSpecular = pow(specularIntensity, 128.0);
	vec3 specularColor = fSpecular * Light0Specular.rgb * MaterialSpecular.rgb;

	if(diffuseIntensity > 0.0)
		vFragColor.rgb += specularColor;

}