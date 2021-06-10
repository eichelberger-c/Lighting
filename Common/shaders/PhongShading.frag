/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DiffuseShader.frag
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

vec2 CalcUV(vec3 vector)
{
  vec2 temp = vec2(0.0f);
  vec3 abs = abs(vector);
  if (abs.x >= abs.y && abs.x >= abs.z)
  {
    (vector.x < 0.0 ) ? ( temp.x = vector.z) : ( temp.x = -vector.z);
    temp.y = vector.y;
    temp = temp / abs.x;
  }
  else if (abs.y >= abs.x && abs.y >= abs.z)
  {
    (vector.y < 0.0 ) ? ( temp.y = vector.z) : ( temp.y = -vector.z);
    temp.x = vector.x;
    temp = temp / abs.y;
  }
  else if (abs.z >= abs.x && abs.z >= abs.y)
  {
    (vector.z < 0.0 ) ? ( temp.x = -vector.x) : ( temp.x = vector.x);
    temp.y = vector.y;
    temp = temp / abs.z;
  }
  temp += 1.0f;
  temp *= 0.5f;
  temp.x = 1 - temp.x;
  return temp;

}
int LEFT = 0;
int RIGHT = 1;
int TOP = 2;
int DOWN = 3;
int FRONT = 4;
int BACK = 5;

int GetPlane(vec3 vector)
{
  vec3 abs = abs(vector);
  int temp = 0;
  if (abs.x >= abs.y && abs.x >= abs.z)
  {
    if(vector.x > 0)
    {
      temp = LEFT;
    }
    else
    {
      temp = RIGHT;
    }
  }
  else if (abs.y >= abs.x && abs.y >= abs.z)
  {
    if(vector.y > 0)
    {
      temp = TOP;
    }
    else
    {
      temp = DOWN;
    }
  }
  else if (abs.z >= abs.x && abs.z >= abs.y)
  {
    if(vector.z < 0)
    {
      temp = FRONT;
    }
    else
    {
      temp = BACK;
    }
  }
  return temp;
}


vec3 GetColourFromTexture(vec2 uv, int plane)
{
  if(plane == TOP)
  {
    //return vec3(1.0f,0.0f,0.0f);
    return texture(up, uv).xyz;
  }
  if(plane == DOWN)
  {
    return texture(down, uv).xyz;
  }
  if(plane == RIGHT)
  {
    return texture(right, uv).xyz;
  }
  if(plane == LEFT)
  {
    return texture(left, uv).xyz;
  }
  if(plane == FRONT)
  {
    return texture(front, uv).xyz;
  }
  if(plane == BACK)
  {
    return texture(back, uv).xyz;
  }
}
vec3 calcReflect(vec3 I, vec3 norm)
{
	return 2 * -dot(norm, I) * norm - I;
}

vec3 calcRefract(vec3 I, vec3 norm, float ratio)
{
  float cosI = -dot(norm, I);
  float sinT2 = ratio * ratio * (1- cosI * cosI);
  float cosT = sqrt(1 - sinT2);
  return ratio * I + (ratio * cosI - cosT) * norm;
}

uniform int rMode;
uniform float eta;
uniform float etar;
uniform float etag;
uniform float etab;

uniform float fpow;
uniform float mixvar;
void main()
{
    vec3 norm = normalize(fs_in.fragNormal);
    float ratio = 1.0 / eta;
    float ratioR = 1.0 / etar;
    float ratioG = 1.0 / etag;
    float ratioB = 1.0 / etab;
    vec3 I = normalize(fs_in.modelViewVertex - fs_in.cameraPos);
    vec3 text = vec3(0,0,0);
    const float FresnelPower = fpow;
    if(rMode == 0)
    {
      vec3 R = calcReflect(I, norm);
      text = GetColourFromTexture(CalcUV(R), GetPlane(R));
    }
    else if (rMode == 1)
    {
      vec3 R = calcRefract(I, norm, ratio);
      text = GetColourFromTexture(CalcUV(R), GetPlane(R));
    }    
    else if (rMode == 2)
    {
      float F =  ((1.0-ratio) * (1.0-ratio)) / ((1.0+ratio) * (1.0+ratio));
      float frezRatio =  F + (1.0 - F) * pow((1.0 - dot(I, norm)), FresnelPower);
      vec3 reflect = calcReflect(I, norm);
      vec3 refractR = calcRefract(I, norm, ratioR);
      vec3 refractG = calcRefract(I, norm, ratioG);
      vec3 refractB = calcRefract(I, norm, ratioB);
      vec3 textReflec = GetColourFromTexture(CalcUV(reflect), GetPlane(reflect));
      vec3 textRefrac = vec3(0,0,0);
      textRefrac.r =  GetColourFromTexture(CalcUV(refractR), GetPlane(refractR)).r;
      textRefrac.g =  GetColourFromTexture(CalcUV(refractG), GetPlane(refractG)).g;
      textRefrac.b =  GetColourFromTexture(CalcUV(refractB), GetPlane(refractB)).b;
      text = mix(textRefrac, textReflec, frezRatio);
    }

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

    fragColor = mix(text, Ifin * fs_in.modelColor, mixvar);

}
