/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: makes textures
Language: c++
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#include "Texture.h"
#include <fstream>
#include <GL/glew.h>
Texture::Texture(std::string fileName)
{
  bool jpg = false;
  if(fileName.find(".jpg") != std::string::npos)
  {
    jpg = true;
  }


  glGenTextures(1, &textID);
  glBindTexture(GL_TEXTURE_2D, textID);
  if(jpg)
    image = SOIL_load_image(fileName.c_str(), &width, &height, NULL, SOIL_LOAD_RGB);
  else
    image = SOIL_load_image(fileName.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if (image)
  {
    if(jpg)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  SOIL_free_image_data(image);
}

Texture::Texture(int width_, int height_) : width(width_), height(height_)
{
  glGenTextures(1, &textID);
  glBindTexture(GL_TEXTURE_2D, textID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}
