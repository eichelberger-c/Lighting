/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.h
Purpose: Base class for future project scenes
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Pushpak
Creation date: 3/28/18
End Header --------------------------------------------------------*/

#ifndef SAMPLE3_2_FBO_SCENE_H
#define SAMPLE3_2_FBO_SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


//#include "SceneManager.h"
//#include "VertexDataManager.h"


#define _GET_GL_ERROR   { GLenum err = glGetError(); std::cout << "[OpenGL Error] " << glewGetErrorString(err) << std::endl; }

class Scene
{

public:
    Scene();
    Scene( int windowWidth, int windowHeight );
    virtual ~Scene();

    // Public methods

    // Init: called once when the scene is initialized
    virtual int Init();

    // LoadAllShaders: This is the placeholder for loading the shader files
    virtual void LoadAllShaders();

    // Display : encapsulates per-frame behavior of the scene
    virtual int Display();

    // preRender : called to setup stuff prior to rendering the frame
    virtual int preRender();

    // Render : per frame rendering of the scene
    virtual int Render();

    // postRender : Any updates to calculate after current frame
    virtual int postRender();

    // cleanup before destruction
    virtual void CleanUp();

    // NanoGUI stuff
    //virtual void SetupNanoGUI(GLFWwindow *pWwindow) = 0;
//    virtual void CleanupNanoGUI(GLFWwindow *pWwindow, const nanogui::FormHelper &screen) = 0;

protected:
    int _windowHeight, _windowWidth;

    // Common functionality for all scenes
//    SceneManager                    _scnManager;
//    std::vector<VertexDataManager>   VAOs;

};


#endif //SAMPLE3_2_FBO_SCENE_H
