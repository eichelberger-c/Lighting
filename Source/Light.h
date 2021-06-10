/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.h
Purpose: Contains code for lighint shader
Language: c++
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#pragma once
#include <glm/glm.hpp>
enum LightType
{
  Point,
  Directional,
  Spotlight
};

struct Light
{
public:
  Light(glm::vec3 color = glm::vec3(1,1,1)) { Ia = color, Is = color; Id = color; }
  float kd = 4.f;
  float ks = .8f;
  float ka = .8f;
  float ns = 10.1f;

  glm::vec3 Id = glm::vec3(1, 1, 1);
  int type = (int)Point;

  glm::vec3 pos = glm::vec3(1, 1, 1);
  float pad = 0.f;

  glm::vec3 Is = glm::vec3(1, 1, 1);
  float innerAngle = 6.f;

  glm::vec3 Ia = glm::vec3(1, 1, 1);
  float outerAngles = 15.f;

  glm::vec3 dir = glm::vec3(0, 0, 0);
  float falloff = 1.f;
};

