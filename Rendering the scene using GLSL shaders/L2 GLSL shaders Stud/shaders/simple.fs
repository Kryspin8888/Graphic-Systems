// SG_lab2 23-11-2016
// simple.fs
// Zast¹pienie podstawowej funkcjonalnoœci sta³ego potoku renderowania
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a

void main( void )
{
   gl_FragColor = gl_Color; // skopiowanie g³ównego koloru (primary color)
}
