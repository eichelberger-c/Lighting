/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DiffuseShader.frag
Purpose: Contains code for lighint shader
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#version 410 core
in VS_OUT
{
    vec3 fragDiffuse;
    vec3 fragNormal;

} fs_in;

out vec3 fragColor;


void main()
{
	float ambientLight = .1f;
	vec3 ambientColor = vec3(1,1,1) * ambientLight;
    vec3 lightVector = vec3( 0.0, 0.0, 1.0 );
    float N_dot_L = max( dot( normalize(fs_in.fragNormal), normalize(lightVector)), 0.0f );
    fragColor = ambientColor;
	fragColor += fs_in.fragDiffuse * N_dot_L;
}
