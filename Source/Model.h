/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.h
Purpose: Contains everything for reading from a file and making a model to be displayed
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#ifndef MODEL_H
#define MODEL_H

#define MAXLIGHTS 16

#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <string>
#include <vector>
#include "Mesh.h"
#include "Light.h"
#include "Texture.h"

class Model
{
  public:
    Model(MeshType type, std::string file = "", float radius = 0.f, glm::vec3 = glm::vec3(1, 1, 1), std::vector<std::string> pics = std::vector<std::string>(), int index = 0, int height = 0, int width = 0);
    ~Model();

    

    std::vector<glm::vec3> GetVertices() { return mesh->GetVertices(); }
    std::vector<unsigned int> GetFaces() { return mesh->GetFaces(); }
    GLuint GetVerttexBuffer() { return mesh->GetVerttexBuffer(); }
    GLuint GetElemenetBuffer() { return mesh->GetElemenetBuffer(); }
    GLuint GetVertexArray() { return mesh->GetVertexArray(); }
    int GetMode() { return mesh->GetMode(); }
    glm::vec3& GetColor() { return color; }
    glm::vec3& GetFaceNormColor() { return fNormColor; }
    glm::vec3& GetVertexNormColor() { return vNormColor; }
    glm::vec3& GetOrbitColor() { return orbitColor; }
    GLuint GetNormalBuffer() { return mesh->GetNormalBuffer();}
    GLuint GetNormalArrayID() { return mesh->GetNormalArrayID();}
    GLuint GetVertexNormalArrayID() { return mesh->GetVertexNormalArrayID(); }
    GLuint GetorbitPathArrayID() { return mesh->GetorbitPathArrayID(); }
    std::vector<glm::vec3> GetNormalsLines() { return mesh->GetNormalsLines(); }
    std::vector<glm::vec3> GetVertexNormalLines() { return mesh->GetVertexNormalLines(); }
    std::vector<glm::vec3> GetOrbitPath() { return mesh->GetOrbitPath(); }
    GLuint& GetProgram() { return mesh->GetProgram(); }
    GLuint& GetNormalProgram() { return mesh->GetNormalProgram(); }
    glm::vec3& GetPos() { return pos; }
    Light* light = nullptr;
    std::vector<Texture*> tex;
  protected:
    MeshType type;

    Mesh* mesh;

    glm::vec3 color = glm::vec3(1.f,1.f,1.f);
    glm::vec3 fNormColor = glm::vec3(1.f,0,0);
    glm::vec3 vNormColor = glm::vec3(0.f, 1.f, 0);
    glm::vec3 orbitColor = glm::vec3(1.f, 1.f, 0);
    glm::vec3 pos = glm::vec3(0,0,0);


};

#endif