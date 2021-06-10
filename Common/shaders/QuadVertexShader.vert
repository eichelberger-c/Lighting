/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: QuadVertexShader.vert
Purpose: Contains code for showing model
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#version 410 core

uniform mat4  vertexTransform;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3  vPosition;
uniform vec3 rgb;
out vec3 vSmoothColor;
void main()
{
    vec4 vPos = vec4( vPosition.x, vPosition.y, vPosition.z, 1.0f );
    gl_Position = vertexTransform * vPos;
    vSmoothColor = rgb;
}
