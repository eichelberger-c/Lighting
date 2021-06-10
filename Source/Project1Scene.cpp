/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Project1Scene.cpp
Purpose: Contains logic for displaying the requirments for this assignment
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/



#include "Project1Scene.h"
#include "Model.h"
#include "Light.h"
#include "shader.hpp"
#include "glm/vec3.hpp"
#include "glm/detail/type_mat.hpp"
#include <random>
#include "Gui.h"
#include "glm/gtx/vector_angle.hpp"
#include <random>
#include <time.h>
GLuint phongShading;
GLuint defaultShader;
GLuint phongLight;
GLuint blinnShading;
GLuint skybox;
float temp = 0;
std::vector<Model*> spheres;
std::vector<Model*> objs;
std::vector<glm::vec3> colors;
int currObjs = 1;
int ReflectMode = 0;
Model* bunny;
Model* quad;
Model* right;
int lMode = 0;
std::vector<Light> lights;
extern GLuint currShader;
extern bool pause;
extern glm::vec3 fog;
extern Scenario scene;
extern glm::vec3 emm;
extern glm::vec3 gamb;
extern float c1, c2, c3, ga;
bool switched = false;

extern float etar;
extern float etag;
extern float etab;
extern float eta;
extern float fpow;
extern float mix;
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void Project1Scene::SetupIMGUI(GLFWwindow *pWindow)
{
  GUI::init(pWindow);
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
Project1Scene::~Project1Scene()
{
    initMembers();
    glDeleteBuffers(1, &ubo);
    GUI::cleanup();
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
Project1Scene::Project1Scene(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight),
                                                                        angleOfRotation(0.0f)
{  
    initMembers();
    CreateFBO();
}
GLuint fbo;
GLuint rbo;
void Project1Scene::CreateFBO()
{
  glGenFramebuffers(1, &fbo);
  glGenRenderbuffers(1, &rbo);
}
glm::mat4 ortho = glm::perspective(glm::radians(45.f), 1024 / 768.f, 1.f, 1000.f);
glm::mat4 cam = glm::lookAt(glm::vec3(1.f, 0.f, -5.f), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
glm::vec3 camVec(0, 0, 5.f);
void Project1Scene::EveryLoop(Model* bun)
{
  glm::mat4 viewMat = cam;
  glm::mat4 project = ortho;

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _windowWidth, _windowHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
  ortho = glm::perspective(glm::radians(90.f), 1.f, .1f, 1000.f);

  glDisable(GL_DEPTH_TEST);

  //Right
  cam = glm::lookAt(bun->GetPos(), glm::vec3(1, 0, 0), glm::vec3(0, 1.f, 0));
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bun->tex[0]->textID, 0);
  Render();

  //Left
  cam = glm::lookAt(bun->GetPos(), glm::vec3(-1, 0, 0), glm::vec3(0, 1.f, 0));
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bun->tex[1]->textID, 0);
  Render();

  cam = glm::lookAt(bun->GetPos(), glm::vec3(0, 1, 0), glm::vec3(0, 0.f, -1.f));
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bun->tex[2]->textID, 0);
  Render();

  cam = glm::lookAt(bun->GetPos(), glm::vec3(0, -1, 0), glm::vec3(0, 0.f, 1.f));
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bun->tex[3]->textID, 0);
  Render();

  //Front
  cam = glm::lookAt(bun->GetPos(), glm::vec3(0, 0, -1), glm::vec3(0, 1.f, 0));
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bun->tex[4]->textID, 0);
  Render();

  //Back
  cam = glm::lookAt(bun->GetPos(), glm::vec3(0, 0, 1), glm::vec3(0, 1.f, 0));
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bun->tex[5]->textID, 0);
  Render();

  ortho = project;
  cam = viewMat;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glEnable(GL_DEPTH_TEST);

}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void Project1Scene::initMembers()
{
  srand(unsigned int(time(NULL)));
  phongShading = LoadShaders("../Common/shaders/PhongShading.vert",
    "../Common/shaders/PhongShading.frag");

  skybox = LoadShaders("../Common/shaders/SkyBox.vert",
    "../Common/shaders/SkyBox.frag");

  defaultShader = LoadShaders("../Common/shaders/QuadVertexShader.vert",
    "../Common/shaders/QuadFragmentShader.frag");

  phongLight = LoadShaders("../Common/shaders/PhongLighting.vert",
                                "../Common/shaders/PhongLighting.frag");

  blinnShading = LoadShaders("../Common/shaders/Blinn.vert",
    "../Common/shaders/Blinn.frag");
    currShader = phongShading;
  
    for (int i = 0; i < MAXLIGHTS; ++i)
    {
      colors.push_back(glm::vec3((rand() % 255) / 255.f, (rand() % 255) / 255.f, (rand() % 255) / 255.f));
    }

  glm::vec3 pos = glm::vec3(.0f, .2f, 1.f);
  for (int i = 0; i < MAXLIGHTS; ++i)
  {      
    float angle = 360.f / (MAXLIGHTS);

      spheres.push_back(new Model(Sphere_, "", .15f, colors[0]));
      spheres[i]->GetNormalProgram() = defaultShader;
      spheres[i]->GetProgram() = defaultShader;
      spheres[i]->GetPos() = pos;
      spheres[i]->GetColor() = glm::vec3(.8f,.4f, 1.f);
      pos = glm::rotateY(pos, glm::radians(angle));
  }
  std::vector<std::string> pics;
  pics.push_back("../Common/textures/right.png");
  pics.push_back("../Common/textures/left.png");
  pics.push_back("../Common/textures/top.png");
  pics.push_back("../Common/textures/bottom.png");
  pics.push_back("../Common/textures/front.png");
  pics.push_back("../Common/textures/back.png");

  std::vector<std::string> pics2;

  bunny = new Model(File_, "sphere", 0.f, glm::vec3(0, 0, 0), pics2, 1, _windowHeight, _windowWidth);
  bunny->GetNormalProgram() = defaultShader;
  bunny->GetProgram() = phongShading;
  bunny->GetPos() = glm::vec3(0, 0.f, 0.f);


  quad = new Model(File_, "cube", 0.f, glm::vec3(0,0,0), pics, 1);
  quad->GetNormalProgram() = defaultShader;
  quad->GetProgram() = skybox;
  quad->GetPos() = glm::vec3(0, 0,0);


  objs.push_back(bunny);
  objs.push_back(quad);

  temp = 10.f;
  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(
    GL_UNIFORM_BUFFER,
    sizeof(Light) * MAXLIGHTS,
    lights.data(),
    GL_DYNAMIC_DRAW
  );
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void Project1Scene::CleanUp()
{
    // Cleanup VBO
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
int Project1Scene::Init()
{
    // Create and compile our GLSL program from the shaders
    

    return Scene::Init();
}

int Project1Scene::preRender()
{
  EveryLoop(bunny);
  return 0;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
int Project1Scene::Render()
{

  for (unsigned i = 0; i < spheres.size(); ++i)
  {
    lights[i].pos = spheres[i]->GetPos();
    spheres[i]->GetColor() = lights[i].Id;
    lights[i].dir = glm::vec3(0, 0, 0) - lights[i].pos;
    if (!pause)
    {
      glm::mat3 rotMat = glm::rotate(.005f, glm::vec3(0.0f, 1.0f, 0.0f));
      spheres[i]->GetPos() = rotMat * spheres[i]->GetPos();
    }
    if (scene == one && switched)
    {
      lights[i].Id = colors[0];
      lights[i].Ia = colors[0];
      lights[i].Is = colors[0];
    }
    if (scene == two && switched)
    {
      lights[i].Id = colors[i];
      lights[i].Ia = colors[i];
      lights[i].Is = colors[i];
    }

    if (scene == three && switched)
    {
      lights[i].Id = colors[i];
      lights[i].Ia = colors[i];
      lights[i].Is = colors[i];
      lights[i].type = (rand() % 3);
    }
  }
  switched = false;

  int temp = sizeof(Light);

 

  if (GUI::rendermode == wireframe_)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if(GUI::rendermode == solid_)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(.5f, 0.5f, .5f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    

    std::vector<Model*> mod = spheres;

    for (int i = 0; i < currObjs; ++i)
    {
      glUseProgram(mod[i]->GetProgram());
      glBindVertexArray(mod[i]->GetVertexArray());
      glEnableVertexAttribArray(0);
      glm::vec3 rgb = mod[i]->GetColor();
      // Uniform matrix
      // Uniform transformation (vertex shader)
      GLint vTransformLoc = glGetUniformLocation(mod[i]->GetProgram(), "vertexTransform");
      GLint vRotation = glGetUniformLocation(mod[i]->GetProgram(), "Rotation");
      GLint colorLoc = glGetUniformLocation(mod[i]->GetProgram(), "rgb");
      // Draw the triangle !
      // T * R * S * Vertex
      glm::vec3 scaleVector = glm::vec3(1.f);
      glm::mat4 modelMat = glm::mat4(1.0f);
      glm::mat3 rotMat = glm::mat4(1.0f);
      rotMat = glm::rotate(.01f, glm::vec3(0.0f, 1.0f, 0.0f));

      glm::vec3 centroid = mod[i]->GetPos();

      modelMat = //glm::rotate(angleOfRotation, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(scaleVector) *
        glm::translate(centroid);

      modelMat = ortho * cam  * modelMat;
      glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);
      glUniformMatrix4fv(vRotation, 1, GL_FALSE, &rotMat[0][0]);
      glUniform3fv(colorLoc, 1, &rgb[0]);


      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[i]->GetElemenetBuffer());
      glDrawElements(mod[i]->GetMode(), mod[i]->GetFaces().size(), GL_UNSIGNED_INT, 0);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, lights.data(), sizeof(Light) * MAXLIGHTS);
    glUnmapBuffer(GL_UNIFORM_BUFFER);


    for (unsigned i = 0; i < objs.size(); ++i)
    {
      
      glUseProgram(objs[i]->GetProgram());

      if (1)
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, objs[i]->tex[0]->textID);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, objs[i]->tex[1]->textID);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, objs[i]->tex[2]->textID);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, objs[i]->tex[3]->textID);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, objs[i]->tex[4]->textID);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, objs[i]->tex[5]->textID);

        glUniform1i(glGetUniformLocation(objs[i]->GetProgram(), "right"), 0);
        glUniform1i(glGetUniformLocation(objs[i]->GetProgram(), "left"), 1);
        glUniform1i(glGetUniformLocation(objs[i]->GetProgram(), "up"), 2);
        glUniform1i(glGetUniformLocation(objs[i]->GetProgram(), "down"), 3);
        glUniform1i(glGetUniformLocation(objs[i]->GetProgram(), "front"), 4);
        glUniform1i(glGetUniformLocation(objs[i]->GetProgram(), "back"), 5);
      }
      


      glBindVertexArray(objs[i]->GetVertexArray());
      glEnableVertexAttribArray(0);
      glm::vec3 rgb = objs[i]->GetColor();
      GLint lights = glGetUniformBlockIndex(objs[i]->GetProgram(), "Lights");
      if (lights >= 0)
      {
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
      }
      // Uniform matrix
      // Uniform transformation (vertex shader)
      GLint vTransformLoc = glGetUniformLocation(objs[i]->GetProgram(), "vertexTransform");
      GLint rModeLoc = glGetUniformLocation(objs[i]->GetProgram(), "rMode");
      GLint vRotation = glGetUniformLocation(objs[i]->GetProgram(), "Rotation");
      GLint colorLoc = glGetUniformLocation(objs[i]->GetProgram(), "rgb");
      GLint numberOfLights = glGetUniformLocation(objs[i]->GetProgram(), "numberOfLights");
      GLint modelMatloc = glGetUniformLocation(objs[i]->GetProgram(), "ModelMat");
      GLint camPosLoc = glGetUniformLocation(objs[i]->GetProgram(), "cameraPos");

      GLint etaLoc = glGetUniformLocation(objs[i]->GetProgram(), "eta");
      GLint etarLoc = glGetUniformLocation(objs[i]->GetProgram(), "etar");
      GLint etagLoc = glGetUniformLocation(objs[i]->GetProgram(), "etag");
      GLint etabLoc = glGetUniformLocation(objs[i]->GetProgram(), "etab");

      GLint fpowLoc = glGetUniformLocation(objs[i]->GetProgram(), "fpow");
      GLint mixLoc = glGetUniformLocation(objs[i]->GetProgram(), "mixvar");

      GLint c1loc = glGetUniformLocation(objs[i]->GetProgram(), "c1uniform");
      GLint c2loc = glGetUniformLocation(objs[i]->GetProgram(), "c2uniform");
      GLint c3loc = glGetUniformLocation(objs[i]->GetProgram(), "c3uniform");
      GLint galoc = glGetUniformLocation(objs[i]->GetProgram(), "gauniform");

      GLint emmloc = glGetUniformLocation(objs[i]->GetProgram(), "foguniform");
      GLint fogloc = glGetUniformLocation(objs[i]->GetProgram(), "emm");

      GLint gambloc = glGetUniformLocation(objs[i]->GetProgram(), "gamb");

      // Draw the triangle !
      // T * R * S * Vertex
      glm::vec3 scaleVector;
      if(i == 0)
        scaleVector = glm::vec3(.5f);
      else
        scaleVector = glm::vec3(4.0f);
      glm::mat4 modelMat = glm::mat4(1.0f);
      glm::mat4 rotMat = glm::mat4(1.0f);
      rotMat = glm::rotate(.01f, glm::vec3(0.0f, 1.0f, 0.0f));


      glm::vec3 centroid = objs[i]->GetPos();
        modelMat = //glm::rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f)) *
          glm::scale(scaleVector) *
          glm::translate(centroid);
      glm::mat4 temp = modelMat;
      modelMat = ortho * cam * modelMat;
      glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);
      glUniformMatrix4fv(vRotation, 1, GL_FALSE, &rotMat[0][0]);
      glUniform3fv(colorLoc, 1, &rgb[0]);

      glUniform3fv(gambloc, 1, &gamb[0]);
      glUniform3fv(camPosLoc, 1, &camVec[0]);
      glUniform1i(numberOfLights, currObjs);
      glUniform1i(rModeLoc, ReflectMode);
      glUniformMatrix4fv(modelMatloc, 1, GL_FALSE, &temp[0][0]);

      glUniform1f(c1loc, c1);
      glUniform1f(c2loc, c2);
      glUniform1f(c3loc, c3);
      glUniform1f(galoc, ga);

      glUniform1f(etaLoc, eta);
      glUniform1f(etarLoc, etar);
      glUniform1f(etagLoc, etag);
      glUniform1f(etabLoc, etab);

      glUniform1f(fpowLoc, fpow);
      glUniform1f(mixLoc, mix);

      glUniform3fv(fogloc, 1, &fog[0]);
      glUniform3fv(emmloc, 1, &emm[0]);
     



      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objs[i]->GetElemenetBuffer());
      glDrawElements(objs[i]->GetMode(), objs[i]->GetFaces().size(), GL_UNSIGNED_INT, 0);
    }
    glEnable(GL_DEPTH_TEST);
    if (GUI::faceNormals)
    {
      for (int i = 0; i < currObjs; ++i)
      {
        glUseProgram(mod[i]->GetNormalProgram());
        glBindVertexArray(mod[i]->GetNormalArrayID());
        glEnableVertexAttribArray(0);
        glm::vec3 rgb2 = mod[i]->GetFaceNormColor();
        // Uniform matrix
        // Uniform transformation (vertex shader)
        GLint vTransformLoc2 = glGetUniformLocation(mod[i]->GetNormalProgram(), "vertexTransform");
        GLint colorLoc2 = glGetUniformLocation(mod[i]->GetNormalProgram(), "rgb");
        // Draw the triangle !
        // T * R * S * Vertex
        glm::vec3 scaleVector2 = glm::vec3(1.f);
        glm::mat4 modelMat2 = glm::mat4(1.0f);
        glm::vec3 centroid2 = mod[i]->GetPos();
          modelMat2 = glm::rotate(angleOfRotation, glm::vec3(0.0f, 1.0f, 1.0f)) *
          glm::scale(scaleVector2) *
          glm::translate(centroid2);
          modelMat2 = ortho * cam * modelMat2;

        glUniformMatrix4fv(vTransformLoc2, 1, GL_FALSE, &modelMat2[0][0]);
        glUniform3fv(colorLoc2, 1, &rgb2[0]);

        glDrawArrays(GL_LINES, 0, mod[i]->GetNormalsLines().size());
      }
    }

    if (GUI::vertexNormals)
    {
      for (int i = 0; i < currObjs; ++i)
      {
        glUseProgram(mod[i]->GetNormalProgram());
        glBindVertexArray(mod[i]->GetVertexNormalArrayID());
        glEnableVertexAttribArray(0);
        glm::vec3 rgb2 = mod[0]->GetVertexNormColor();
        // Uniform matrix
        // Uniform transformation (vertex shader)
        GLint vTransformLoc2 = glGetUniformLocation(mod[i]->GetNormalProgram(), "vertexTransform");
        GLint colorLoc2 = glGetUniformLocation(mod[i]->GetNormalProgram(), "rgb");
        // Draw the triangle !
        // T * R * S * Vertex
        glm::vec3 scaleVector2 = glm::vec3(1.f);
        glm::mat4 modelMat2 = glm::mat4(1.0f);
        glm::vec3 centroid2 = mod[i]->GetPos();
          modelMat2 = glm::rotate(angleOfRotation, glm::vec3(0.0f, 1.0f, 1.0f)) *
          glm::scale(scaleVector2) *
          glm::translate(centroid2);
          modelMat2 = ortho * cam * modelMat2;

        glUniformMatrix4fv(vTransformLoc2, 1, GL_FALSE, &modelMat2[0][0]);
        glUniform3fv(colorLoc2, 1, &rgb2[0]);

        glDrawArrays(GL_LINES, 0, mod[i]->GetVertexNormalLines().size());
      }
    }

    if (GUI::faceNormals)
    {
      for (unsigned i = 0; i < objs.size(); ++i)
      {
        glUseProgram(objs[i]->GetNormalProgram());
        glBindVertexArray(objs[i]->GetNormalArrayID());
        glEnableVertexAttribArray(0);
        glm::vec3 rgb2 = objs[i]->GetFaceNormColor();
        // Uniform matrix
        // Uniform transformation (vertex shader)
        GLint vTransformLoc2 = glGetUniformLocation(objs[i]->GetNormalProgram(), "vertexTransform");
        GLint colorLoc2 = glGetUniformLocation(objs[i]->GetNormalProgram(), "rgb");
        // Draw the triangle !
        // T * R * S * Vertex
        glm::vec3 scaleVector2;
        if(i > 0)
          scaleVector2 = glm::vec3(10.f);
        else
          scaleVector2 = glm::vec3(1.f);
        glm::mat4 modelMat2 = glm::mat4(1.0f);
        glm::vec3 centroid2 = objs[i]->GetPos();
        modelMat2 = glm::rotate(angleOfRotation, glm::vec3(0.0f, 1.0f, 0.0f)) *
          glm::scale(scaleVector2) *
          glm::translate(centroid2);
        modelMat2 = ortho * cam * modelMat2;

        glUniformMatrix4fv(vTransformLoc2, 1, GL_FALSE, &modelMat2[0][0]);
        glUniform3fv(colorLoc2, 1, &rgb2[0]);

        glDrawArrays(GL_LINES, 0, objs[i]->GetNormalsLines().size());
      }
    }

    if (GUI::vertexNormals)
    {
      for (unsigned i =0; i < objs.size(); ++i)
      {
        glUseProgram(objs[i]->GetNormalProgram());
        glBindVertexArray(objs[i]->GetVertexNormalArrayID());
        glEnableVertexAttribArray(0);
        glm::vec3 rgb2 = objs[i]->GetVertexNormColor();
        // Uniform matrix
        // Uniform transformation (vertex shader)
        GLint vTransformLoc2 = glGetUniformLocation(objs[i]->GetNormalProgram(), "vertexTransform");
        GLint colorLoc2 = glGetUniformLocation(objs[i]->GetNormalProgram(), "rgb");
        // Draw the triangle !
        // T * R * S * Vertex
        glm::vec3 scaleVector2;
        if (i > 0)
          scaleVector2 = glm::vec3(10.f);
        else
          scaleVector2 = glm::vec3(1.f);
        glm::mat4 modelMat2 = glm::mat4(1.0f);
        glm::vec3 centroid2 = objs[i]->GetPos();
        modelMat2 = glm::rotate(angleOfRotation, glm::vec3(0.0f, 1.0f, 0.0f)) *
          glm::scale(scaleVector2) *
          glm::translate(centroid2);
        modelMat2 = ortho * cam * modelMat2;

        glUniformMatrix4fv(vTransformLoc2, 1, GL_FALSE, &modelMat2[0][0]);
        glUniform3fv(colorLoc2, 1, &rgb2[0]);

        glDrawArrays(GL_LINES, 0, objs[i]->GetVertexNormalLines().size());
      }
    }


    if (GUI::orbitPath)
    {
      for (int i = 0; i < 1; ++i)
      {
        glUseProgram(mod[i]->GetNormalProgram());
        glBindVertexArray(mod[i]->GetorbitPathArrayID());
        glEnableVertexAttribArray(0);
        glm::vec3 rgb2 = mod[0]->GetOrbitColor();
        // Uniform matrix
        // Uniform transformation (vertex shader)
        GLint vTransformLoc2 = glGetUniformLocation(mod[i]->GetNormalProgram(), "vertexTransform");
        GLint colorLoc2 = glGetUniformLocation(mod[i]->GetNormalProgram(), "rgb");
        // Draw the triangle !
        // T * R * S * Vertex
        glm::vec3 scaleVector2 = glm::vec3(1.f);
        glm::mat4 modelMat2 = glm::mat4(1.0f);
        glm::vec3 centroid2 = glm::vec3(0,0,0);
        if (i != 8)
          modelMat2 = //glm::rotate(angleOfRotation, glm::vec3(0.0f, 1.0f, 1.0f)) *
          glm::scale(scaleVector2) *
          glm::translate(centroid2);
        else
          modelMat2 = glm::scale(scaleVector2) *
          glm::translate(centroid2);
        modelMat2 = ortho * cam * modelMat2;

        glUniformMatrix4fv(vTransformLoc2, 1, GL_FALSE, &modelMat2[0][0]);
        glUniform3fv(colorLoc2, 1, &rgb2[0]);

        glDrawArrays(GL_LINES, 0, mod[i]->GetOrbitPath().size());
      }
    }
    return 0;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
int Project1Scene::postRender()
{
    //angleOfRotation += 0.01f;
  GUI::render();
    cam = glm::lookAt(camVec, glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
    return 0;
}
