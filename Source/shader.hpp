/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.hpp
Purpose: Base class for future project scenes
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Pushpak
Creation date: 3/28/18
End Header --------------------------------------------------------*/
#ifndef SHADER_HPP
#define SHADER_HPP

enum Primitive_Enum
{
    TriangleStrip = 0,
    Points,
    LineStrip
};

///////////////////
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

///////////////////
GLuint LoadPipeline(const char * vertex_file_path,const char * fragment_file_path, GLuint * programIDs );

// Load shaders where multiple shader files == one complete shader
// Show how code can be reused across shaders
GLuint LoadMultiShaders(const char *vertex_file_path, const char *fragment_file_path,
                        const char *geom_file_path, Primitive_Enum  out_primitive_type );


#endif
