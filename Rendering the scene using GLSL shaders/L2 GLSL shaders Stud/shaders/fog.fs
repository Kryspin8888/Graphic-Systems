// SG_lab2 23-11-2016
// fog.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a

void main( void )
{
   gl_FragColor = gl_Color; // skopiowanie g³ównego koloru (primary color)
}
