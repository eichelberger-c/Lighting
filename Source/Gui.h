/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Gui.cpp
Purpose: Contains code for displaying gui
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <GL/glew.h>

enum RenderMode
{
  wireframe_,
  solid_,
  points_
};

enum Scenario
{
  one,
  two,
  three,
};

class GUI
{
  public:
    static void init(GLFWwindow* window);
    static void cleanup();
    static void render();
    static void LightsMenu();
    static void ShadersMenu();
    //static bool isDisplay() { return display; }
    static bool faceNormals;
    static bool vertexNormals; 
    static bool orbitPath;
    static void SceneMenu();
    static RenderMode rendermode;
    static void ResetShader(GLuint id);

};