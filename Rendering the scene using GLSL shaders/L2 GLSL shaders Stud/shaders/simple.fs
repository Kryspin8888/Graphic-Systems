// SG_lab2 23-11-2016
// simple.fs
// Zastąpienie podstawowej funkcjonalności stałego potoku renderowania
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory światła

void main( void )
{
   gl_FragColor = gl_Color; // skopiowanie głównego koloru (primary color)
}
