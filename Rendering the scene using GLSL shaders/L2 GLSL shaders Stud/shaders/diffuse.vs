// SG_lab2 23-11-2016
// diffuse.vs
// 

uniform vec3 lightPos[ 3 ]; // maksymalnie trzy po³o¿enia œwiat³a
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a

void main( void )
{
   N = normalize(gl_NormalMatrix * gl_Normal);
   vec4 V = vec4(gl_ModelViewMatrix * gl_Vertex);
   L[0] = normalize(lightPos[0] - V.xyz);

   // transformacja wierzcho³ka zgodnie z aktualn¹ macierz¹ widoku modelu i rzutowania
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   // skopiowanie koloru wierzcho³ka (primary color)
   gl_FrontColor = gl_Color;
}
