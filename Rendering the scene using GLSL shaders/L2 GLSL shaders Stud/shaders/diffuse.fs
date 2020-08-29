// SG_lab2 23-11-2016
// diffuse.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a

void main( void )
{
	float intensity = max(0.0, normalize(dot(N,L[0])));
   gl_FragColor = gl_Color; // skopiowanie g³ównego koloru (primary color)
   gl_FragColor.xyz *= intensity;
}
