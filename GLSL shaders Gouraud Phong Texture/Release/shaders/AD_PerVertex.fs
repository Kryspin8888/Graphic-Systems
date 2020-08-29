// Simple Diffuse lighting Shader
// Fragment Shader
#version 130

// INTERPOLOWANE WEJŒCIE z VS (varying)
smooth in vec4 vVaryingColor;

// WYJŒCIE
out vec4 vFragColor; // kolor fragmentu

void main( void )
{ 
   vFragColor = vVaryingColor; // przepisanie interpolowanego koloru
}