/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Gui.cpp
Purpose: Contains code for displaying gui
Language: C++ Visual Studio 2019 Compiler
Platform: MSVC x86 2019, windows 10, support OpenGL 4.6
Project: eichelberger.c_CS300_1
Author: Charles Eichelberger, eichelberger.c, 180000817
Creation date: 10/02/19
End Header --------------------------------------------------------*/
#include "Gui.h"
#include <string>
#include "shader.hpp"
#include "Model.h"
#include "Light.h"
#include "Project1Scene.h"
#include <glm\gtx\rotate_vector.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/vector_angle.hpp"
enum SelectedTab
{
  shaders_,
  lights_,
  models_,
  scene_,
};

float etar = 1.52f;
float etag = 1.52f;
float etab = 1.52f;
float eta = 1.52f;

float fpow = 1.f;
float mix = .5f;

int currLight = 1;
extern int ReflectMode;
Scenario scene = one;
SelectedTab tab = lights_;
bool fileOpen = false;
bool optionsOpen = true;
extern bool switched;
char fileName[20];
extern GLuint phongShading;
Model* currModel;
extern GLuint phongLight;
extern GLuint blinnShading;
GLuint currShader = 0;
RenderMode GUI::rendermode = solid_;
extern int currObjs;
bool GUI::faceNormals = false;
bool GUI::vertexNormals = false;
bool GUI::orbitPath = false;
extern int lMode;
extern Model* bunny;
extern Model* quad;
extern std::vector<Light> lights;
bool pause = false;
glm::vec3 gamb = glm::vec3(0,0,0);
extern glm::vec3 camVec;


float c1 = 1.f, c2 = 1.5f, c3 = 2.0f, ga = 1.f;
glm::vec3 fog = glm::vec3(0, 0, 0);
glm::vec3 emm = glm::vec3(0, 0, 0);
void GUI::init(GLFWwindow* window)
{
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
  ImGui::StyleColorsDark();
}

void GUI::cleanup()
{
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
}

