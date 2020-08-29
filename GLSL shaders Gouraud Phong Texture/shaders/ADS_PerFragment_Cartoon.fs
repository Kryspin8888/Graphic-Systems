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

uniform sampler1D colorTable;

void main( void )
{ 
	
	vec3 color2;

	vec3 ambientColor = Light0Ambient.rgb * MaterialAmbient.rgb;  // sk�adowa ambient

	float diffuseIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize( vVaryingLightDir ) ) ); // intensywno�� o�wietlenia na podstawie prawa Lamberta
	vec3 diffuseColor = diffuseIntensity * Light0Diffuse.rgb * MaterialDiffuse.rgb; // kolor �wiat�a rozproszonego na obiekcie

   vFragColor.rgb =  ambientColor + diffuseColor; 
   vFragColor.a = MaterialDiffuse.a;


	if (diffuseIntensity > 0.95)      
		color2 = vec3(1.0, 1.0, 1.0);
    else if (diffuseIntensity > 0.75) 
		color2 = vec3(0.8, 0.8, 0.8);
    else if (diffuseIntensity > 0.50) 
		color2 = vec3(0.6, 0.6, 0.6);
    else if (diffuseIntensity > 0.25) 
		color2 = vec3(0.4, 0.4, 0.4);
    else                       
		color2 = vec3(0.2, 0.2, 0.2);

	vFragColor.rgb *= color2;

}