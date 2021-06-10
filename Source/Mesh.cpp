/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: Contains code for making meshes
Language: c++
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/

#include "Mesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/vector_angle.hpp"
#include <utility>
#include <random>
void Mesh::LoadFromFile(std::string file, int index)
{
    std::string path = "../Common/models/";
    path+=file;
    path+=".obj";
    //path = "../Common/models/bunny.obj";
    std::ifstream in(path);

    glm::vec3 average(0);
    if (in.is_open())
    {
        vertices.clear();
        faces.clear();
        normalLines.clear();
        vertexNormalLines.clear();
        normals.clear();
        vertexNormals.clear();
        char prev = '\0';
        while (!in.eof())
        {
            char cur;
            cur = in.get();
            switch (cur)
            {
                case 'v':
                {
                    if ((prev == '\n' || prev == '\0'))
                    {
                        float x = 0;
                        float y = 0;
                        float z = 0;

                        in >> x >> y >> z;
                        glm::vec3 temp = glm::vec3(x, y, z);
                        if (file == "quad")
                        {
                          if (index == 1)
                            temp = glm::rotateZ(temp, glm::radians(-90.f));
                          else if(index == 2)
                            temp = glm::rotateY(temp, glm::radians(-90.f));
                        }
                          
                        vertexNormals.push_back(glm::vec3(0, 0, 0));
                        average = average + temp;
                        vertices.push_back(temp);
                    }
                    break;
                }

                case 'f':
                {
                    if (prev == '\n' || prev == '\0')
                    {
                        int count = 0;
                        while (!in.eof() && in.peek() != '\n' && in.peek() != '\\')
                        {
                            unsigned int x = 0;
                            in >> x;
                            faces.push_back(x - 1);
                            count++;
                        }
                        if (count > 3)
                            drawMode = GL_TRIANGLE_FAN;
                        else
                            drawMode = GL_TRIANGLES;
                    }
                    break;
                }
            }
            prev = cur;
        }
    }

    average.x = average.x / vertices.size();
    average.y = average.y / vertices.size();
    average.z = average.z / vertices.size();

    glm::vec3 minV(0.f);
    glm::vec3 maxV(0.f);

    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = vertices[i] - average;
        minV.x = std::min(minV.x, vertices[i].x);
        minV.y = std::min(minV.y, vertices[i].y);
        minV.z = std::min(minV.z, vertices[i].z);

        maxV.x = std::max(maxV.x, vertices[i].x);
        maxV.y = std::max(maxV.y, vertices[i].y);
        maxV.z = std::max(maxV.z, vertices[i].z);
    }

    float width, height, depth;

    height = std::abs(maxV.y - minV.y);
    width = std::abs(maxV.x - minV.x);
    depth = std::abs(maxV.z - minV.z);

    glm::vec3 scale(0.f);

    if (width > height && width > depth)
    {
        scale.y = 2 * height / width;

        scale.z = 2 * depth / width;
    }
    else if (height > width && height > depth)
    {
        scale.x = 2 * width / height;

        scale.z = 2 * depth / height;
    }
    else if (depth > height && depth > width)
    {
        scale.y = 2 * height / depth;

        scale.x = 2 * width / depth;
    }

    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        if (width > height && width > depth)
        {
            vertices[i].x = (vertices[i].x - minV.x) / width * 2.0f - 1.f;
            vertices[i].y = (vertices[i].y - minV.y) / height * scale.y - (scale.y / 2.f);
            vertices[i].z = (vertices[i].z - minV.z) / depth * scale.z - (scale.z / 2.f);
        }
        else if (height > width && height > depth)
        {

            vertices[i].x = (vertices[i].x - minV.x) / width * scale.x - (scale.x / 2.f);
            vertices[i].y = (vertices[i].y - minV.y) / height * 2.0f - 1.f;
            vertices[i].z = (vertices[i].z - minV.z) / depth * scale.z - (scale.z / 2.f);
        }
        else if (depth > height && depth > width)
        {
            vertices[i].x = (vertices[i].x - minV.x) / width * scale.x - (scale.x / 2.f);
            vertices[i].z = (vertices[i].y - minV.y) / height * scale.y - (scale.y / 2.f);
            vertices[i].y = (vertices[i].z - minV.z) / depth * 2.0f - 1.f;
        }
    }
}

Mesh::Mesh(MeshType type, std::string file, float radius, int index) : type_(type), file_(file), radius_(radius), programID()
{
    if(type == File_)
        LoadFromFile(file, index);
    else if(type == Sphere_)
        CreateSphere(radius, 360);

    SetupBuffers();
    index_ = index;
}

void Mesh::CreateSphere(float radius, int numDivisions)
{
    float x, y, z, xy;                              // vertex position
    float numDivisions2 = float(360) / numDivisions;
    float sectorCount = float(180) / numDivisions2;
    float stackCount = float(360) / numDivisions2;

    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = glm::pi<float>() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(glm::vec3(x, y, z));
            vertexNormals.push_back(glm::vec3(0, 0, 0));
        }
    }


    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = int(i * (sectorCount + 1));     // beginning of current stack
        k2 = int(k1 + sectorCount + 1);      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                faces.push_back(k1);
                faces.push_back(k2);
                faces.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                faces.push_back(k1 + 1);
                faces.push_back(k2);
                faces.push_back(k2 + 1);
            }
        }
    }
}