void GUI::ShadersMenu()
{
  const char* shaders[] = { "Phong Shading", "Phong Lighting" };
  static const char* selected = shaders[0];
  if (ImGui::BeginCombo("Shader Selector", selected))
  {
    int size = sizeof(shaders) / sizeof(int*);
    for (int i = 0; i < size; ++i)
    {
      if (selected != shaders[i])
      {
        if (ImGui::Selectable(shaders[i]))
        {
          selected = shaders[i];
          if (!strcmp(selected, "Phong Shading"))
          {
            currShader = phongShading;
            bunny->GetProgram() = phongShading;

          }
          else if (!strcmp(selected, "Phong Lighting"))
          {
            currShader = phongLight;
            bunny->GetProgram() = phongLight;
          }
        }
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::Button("Reload Shader", ImVec2(300, 50)))
    ResetShader(currShader);
}

void GUI::SceneMenu()
{

  if (!pause)
  {
    if (ImGui::Button("Pause"))
      pause = true;
  }
  else
  {
    if (ImGui::Button("Play"))
      pause = false;
  }

  ImGui::NewLine();
  ImGui::NewLine();

  if (ImGui::Button("Reflection"))
  {
    ReflectMode = 0;
  }
  ImGui::SameLine();
  if (ImGui::Button("Refraction"))
  {
    ReflectMode = 1;
  }
  ImGui::SameLine();
  if (ImGui::Button("Fresnel"))
  {
    ReflectMode = 2;
  }

  ImGui::VSliderFloat("##all", ImVec2(50, 100), &eta, 1.f, 100.f, "%.6f\nEta\nOverall", 5);
  ImGui::SameLine();
  ImGui::VSliderFloat("##r", ImVec2(50, 100), &etar, 1.f, 100.f, "%.6f\nEtaR", 5);
  ImGui::SameLine();
  ImGui::VSliderFloat("##g", ImVec2(50, 100), &etag, 1.f, 100.f, "%.6f\nEtaG", 5);
  ImGui::SameLine();
  ImGui::VSliderFloat("##b", ImVec2(50, 100), &etab, 1.f, 100.f, "%.6f\nEtaB", 5);
  ImGui::SameLine();
  ImGui::VSliderFloat("##pow", ImVec2(50, 100), &fpow, 0.f, 10.f, "%.6f\nFresnal\nPower", 5);
  ImGui::SameLine();
  ImGui::VSliderFloat("##mix", ImVec2(50, 100), &mix, 0.f, 10.f, "%.6f\nMix", 5);


  if (ImGui::Button("Air"))
  {
    eta = 1.0002930f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Hydrogen"))
  {
    eta = 1.000132f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Water"))
  {
    eta = 1.333f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Diamond"))
  {
    eta = 2.42f;
  }

  if (ImGui::Button("Olive Oil"))
  {
    eta = 1.47f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Ice"))
  {
    eta = 1.31f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Quartz"))
  {
    eta = 1.46f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Acrylic"))
  {
    eta = 1.49f;
  }

}

void GUI::LightsMenu()
{
  ImGui::SliderInt("Num Lights", &currObjs, 0, MAXLIGHTS);
  ImGui::InputInt("Selected Light", &currLight);
  if (currLight > currObjs)
    currLight = 1;
  if (currLight < 1)
    currLight = currObjs;

  Light* light = &(lights[currLight - 1]);

  const char* types[] = { "Point", "Directional", "Spotlight" };
  static const char* selected = types[0];
  if (ImGui::BeginCombo("Light Selector", selected))
  {
    int size = sizeof(types) / sizeof(int*);
    for (int i = 0; i < size; ++i)
    {
      if (selected != types[i])
      {
        if (ImGui::Selectable(types[i]))
        {

          if (!strcmp(types[i], "Point"))
          {
            lights[currLight - 1].type = (int)Point;
          }
          else if (!strcmp(types[i], "Directional"))
          {
            lights[currLight - 1].type = (int)Directional;
          }
          else
          {
            lights[currLight - 1].type = (int)Spotlight;
          }
        }
      }

    }

    ImGui::EndCombo();
  }
  selected = types[(int)light->type];

  ImGui::VSliderFloat("##0", ImVec2(30, 100), &ga,0.f, 10.f, "%.2f\nGa", 14);
  ImGui::SameLine();                
  ImGui::VSliderFloat("##1", ImVec2(30, 100), &light->ka, 0.f, 10.f, "%.2f\nKa",14);
  ImGui::SameLine();                                                        
  ImGui::VSliderFloat("##2", ImVec2(30, 100), &light->kd, 0.f, 10.f, "%.2f\nKd",14);
  ImGui::SameLine();                                                        
  ImGui::VSliderFloat("##3", ImVec2(30, 100), &light->ks, 0.f, 10.f, "%.2f\nKs",14);
  ImGui::SameLine();                                                        
  ImGui::VSliderFloat("##4", ImVec2(30, 100), &light->ns, 0.f, 100.f, "%.2f\nNs",14);
  ImGui::SameLine();
  ImGui::VSliderFloat("##5", ImVec2(30, 100), &c1, 0.f, 10.f, "%.2f\nC1",14);
  ImGui::SameLine();                                                        
  ImGui::VSliderFloat("##6", ImVec2(30, 100), &c2, 0.f, 10.f, "%.2f\nC2",14);
  ImGui::SameLine();                                                        
  ImGui::VSliderFloat("##7", ImVec2(30, 100), &c3, 0.f, 10.f, "%.2f\nC3",14);

  ImGui::SetNextItemWidth(125);
  ImGui::DragFloat("##8", &light->outerAngles, 1.f, 0.f, 100.f, "%.2f\nOuter Angle");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(125);
  ImGui::DragFloat("##9", &light->innerAngle, 1.f, 0.f, 100.f, "%.2f\nInner Angle");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(125);
  ImGui::DragFloat("##10", &light->falloff, 1.f, 0.f, 100.f, "%.2f\nFalloff");


  ImGui::ColorEdit3("Global Ambiant Color", &gamb[0]);
  ImGui::ColorEdit3("Local Ambiant Color", &(light->Ia[0]));
  ImGui::ColorEdit3("Diffuse Color", &(light->Id[0]));
  ImGui::ColorEdit3("Specular Color", &(light->Is[0]));
  ImGui::ColorEdit3("Emissive Color", &emm[0]);
  ImGui::ColorEdit3("Fog Color", &fog[0]);

}

void GUI::render()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      /*if (ImGui::MenuItem("Open"))
      {
        fileOpen = !fileOpen;
      }*/
      if (ImGui::MenuItem("Options"))
      {
        optionsOpen = !optionsOpen;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  if (fileOpen)
  {
    /*ImGui::OpenPopup("FileName");
    if (ImGui::BeginPopup("FileName"))
    {
      ImGui::InputText("File Name", fileName, 20);
      ImGui::SameLine();
      if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || ImGui::Button("Open"))
      {
        mod[8]->Reset(fileName);
        fileOpen = false;
      }
      ImGui::EndPopup();
    }*/
  }

  if (optionsOpen)
  {
    if (ImGui::Begin("Options"))
    {
      if (ImGui::BeginTabBar("Tabs"))
      {
        if (ImGui::BeginTabItem("Model"))
        {
          tab = models_;
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Shader"))
        {
          tab = shaders_;
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Light"))
        {
          tab = lights_;
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Scene"))
        {
          tab = scene_;
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
      if(tab == models_)
      {
      ImGui::Text("Draw Mode");
      static int e = 0;
      if (ImGui::RadioButton("Fill", &e, 0))
      {
        rendermode = solid_;
      }
      ImGui::SameLine();
      if (ImGui::RadioButton("Wireframe", &e, 1))
      {
        rendermode = wireframe_;
      }
      ImGui::SameLine();
      if (ImGui::RadioButton("Points", &e, 2))
      {
        rendermode = points_;
      }

      ImGui::NewLine();

      ImGui::Checkbox("Face Norms", &faceNormals);
      ImGui::ColorEdit3("Face Norms Color", &(bunny->GetFaceNormColor()[0]));

      ImGui::Checkbox("Vertex Norms", &vertexNormals);
      ImGui::ColorEdit3("Vertex Norms Color", &(bunny->GetVertexNormColor()[0]));


      ImGui::NewLine();

      

      

      
    }
      else if(tab == shaders_)
      {
        ShadersMenu();
      }
      else if (tab == lights_)
      {
        LightsMenu();
      }
      else if (tab == scene_)
      {
        SceneMenu();
      }

    }
    ImGui::End();
  }
  //ImGui::ShowDemoWindow();
  ImGui::EndFrame();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::ResetShader(GLuint id)
{
    glDeleteProgram(id);
    if (id == phongShading)
    {
      phongShading = LoadShaders("../Common/shaders/PhongShading.vert",
        "../Common/shaders/PhongShading.frag");
      id = phongShading;
      bunny->GetProgram() = id;
    }
    else if (id == phongLight)
    {
      phongLight = LoadShaders("../Common/shaders/PhongLighting.vert",
        "../Common/shaders/PhongLighting.frag");
      id = phongLight;
    }

    currShader = id;
}
