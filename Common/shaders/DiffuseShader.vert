/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DiffuseShader.Vert
Purpose: Contains code for lighint shader
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#version 410 core
uniform mat4 vertexTransform;
in layout(location = 0) vec3 modelPosition;
in layout(location = 1) vec3 vertexNormal;
//in layout(location = 2) vec3 diffuseColor;

vec3 diffuseColor = vec3( 0.3f, 0.3f, 0.3f );
out VS_OUT
{
    vec3 fragDiffuse;
    vec3 fragNormal;

} vs_out;
uniform vec3 rgb;
uniform mat4 Rotation;

void main()
{
    vs_out.fragDiffuse = rgb;
    vs_out.fragNormal = (Rotation * vec4(vertexNormal, 0.0f)).xyz;

	gl_Position = vertexTransform * vec4( modelPosition, 1.0f );
}