void Mesh::SetupBuffers()
{
    if (drawMode != GL_TRIANGLE_FAN)
    {
        for (unsigned i = 0; i < faces.size(); i += 3)
        {
            glm::vec3 v1 = vertices[faces[i + 2]] - vertices[faces[i]];
            glm::vec3 v0 = vertices[faces[i + 1]] - vertices[faces[i]];
            v0 = glm::normalize(glm::cross(v0, v1));
            glm::vec3 center = vertices[faces[i]] + vertices[faces[i + 1]] + vertices[faces[i + 2]];
            center /= 3.f;
            normals.push_back(v0);
            normalLines.push_back(center);
            normalLines.push_back(center + v0 * .004f);

            vertexNormals[faces[i + 0]] += v0;
            vertexNormals[faces[i + 1]] += v0;
            vertexNormals[faces[i + 2]] += v0;


        }
        for (unsigned i = 0; i < vertexNormals.size(); ++i)
        {
            vertexNormals[i] = glm::normalize(vertexNormals[i]);
        }

        for (unsigned i = 0; i < faces.size(); i += 3)
        {
            glm::vec3 p0 = vertices[faces[i + 0]];
            glm::vec3 p1 = vertices[faces[i + 1]];
            glm::vec3 p2 = vertices[faces[i + 2]];
            vertexNormalLines.push_back(p0);
            vertexNormalLines.push_back(vertexNormals[faces[i + 0]] * .004f + p0);
            vertexNormalLines.push_back(p1);
            vertexNormalLines.push_back(vertexNormals[faces[i + 1]] * .004f + p1);
            vertexNormalLines.push_back(p2);
            vertexNormalLines.push_back(vertexNormals[faces[i + 2]] * .004f + p2);
        }

    }
    float radius = .55f;

    for (float i = 1.f; i < 360.f; ++i)
    {
        orbitPath.push_back(glm::rotateX((glm::vec3(.0f,-.45f,0.50f) + glm::vec3(radius * sin(glm::radians(i)), radius * cos(glm::radians(i)), 0.f)), glm::radians(-90.f)));
    }



    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &elementBuffer);

    int size = vertices.size() * sizeof(glm::vec3) + vertexNormals.size() * sizeof(glm::vec3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glNamedBufferStorage(vertexbuffer, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vertexbuffer, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
    glNamedBufferSubData(vertexbuffer, vertices.size() * sizeof(glm::vec3), vertexNormals.size() * sizeof(glm::vec3), vertexNormals.data());



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint),
                 faces.data(), GL_STATIC_DRAW);



    glGenVertexArrays(1, &NormalArrayID);
    glBindVertexArray(NormalArrayID);
    glGenBuffers(1, &NormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
    glNamedBufferStorage(NormalBuffer, normalLines.size() * sizeof(glm::vec3), normalLines.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenVertexArrays(1, &vertexNormalArrayID);
    glBindVertexArray(vertexNormalArrayID);
    glGenBuffers(1, &vertexNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
    glNamedBufferStorage(vertexNormalBuffer, vertexNormalLines.size() * sizeof(glm::vec3), vertexNormalLines.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


    glGenVertexArrays(1, &orbitPathArrayID);
    glBindVertexArray(orbitPathArrayID);
    glGenBuffers(1, &orbitBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, orbitBuffer);
    glNamedBufferStorage(orbitBuffer, orbitPath.size() * sizeof(glm::vec3), orbitPath.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // attach VBO with vertex position attribute to VAO binding 0
    glVertexArrayVertexBuffer(VertexArrayID, 0, vertexbuffer, 0, sizeof(glm::vec3));
    // attach VBO with vertex color attribute to VAO binding 1
    glVertexArrayVertexBuffer(VertexArrayID, 1, vertexbuffer,
                              sizeof(glm::vec3) * vertices.size(), sizeof(float) * 3);


    glEnableVertexArrayAttrib(VertexArrayID, 0); // enable index 0: pos attribute
    glEnableVertexArrayAttrib(VertexArrayID, 1); // enable index 1: color attribute
    glEnableVertexArrayAttrib(VertexArrayID, 2); // enable index 1: color attribute
    // data format for vertex VertexArrayIDtributes
    glVertexArrayAttribFormat(VertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // data format for vertex VertexArrayIDbutes
    glVertexArrayAttribFormat(VertexArrayID, 1, 3, GL_FLOAT, GL_FALSE, 0);
    // associate VAO binding 0 to vertex attribute index 0
    glVertexArrayAttribBinding(VertexArrayID, 0, 0);
    // associate VAO binding 1 to vertex attribute index 1
    glVertexArrayAttribBinding(VertexArrayID, 1, 1);
    glVertexArrayAttribBinding(VertexArrayID, 2, 2);

}

void Mesh::CleanUpBuffers()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1, &elementBuffer);

    glDeleteBuffers(1, &NormalBuffer);
    glDeleteVertexArrays(1, &NormalArrayID);

    glDeleteBuffers(1, &vertexNormalBuffer);
    glDeleteVertexArrays(1, &vertexNormalArrayID);

    glDeleteBuffers(1, &orbitBuffer);
    glDeleteVertexArrays(1, &orbitPathArrayID);
}
