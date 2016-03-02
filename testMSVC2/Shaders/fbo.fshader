uniform sampler2D colorTxt, depthTxt;
float zNear = 0.1, zFar = 450.;
const float blurclamp = 3.0, bias = 0.6;
const float focus = 0.02;
void main()
{	
	float aspect = 800.0/600.0;
	vec2 aspectCorrect = vec2(1.0, aspect);

	vec4 color = texture2D(colorTxt, gl_TexCoord[0].xy);
gl_FragColor = color;
return;
	vec4 depth = texture2D(depthTxt, gl_TexCoord[0].xy);
	float factor = depth.x-focus;
	vec2 dofBlur = vec2(clamp(factor*bias, -blurclamp, blurclamp));
	color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.0,0.4 )*aspectCorrect) * dofBlur);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.15,0.37 )*aspectCorrect) * dofBlur);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.29,0.29 )*aspectCorrect) * dofBlur);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.37,0.15 )*aspectCorrect) * dofBlur);       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.4,0.0 )*aspectCorrect) * dofBlur);   
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.37,-0.15 )*aspectCorrect) * dofBlur);       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.29,-0.29 )*aspectCorrect) * dofBlur);       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.15,-0.37 )*aspectCorrect) * dofBlur);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.0,-0.4 )*aspectCorrect) * dofBlur); 
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.15,0.37 )*aspectCorrect) * dofBlur);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.29,0.29 )*aspectCorrect) * dofBlur);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.37,0.15 )*aspectCorrect) * dofBlur); 
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.4,0.0 )*aspectCorrect) * dofBlur); 
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.37,-0.15 )*aspectCorrect) * dofBlur);       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.29,-0.29 )*aspectCorrect) * dofBlur);       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.15,-0.37 )*aspectCorrect) * dofBlur);
       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.15,0.37 )*aspectCorrect) * dofBlur*0.9);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.37,0.15 )*aspectCorrect) * dofBlur*0.9);           
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.37,-0.15 )*aspectCorrect) * dofBlur*0.9);           
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.15,-0.37 )*aspectCorrect) * dofBlur*0.9);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.15,0.37 )*aspectCorrect) * dofBlur*0.9);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.37,0.15 )*aspectCorrect) * dofBlur*0.9);            
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.37,-0.15 )*aspectCorrect) * dofBlur*0.9);   
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.15,-0.37 )*aspectCorrect) * dofBlur*0.9);   
       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.29,0.29 )*aspectCorrect) * dofBlur*0.7);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.4,0.0 )*aspectCorrect) * dofBlur*0.7);       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.29,-0.29 )*aspectCorrect) * dofBlur*0.7);   
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.0,-0.4 )*aspectCorrect) * dofBlur*0.7);     
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.29,0.29 )*aspectCorrect) * dofBlur*0.7);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.4,0.0 )*aspectCorrect) * dofBlur*0.7);     
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.29,-0.29 )*aspectCorrect) * dofBlur*0.7);   
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.0,0.4 )*aspectCorrect) * dofBlur*0.7);
                         
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.29,0.29 )*aspectCorrect) * dofBlur*0.4);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.4,0.0 )*aspectCorrect) * dofBlur*0.4);       
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.29,-0.29 )*aspectCorrect) * dofBlur*0.4);   
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.0,-0.4 )*aspectCorrect) * dofBlur*0.4);     
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.29,0.29 )*aspectCorrect) * dofBlur*0.4);
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.4,0.0 )*aspectCorrect) * dofBlur*0.4);     
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( -0.29,-0.29 )*aspectCorrect) * dofBlur*0.4);   
        color += texture2D(colorTxt, gl_TexCoord[0].xy + (vec2( 0.0,0.4 )*aspectCorrect) * dofBlur*0.4);  
/*	vec4 depthComp;
	if (depth < .5) depthComp = vec4(1.,0.,0.,1.);
	else if (depth > 0.5) depthComp = vec4(0.,1.,0.,1.);
	else depthComp = vec4(0.,0.,1.,1.);*/
//	depth = depth*(zFar - zNear);
//	gl_FragColor = color*depthComp;
//color = vec4(0.7, 0.5, 0.3, 1.0);
	gl_FragColor = color/31.0;
	gl_FragColor.a = 1.0;
}
