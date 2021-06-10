/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Skybox.frag
Purpose: Contains code for lighint shader
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 12/02/19
End Header --------------------------------------------------------*/
#version 460 core
const int POINT = 0;
const int DIRECTIONAL = 1;
const int SPOTLIGHT = 2;


struct Light
{
  float kd;
  float ks;
  float ka;
  float ns;

  vec3 id;
  int type;

  vec3 pos;
  float pad;

  vec3 Is;
  float innerAngle;

  vec3 Ia;
  float outerAngles;

  vec3 dir;
  float falloff;
};


#define NUMBER_POINT_LIGHTS 16
layout(std140, binding = 2) uniform Lights
{
    Light lights[NUMBER_POINT_LIGHTS];
};

in VS_OUT
{
    vec3 fragNormal;
    vec3 cameraPos;
    vec3 modelViewVertex;
    vec3 modelColor;
} fs_in;

uniform sampler2D right;
uniform sampler2D left;
uniform sampler2D up;
uniform sampler2D down;
uniform sampler2D front;
uniform sampler2D back;

out vec3 fragColor;
uniform int numberOfLights;
uniform float c1uniform;
uniform float c2uniform;
uniform float c3uniform;

uniform vec3 emm;
vec3 calcLight(Light light, vec3 text)
{
    int lightMode = light.type;
    float ka = light.ka;
    float kd = light.kd;
    float ks = light.ks;
    vec3 iem = emm;
    float ns =  light.ns;
    vec3 id = light.id;
    vec3 is = light.Is;
    vec3 ia = text;
    
    float c1 = c1uniform;
    float c2 = c2uniform;
    float c3 = c3uniform;

    float theta = 1.f;
    float cutoff = light.outerAngles;


    vec3 lightDir = normalize(light.dir);
    vec3 lightVec = vec3(0,0,0);
    float dist = length(light.pos - fs_in.modelViewVertex);
    float vdist = length(fs_in.cameraPos - fs_in.modelViewVertex);
    float att = 1.f;
    vec3 norm = normalize(fs_in.fragNormal);
    vec3 viewVec = normalize(fs_in.cameraPos - fs_in.modelViewVertex);

    lightVec = normalize(light.pos - fs_in.modelViewVertex);
    vec3 reflectVec = 2 * dot(norm, lightVec) * norm - lightVec;
    vec3 Iamb = ia * ka;

    vec3 Idiff = vec3(0,0,0);
     att = min(1 / (c1 + c2 * dist + c3 * dist *dist), 1.0);

   
    if(lightMode == POINT)
    {
         Idiff = id * kd * max(dot(lightVec,norm), 0.f);

    }
    if(lightMode == DIRECTIONAL)
    {
        if(dot(norm, lightVec) <0)
            Idiff *= 0;
        else
            Idiff = id * kd * max(dot(lightDir, -lightVec),0);
    }
    else if(lightMode == SPOTLIGHT)
    {
        theta = dot(lightVec, -lightDir);
        theta = acos(theta);
        theta = degrees(theta);
        if(theta > cutoff || dot(norm, lightVec) <0)
          Idiff *= 0;
        else
          Idiff = id * kd * pow(smoothstep(cutoff, light.innerAngle, theta), light.falloff);
    }

    vec3 Ispec = is * ks * pow(max(dot(viewVec,reflectVec),0.f), ns);

    vec3 Iloc = iem + (att * (Idiff+ Iamb +  Ispec));

    return Iloc;
}
uniform vec3 foguniform;
uniform vec3 gamb;
uniform float gauniform;

vec2 CalcUV()
{
  vec3 entity = fs_in.modelViewVertex;
  vec2 temp = vec2(0.0f);
  vec3 abs = abs(entity);

  if (abs.x >= abs.y && abs.x >= abs.z)
  {
    temp.x = entity.y / abs.x;
    temp.y = entity.z / abs.x;
  }
  else if (abs.y >= abs.x && abs.y >= abs.z)
  {
    temp.x = entity.x / abs.y;
    temp.y = entity.z / abs.y;
  }
  else if (abs.z >= abs.x && abs.z >= abs.y)
  {
    temp.x = entity.x / abs.z;
    temp.y = entity.y / abs.z;
  }

  temp += 1.0f;
  temp *= 0.5f;
  //temp.y = 1 - temp.y;
  //temp.x = 1 - temp.x;

  return temp.xy;
}

vec3 CalcText(vec2 TextCoord)
{
  vec3 pos = fs_in.modelViewVertex;
  vec3 returnVal;
  vec3 absVec = abs(pos);
  // +-X
  if( absVec.x >= absVec.y && absVec.x >= absVec.z )
  {
    ( pos.x < 0.0 ) ? (returnVal = texture(left, TextCoord).xyz) : (returnVal = texture(right, TextCoord).xyz);
  }

  if( absVec.y >= absVec.x && absVec.y >= absVec.z )
  {
    ( pos.y < 0.0 ) ? (returnVal = texture(down, TextCoord).xyz) : (returnVal = texture(up, TextCoord).xyz);
  }

  if( absVec.z >= absVec.y && absVec.z >= absVec.x )
  {
    ( pos.z < 0.0 ) ? (returnVal = texture(back, TextCoord).xyz) : (returnVal = texture(front, TextCoord).xyz);
  }
  return returnVal;
}
void main()
{
    vec3 norm = normalize(fs_in.fragNormal);
    vec3 pos = fs_in.modelViewVertex;
    vec3 text = CalcText(CalcUV());


    vec3 lightResult = gamb * gauniform;
    for(int i = 0; i < numberOfLights; ++i)
    {
        lightResult += calcLight(lights[i], text);
    }

    vec3 fog = foguniform;
    float vdist = length(fs_in.cameraPos - fs_in.modelViewVertex);
    float far = -1;
    float near = 1;
    float s = (far - vdist)/(far - near);

    vec3 Ifin = s * lightResult + (1.f - s) * fog;

    fragColor = text; // (Ifin * fs_in.modelColor);

}
