// SG_lab2 23-11-2016
// params.vs
// 

uniform vec3 lightPos[ 3 ]; // maksymalnie trzy po�o�enia �wiat�a
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory �wiat�a
varying float distance;

void main( void )
{
   // transformacja wierzcho�ka zgodnie z aktualn� macierz� widoku modelu i rzutowania
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // skopiowanie koloru wierzcho�ka (primary color)
   gl_FrontColor = gl_Color;

   vec4 V = gl_ModelViewMatrix * gl_Vertex;
   distance = length(V);
   N = gl_NormalMatrix * gl_Normal;

   for(int i=0;i<3;i++)
	L[i] = lightPos[i] - V.xyz;
}
