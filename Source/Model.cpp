/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.cpp
Purpose: Contains everything for reading from a file and making a model to be displayed
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#include "Model.h"
#include <fstream>
#include "shader.hpp"
#include <algorithm>
#include <SOIL2.h>
#include "MeshManager.h"
#include<iostream>
#include "Project1Scene.h"
extern std::vector<Light> lights;
int curr = 0;
void APIENTRY openGLDebug(GLenum source, GLenum type, GLuint id,
  GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  switch (severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
  case GL_DEBUG_SEVERITY_MEDIUM:
    case GL_DEBUG_SEVERITY_LOW:
    std::cout << message << std::endl;
    break;
  default:
    break;
  }
}

Model::Model(MeshType type, std::string fileName, float radius, glm::vec3 color, std::vector<std::string> pics, int index, int height, int width)
{
    mesh = MeshManager::GetMesh(type, fileName, radius, index);
    if(radius > 0.f)
    {
      light = new Light(color);
      lights.push_back(*light);
    }
    if (pics.size() < 6)
    {
      for (unsigned i = 0; i < 6; ++i)
        tex.push_back(new Texture(width, height));
    }
    else
    {

      for (unsigned i = 0; i < pics.size(); ++i)
        tex.push_back(new Texture(pics[i].c_str()));
    }
}