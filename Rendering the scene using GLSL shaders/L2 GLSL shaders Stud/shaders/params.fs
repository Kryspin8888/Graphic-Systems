// SG_lab2 23-11-2016
// params.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a
varying float distance;
uniform float density = 0.1;

void main( void )
{
	const float specularExp = 128.0;
	const vec4 fogColor = vec4(0.5,0.8,0.5,1.0);
	const float e = 2.71828;

	
	vec3 NN = normalize(N);

   vec3 lightCol[3];
   lightCol[0] = vec3(1.0,0.25,0.25);
   lightCol[1] = vec3(0.25,1,0.25);
   lightCol[2] = vec3(0.25,0.25,1.0);

   gl_FragColor = vec4(0.0);

   for(int i =0; i<3; i++)
   {
		vec3 NL = normalize(L[i]);
		vec3 NH = normalize(NL + vec3(0.0,0.0,1.0));

		float NdotL = max(0.0,dot(NN,NL));

		gl_FragColor.rgb += gl_Color.rgb * lightCol[i] * NdotL;

		if(NdotL >0.0)
		gl_FragColor.rgb += lightCol[i] * pow(max(0.0, dot(NN,NH)),specularExp);
   }

   float fogExp = (density * distance)/200;
   fogExp *= fogExp;
   float fogFactor = clamp(pow(e,-fogExp), 0.0,1.0);

   gl_FragColor = mix(fogColor,gl_FragColor,fogFactor);
   gl_FragColor.a = gl_Color.a;
}
