/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: Contains code for making meshes
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 11/04/19
End Header --------------------------------------------------------*/

#ifndef ASSIGNMENT2_MESH_H
#define ASSIGNMENT2_MESH_H

#include <string>
#include <fstream>
#include <GL/glew.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
enum MeshType
{
    File_,
    Sphere_
};
class Mesh {
public:
    Mesh(MeshType type, std::string file = "", float radius = 0.0f, int index = 0);
    float Radius() {return radius_;}
    MeshType Type(){return type_;}
    std::string& File(){return file_;}

    std::vector<glm::vec3> GetVertices() { return vertices; }
    std::vector<unsigned int> GetFaces() { return faces; }
    GLuint GetVerttexBuffer() { return vertexbuffer; }
    GLuint GetElemenetBuffer() { return elementBuffer; }
    GLuint GetVertexArray() { return VertexArrayID; }
    int GetMode() { return drawMode; }
    GLuint GetNormalBuffer() { return NormalBuffer;}
    GLuint GetNormalArrayID() { return NormalArrayID;}
    GLuint GetVertexNormalArrayID() { return vertexNormalArrayID; }
    GLuint GetorbitPathArrayID() { return orbitPathArrayID; }
    std::vector<glm::vec3> GetNormalsLines() { return normalLines; }
    std::vector<glm::vec3> GetVertexNormalLines() { return vertexNormalLines; }
    std::vector<glm::vec3> GetOrbitPath() { return orbitPath; }
    GLuint& GetProgram() { return programID; }
    GLuint& GetNormalProgram() { return NormalProgram; }


    int index_ = 0;
private:
    void LoadFromFile(std::string file, int index = 0);
    void CreateSphere(float rad, int numDivisions);

    void SetupBuffers();

    void CleanUpBuffers();

    float radius_ = 0.f;
    std::string file_ = "";
    MeshType type_ = Sphere_;

    GLuint  vertexbuffer;
    GLuint  VertexArrayID;
    GLuint elementBuffer;
    GLuint NormalArrayID;
    GLuint vertexNormalArrayID;
    GLuint NormalBuffer;
    GLuint vertexNormalBuffer;
    GLuint orbitBuffer;
    GLuint vertexNormalLinesBuffer;
    GLuint  programID;
    GLuint  NormalProgram;
    GLuint orbitPathArrayID;

    int drawMode = GL_TRIANGLES;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normalLines;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> vertexNormals;
    std::vector<glm::vec3> vertexNormalLines;
    std::vector<glm::vec3> orbitPath;

    std::vector<unsigned int> faces;
};


#endif //ASSIGNMENT2_MESH_H
