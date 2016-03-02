varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

void main()
 {
	vec3 n;
	float NdotL, NdotHV;
	vec4 color = ambient;
	n = normalize(normal);
	NdotL = max(dot(n, lightDir), 0.0);
	if (NdotL > 0.0)
	{
		color += diffuse * NdotL; 
	}
 	gl_FragColor = color;
 }