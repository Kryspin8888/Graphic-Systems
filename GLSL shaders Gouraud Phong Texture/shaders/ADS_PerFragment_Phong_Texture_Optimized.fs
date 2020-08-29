// Simple Diffuse lighting Shader
// Fragment Shader
#version 130

// INTERPOLOWANE WEJŒCIE z VS (varying)
smooth in vec4 vVaryingColor;

uniform vec4 Light0Specular;
uniform vec4 MaterialSpecular;

// WYJŒCIE
out vec4 vFragColor; // kolor fragmentu

in vec2 vTexCoords;
uniform sampler2D colorMap;

in float diffuseIntensity;
varying vec3 vVaryingNormal;
in vec3 vReflection;

void main( void )
{ 
	
    vFragColor = vVaryingColor; // przepisanie interpolowanego koloru

	float specularIntensity = max( 0.0, dot( normalize( vVaryingNormal ), normalize ( vReflection ) ) );
	float fSpecular = pow(specularIntensity, 128.0);
	vec3 specularColor = fSpecular * Light0Specular.rgb * MaterialSpecular.rgb;

	if(diffuseIntensity > 0.0)
		vFragColor.rgb += specularColor;


	vFragColor *= texture( colorMap, vTexCoords );

}