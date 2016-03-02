varying vec3 lightDir, normal;
//uniform sampler2D tex[3];
uniform sampler2D tex1, tex2, tex3;

void main()
{
	vec3 cf, ct;
	vec4 texel;
	float intensity, af, at;
	intensity = max(dot(lightDir, normalize(normal)),0.0);	
	cf = intensity * (gl_FrontMaterial.diffuse).rgb + gl_FrontMaterial.ambient.rgb;
	af = gl_FrontMaterial.diffuse.a;
	

	texel = texture2D(tex3, gl_TexCoord[2].st) * texture2D(tex2, gl_TexCoord[1].st)+texture2D(tex1, gl_TexCoord[0].st);
//	texel = texture2D(tex2, gl_TexCoord[0].st);
//	texel = texture2D(tex[0], gl_TexCoord[0].st);
	ct = texel.rgb;
	at = texel.a;

	gl_FragColor = vec4(ct * cf, at * af);
}