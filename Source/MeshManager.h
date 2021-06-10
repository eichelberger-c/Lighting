#ifndef ASSIGNMENT2_MESHMANAGER_H
#define ASSIGNMENT2_MESHMANAGER_H
#include "Mesh.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: meshmanager.h
Purpose: controls the meshes and makes sure there are only one
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/

class MeshManager
{
public:
    static Mesh* GetMesh(MeshType type, std::string file = "", float radius = 0.f, int index = 0);

private:
    static std::vector<Mesh*> meshes;
};

#endif //ASSIGNMENT2_MESHMANAGER_H
