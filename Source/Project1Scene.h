/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Project1Scene.h
Purpose: Contains logic for displaying the requirments for this assignment
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#ifndef SIMPLE_SCENE_Project1Scene_H
#define SIMPLE_SCENE_Project1Scene_H

#include "Scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <vector>
#include "Model.h"

class Project1Scene : public Scene
{

public:
    Project1Scene() = default;
    Project1Scene(int windowWidth, int windowHeight);
    void CreateFBO();
    void EveryLoop(Model* bun);
    virtual ~Project1Scene();
    static void SetupIMGUI(GLFWwindow* pWwindow);

public:
    int Init() override;
    void CleanUp() override;

    int preRender() override;
    int Render() override;
    int postRender() override;


private:

    // member functions
    void initMembers();

    // This is the non-software engineered portion of the code
    // Go ahead and modularize the VAO and VBO setup into
    // BufferManagers and ShaderManagers

    

    // data members
    
    GLfloat   angleOfRotation;

    GLuint ubo;


};


#endif //SIMPLE_SCENE_Project1Scene_H
