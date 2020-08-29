// SG_lab2 23-11-2016
// specular.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a

void main( void )
{
const float specularExp = 128.0;
vec3 NN = normalize(N);
vec3 NL = normalize(L[0]);
vec3 NH = normalize(NL + vec3(0.0,0.0,1.0));

float diffuse = max(0.0, normalize(dot(NN,NL)));

float specular = max(0.0, dot(NN, NH));
    specular = pow(specular, specularExp);

   //gl_FragColor = gl_Color; // skopiowanie g³ównego koloru (primary color)
   gl_FragColor = gl_Color;
   gl_FragColor.xyz *= (specular + diffuse);
}
