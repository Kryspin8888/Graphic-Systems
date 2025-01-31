// SG_lab2 23-11-2016
// fog.vs
// Zastąpienie podstawowej funkcjonalności stałego potoku renderowania

uniform vec3 lightPos[ 3 ]; // maksymalnie trzy położenia światła
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory światła

void main( void )
{
   // transformacja wierzchołka zgodnie z aktualną macierzą widoku modelu i rzutowania
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // skopiowanie koloru wierzchołka (primary color)
   gl_FrontColor = gl_Color;
}
