/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DiffuseShader.Vert
Purpose: Contains code for lighint shader
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#version 460 core
uniform mat4 vertexTransform;

in layout(location = 0) vec3 modelPosition;
in layout(location = 1) vec3 vertexNormal;
in layout(location = 2) vec2 atexture;

out VS_OUT
{
    vec3 fragNormal;
    vec3 cameraPos;
    vec3 modelViewVertex;
    vec3 modelColor;
} vs_out;
uniform vec3 rgb;
uniform mat4 Rotation;
uniform mat4 ModelMat;

out vec2 TextCoord;

vec2 CalcUV()
{
  vec3 absVec = abs(modelPosition);
  vec3 uv = vec3(0.0);
  // +-X
  if( absVec.x >= absVec.y && absVec.x >= absVec.z )
  {
    ( modelPosition.x < 0.0 ) ? ( uv.x = modelPosition.z ) : ( uv.x = -modelPosition.z );
    uv.y = modelPosition.y;
  }

  if( absVec.y >= absVec.x && absVec.y >= absVec.z )
  {
    uv.x = modelPosition.x;
    ( modelPosition.y < 0.0 ) ? ( uv.y = modelPosition.z ) : ( uv.y = -modelPosition.z );
  }

  if( absVec.z >= absVec.y && absVec.z >= absVec.x )
  {
    ( modelPosition.z < 0.0 ) ? ( uv.x = -modelPosition.x ) : ( uv.x = modelPosition.x );
    uv.y = modelPosition.y;
  }
  uv =  (( uv + vec3(1.0) ) * 0.5);
  uv.y = 1 - uv.y;
  return uv.xy;
}

uniform vec3 cameraPos;

void main()
{
    vs_out.cameraPos = cameraPos;
    vs_out.modelColor = rgb;
    vs_out.fragNormal = (Rotation * vec4(vertexNormal, 0.0f)).xyz;

    vs_out.modelViewVertex = (ModelMat * vec4(modelPosition, 1.0)).xyz;

    TextCoord = CalcUV();
	gl_Position = vertexTransform * vec4( modelPosition, 1.0f );
}
