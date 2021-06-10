/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: blinn.frag
Purpose: Contains code for lighint shader
Language: GLSL
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
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

out vec3 fragColor;
uniform int numberOfLights;
uniform float c1uniform;
uniform float c2uniform;
uniform float c3uniform;

uniform vec3 emm;
vec3 calcLight(Light light)
{
    int lightMode = light.type;
    float ka = light.ka;
    float kd = light.kd;
    float ks = light.ks;
    vec3 iem = emm;
    float ns =  light.ns;
    vec3 id = light.id;
    vec3 is = light.Is;
    vec3 ia = light.Ia;
    
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
    vec3 halfvec = lightVec + viewVec;
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

    vec3 Ispec = is * ks * pow(max(dot(norm,halfvec),0.f), ns);

    vec3 Iloc = iem + (att * (Idiff+ Iamb +  Ispec));

    return Iloc;
}
uniform vec3 foguniform;
uniform vec3 gamb;
uniform float gauniform;
void main()
{
    vec3 lightResult = gamb * gauniform;
    for(int i = 0; i < numberOfLights; ++i)
    {
        lightResult += calcLight(lights[i]);
    }

    vec3 fog = foguniform;
    float vdist = length(fs_in.cameraPos - fs_in.modelViewVertex);
    float far = -1;
    float near = 1;
    float s = (far - vdist)/(far - near);

    vec3 Ifin = s * lightResult + (1.f - s) * fog;

    fragColor = Ifin * fs_in.modelColor;

}
