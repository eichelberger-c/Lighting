/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: QuadFragmentShader.frag
Purpose: Contains code for showing model
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#version 410 core

// Input from the rasterizer
// Conceptually equivalent to
// "WritePixel( x, y, rasterColor )"

// Output data
in vec3 vSmoothColor;
out vec3 color;

void main()
{
	color = vSmoothColor;
}
