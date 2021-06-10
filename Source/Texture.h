/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: textures.h
Purpose: code for textures
Language: c++
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 11/12/19
End Header --------------------------------------------------------*/
#pragma once
#include <string>
#include <SOIL2.h>
#include<vector>
#include <GL/glew.h>
class Texture
{
public:
  Texture(int width, int height);
  Texture(std::string fileName);
  int width;
  int height;
  GLuint textID = 0;
  unsigned char* image;
};