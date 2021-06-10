/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MeshManger.cpp
Purpose: Contains code for lighint shader
Language: c++
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 11/12/19
End Header --------------------------------------------------------*/

#include "MeshManager.h"
std::vector<Mesh*> MeshManager::meshes;
Mesh *MeshManager::GetMesh(MeshType type, std::string file, float radius, int index)
{
    int size = meshes.size();
    for(int i = 0; i < size; ++i)
    {
        if(meshes[i]->Type() == type && meshes[i]->File() == file && meshes[i]->Radius() == radius && meshes[i]->index_ == index)
            return meshes[i];
    }

    Mesh* mesh = new Mesh(type, file, radius, index);
    meshes.push_back(mesh);
    return mesh;
}

