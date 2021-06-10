/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Skybox.Vert
Purpose: Contains code for lighint shader
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 12/02/19
End Header --------------------------------------------------------*/
#version 460 core
uniform mat4 vertexTransform;

in layout(location = 0) vec3 modelPosition;
in layout(location = 1) vec3 vertexNormal;
in layout(location = 2) vec2 atexture;

out VS_OUT
{
    vec3 fragNormal;
    vec3 cameraPos;
    vec3 modelViewVertex;
    vec3 modelColor;
} vs_out;
uniform vec3 rgb;
uniform mat4 Rotation;
uniform mat4 ModelMat;

void main()
{
    vs_out.cameraPos = ( vec4(0,2, 5, 0)).xyz;
    vs_out.modelColor = rgb;
    vs_out.fragNormal = (Rotation * vec4(vertexNormal, 0.0f)).xyz;

    vs_out.modelViewVertex = (ModelMat * vec4(modelPosition, 1.0)).xyz;
	gl_Position = vertexTransform * vec4( modelPosition, 1.0f );
}
