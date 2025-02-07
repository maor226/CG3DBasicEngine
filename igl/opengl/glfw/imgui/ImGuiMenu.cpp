// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2018 Jérémie Dumas <jeremie.dumas@ens-lyon.org>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
////////////////////////////////////////////////////////////////////////////////
//#include "ImGuiMenu.h"
//#include "ImGuiHelpers.h"
#include <igl/project.h>
#include "ImGuiHelpers.h"

#include "ImGuiMenu.h"
#include "../imgui.h"
#include "igl/opengl/glfw/imgui/imgui_impl_glfw.h"
#include "igl/opengl/glfw/imgui/imgui_impl_opengl3.h"

//#include <imgui_fonts_droid_sans.h>
//#include <GLFW/glfw3.h>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
#define BACKGROUND_DATA 0 
#define  makeMiror( ind,viewer) viewer->SetShapeShader(ind,viewer->data_list[BACKGROUND_DATA]->shaderID);\
                                viewer->SetCubeShapeMaterial(ind,(int)(viewer->data_list[BACKGROUND_DATA]->GetMaterial()));
#define  makeUnMiror( ind,viewer) viewer->SetShapeShader(viewer->shapes[ind].shapeIdx,3);\
                                viewer->SetShapeMaterial(viewer->shapes[ind].shapeIdx,viewer->shapes[ind].materialIdx);

namespace igl
{
namespace opengl
{
namespace glfw
{
namespace imgui
{

IGL_INLINE void ImGuiMenu::init(Display* disp)
{
  // Setup ImGui binding
  if (disp->window)
  {
    IMGUI_CHECKVERSION();
    if (!context_)
    {
      // Single global context by default, but can be overridden by the user
      static ImGuiContext * __global_context = ImGui::CreateContext();
      context_ = __global_context;
    }
    const char* glsl_version = "#version 150";

    ImGui_ImplGlfw_InitForOpenGL(disp->window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    reload_font();
  }

  layers.push_back("Layer 1");
}

IGL_INLINE void ImGuiMenu::reload_font(int font_size)
{
  hidpi_scaling_ = hidpi_scaling();
  pixel_ratio_ = pixel_ratio();
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();
 // io.Fonts->AddFontFromMemoryCompressedTTF(droid_sans_compressed_data,
 //   droid_sans_compressed_size, font_size * hidpi_scaling_);
  io.FontGlobalScale = 1.0 / pixel_ratio_;
}

IGL_INLINE void ImGuiMenu::shutdown()
{
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  // User is responsible for destroying context if a custom context is given
  // ImGui::DestroyContext(*context_);
}

IGL_INLINE bool ImGuiMenu::pre_draw()
{
  glfwPollEvents();

  // Check whether window dpi has changed
  float scaling = hidpi_scaling();
  if (std::abs(scaling - hidpi_scaling_) > 1e-5)
  {
    reload_font();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  return false;
}

IGL_INLINE bool ImGuiMenu::post_draw() {
  //draw_menu(viewer,core);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return false;
}

IGL_INLINE void ImGuiMenu::post_resize(int width, int height)
{
  if (context_)
  {
    ImGui::GetIO().DisplaySize.x = float(width);
    ImGui::GetIO().DisplaySize.y = float(height);
  }
}

// Mouse IO
IGL_INLINE bool ImGuiMenu::mouse_down(GLFWwindow* window, int button, int modifier)
{
  ImGui_ImplGlfw_MouseButtonCallback(window, button, GLFW_PRESS, modifier);
  return ImGui::GetIO().WantCaptureMouse;
}

IGL_INLINE bool ImGuiMenu::mouse_up(GLFWwindow* window,int button, int modifier)
{
  //return ImGui::GetIO().WantCaptureMouse;
  // !! Should not steal mouse up
  return false;
}

IGL_INLINE bool ImGuiMenu::mouse_move(GLFWwindow* window,int mouse_x, int mouse_y)
{
  return ImGui::GetIO().WantCaptureMouse;
}

IGL_INLINE bool ImGuiMenu::mouse_scroll(GLFWwindow* window,float delta_y)
{
  ImGui_ImplGlfw_ScrollCallback(window, 0.f, delta_y);
  return ImGui::GetIO().WantCaptureMouse;
}

// Keyboard IO
IGL_INLINE bool ImGuiMenu::key_pressed(GLFWwindow* window,unsigned int key, int modifiers)
{
  ImGui_ImplGlfw_CharCallback(nullptr, key);
  return ImGui::GetIO().WantCaptureKeyboard;
}

IGL_INLINE bool ImGuiMenu::key_down(GLFWwindow* window, int key, int modifiers)
{
  ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_PRESS, modifiers);
  return ImGui::GetIO().WantCaptureKeyboard;
}

IGL_INLINE bool ImGuiMenu::key_up(GLFWwindow* window,int key, int modifiers)
{
  ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_RELEASE, modifiers);
  return ImGui::GetIO().WantCaptureKeyboard;
}



IGL_INLINE void ImGuiMenu::draw_viewer_menu(igl::opengl::glfw::Viewer *viewer, std::vector<igl::opengl::Camera*> &camera,Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos)
{
    bool* p_open = NULL;
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = true;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    
    ImGui::Begin(
        "Viewer", p_open,
        window_flags
    );

    ImGui::SetWindowPos(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::End();
    no_move = true;
    no_resize = true;
    
    ImGui::Begin(
        "Viewer", p_open,
        window_flags
    );
    char  * b_name = "Play";
    if(viewer->IsActive()){
      b_name = "Stop";
      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(168,50,70,255));
    }
    else
      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50,168,105,255));
    
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;

    int length = viewer->isActive ? 200 : -1;
    if(ImGui::Button(b_name, ImVec2(length, 0)))
    {
      if (viewer->IsActive())
        viewer->Deactivate();
      else
        viewer->Activate();
    }
    ImGui::PopStyleColor();

    if(viewer->isActive) {
      ImGui::End();
      return;
    }

    if(ImGui::SliderFloat("delay", &viewer->delayVal ,0.f, 1.f)){
       viewer->ChangePickedShapeDelay();
    }
    // Layer
    if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_None)) {
      float w = ImGui::GetContentRegionAvailWidth();
      float p = ImGui::GetStyle().FramePadding.x;

    if (ImGui::Button("Add##Layers", ImVec2((w-p), 0))){
          viewer->show_layer.push_back(new bool(true));
          stringstream s("");

          s << "Layer  ";
          s << (layers.size() + 1);
          layers.push_back(s.str());
    }

    if (ImGui::CollapsingHeader("Hide/Unhide##Layers", ImGuiTreeNodeFlags_DefaultOpen)) {
      float w_i = ImGui::GetContentRegionAvailWidth();
      float p_i = ImGui::GetStyle().FramePadding.x;
      
      for (int i = 0; i < viewer->show_layer.size(); i++){
      std::stringstream s("");
      s << "Layer ";
      s << i + 1;
      s << "##Hide/Unhide##Layers";

      if (ImGui::Checkbox(s.str().c_str(),viewer->show_layer[i])){
        for(int j = 0 ; j < viewer->shapes.size() ; j++) {
          Shape &s = viewer->shapes[j];
          if(s.layer == i) {
            viewer->data_list[s.shapeIdx]->hide = !(*(viewer->show_layer[i]));
          }
        }
      }
    }
    }
    // ImGui::SameLine(0, p);
    
     if (ImGui::CollapsingHeader("Select Layer##Layers", ImGuiTreeNodeFlags_DefaultOpen)) {
      if(ImGui::Button("Change Layer", ImVec2((w-p), 0))) {
        viewer->changePickedLayer();
      }
      float w_i = ImGui::GetContentRegionAvailWidth();
      float p_i = ImGui::GetStyle().FramePadding.x;
      if(ImGui::ListBox("##Select Layer##Layers", &(viewer->layer_index), layers)) {}
     }
  }
  //material 
  if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_None))
  {
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    if (ImGui::Button("Add##Materials", ImVec2((w-p), 0)))
    {
       viewer->open_dialog_load_texture();
    }
    if (ImGui::Button("Change##Materials", ImVec2((w-p), 0))) {
      viewer->ChangePickedShapeMaterial();
    }
    if (ImGui::Button("Tugle Miror ##Materials", ImVec2((w-p), 0))) {
        for (int i = 0; i < viewer->shapes.size(); i++)
        {
          if(*(viewer->shapes[i].picked)){
            if(!*(viewer->shapes[i].isMiror)){
            *(viewer->shapes[i].isMiror) = true;
            makeMiror((viewer->shapes[i].shapeIdx),viewer);
            }else{
            *(viewer->shapes[i].isMiror) = false;
            viewer->SetShapeShader(viewer->shapes[i].shapeIdx,3);
            viewer->SetShapeMaterial(viewer->shapes[i].shapeIdx,viewer->shapes[i].materialIdx);
            }
          }
        }
        
    }
    
    // if (ImGui::Button("Make Unmiror ##Materials", ImVec2((w-p), 0))) {
    //     for (int i = 0; i < viewer->shapes.size(); i++)
    //     {
    //       if(*(viewer->shapes[i].picked)){
    //         *(viewer->shapes[i].isMiror) = false;
    //         viewer->SetShapeShader(viewer->shapes[i].shapeIdx,3);
    //         viewer->SetShapeMaterial(viewer->shapes[i].shapeIdx,viewer->shapes[i].materialIdx);
    //       }
    //     }
    // }
   
    if(ImGui::ListBox("##Materials", &viewer->material_idx, viewer->material_names)) {

    }

  }
  if (ImGui::CollapsingHeader("Blur", ImGuiTreeNodeFlags_None))
  {
    if(viewer->single_picked_shape_idx!=-1){
      int i =viewer->shapes[viewer->single_picked_shape_idx].shapeIdx;
      if(ImGui::Checkbox("Toggle Blur ##Materials", &(viewer->data_list[i]->is_blur))) {}
      if(ImGui::SliderFloat("Radius", &viewer->data_list[i]->blur_radius ,0.f, 10.f)) {}
      if(ImGui::SliderFloat("Resolution", &viewer->data_list[i]->blur_resolution ,0.f, 200.f)) {}
      }
  }
  
  
  if (ImGui::CollapsingHeader("Transparent", ImGuiTreeNodeFlags_None))
  {
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    if (ImGui::Button("toggle transperent ##Tranperent", ImVec2((w-p), 0))) {
      if(viewer->single_picked_shape_idx!=-1){
        viewer->data_list[viewer->shapes[viewer->single_picked_shape_idx].shapeIdx]->isTransfetent = !viewer->data_list[viewer->shapes[viewer->single_picked_shape_idx].shapeIdx]->isTransfetent;
        viewer->data_list[viewer->shapes[viewer->single_picked_shape_idx].shapeIdx]->alpha = viewer->alpha;
      }
    }
    if(ImGui::SliderFloat("alpha ##Transparent", &viewer->alpha ,0.f, 1.f)){
       if(viewer->single_picked_shape_idx!=-1){
        viewer->data_list[viewer->shapes[viewer->single_picked_shape_idx].shapeIdx]->alpha = viewer->alpha;
      }
    }
  }
   if (ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_None))
  {
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    // if (ImGui::Button("Add##Background", ImVec2((w-p), 0)))
    // {
    //    viewer->open_dialog_load_cube_texture();
    // }
    
  if (ImGui::Button("Change##Background", ImVec2((w-p), 0)))
    {
      std::cout << viewer->background_idx <<"\n";
      viewer->SetCubeShapeMaterial(0,viewer->background_idx);
      for (int i = 0; i < viewer->shapes.size(); i++)
      {
        if(*(viewer->shapes[i].isMiror))
        {
          makeMiror((viewer->shapes[i].shapeIdx),viewer)
        }
      }
    }
    if(ImGui::ListBox("##Background", &viewer->background_idx, viewer->cube_material_names)) {
      
    }
  }

  if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None))
  {
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    if(!switch_camera_mode && !viewer->isActive)
      if(ImGui::Button("Split Screen##Camera", ImVec2((w-p), 0))) {
        viewer->splitScreenToggle = true;
      }
    if(!viewer->splitScreenMode) {
      if(viewer->animation_camera_active) {
        if (ImGui::Button("Switch##Camera", ImVec2((w-p), 0))) {
            if(!viewer->isActive) {
              viewer->switch_cameras = true;
              switch_camera_mode = !switch_camera_mode;
            }
        }
      }
      else if (ImGui::Button("Add##Camera", ImVec2((w-p), 0))) 
        {
          if(!viewer->isActive)
            viewer->animation_camera_active = true;
        }
    }

  }

  // Mesh
  if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_None))
  {
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    if (ImGui::Button("Load##Mesh", ImVec2((w-p), 0)))
    {
        int savedIndx = viewer->selected_data_index;
       // viewer->selected_data_index = viewer->parents.size();
       // viewer->AddShape(viewer->xCylinder,-1,viewer->TRIANGLES);
        viewer->open_dialog_load_mesh();
      if (viewer->data_list.size() > viewer->parents.size())
      {
          
          viewer->parents.push_back(-1);
          viewer->SetShapeViewport(viewer->selected_data_index, 0);
          viewer->SetShapeShader(viewer->selected_data_index, 2);
          viewer->SetShapeMaterial(viewer->selected_data_index,0);
          //viewer->data_list.back()->set_visible(false, 1);
          //viewer->data_list.back()->set_visible(true, 2);
          viewer->data_list.back()->UnHide();
          viewer->data_list.back()->show_faces = 3;
          viewer->data()->mode = viewer->TRIANGLES;
          viewer->selected_data_index = savedIndx;
      }
    }

    if (ImGui::CollapsingHeader("Select Shapes##Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
      float w_i = ImGui::GetContentRegionAvailWidth();
      float p_i = ImGui::GetStyle().FramePadding.x;
      
      for (int i = 0; i < viewer->picked_shapes.size(); i++){
        std::stringstream s("");
        s << i + 1;
        s << " ";
        s << viewer->shape_names[i];

        if (ImGui::Checkbox(s.str().c_str(),viewer->picked_shapes[i])) {
          viewer->changePickedShape();
        }
      }
    }
  }

  if(ImGui::CollapsingHeader("Bezier", ImGuiTreeNodeFlags_None)) {
    if (ImGui::Button("Add Shape", ImVec2((w-p), 0))){
            viewer->AddBezierShape();
    }
    if (ImGui::Checkbox("move camera", &viewer->move_camera)) {
      if(viewer->isActive )
        viewer->move_camera = !viewer->move_camera;
    }
    if (ImGui::Button("update camera", ImVec2((w-p), 0))){
            viewer->update_camera_bezier();
    }
  }
  if(ImGui::Checkbox("Toggle Fog", &(viewer->isFog))) {}

  ImGui::End();
}

IGL_INLINE float ImGuiMenu::pixel_ratio()
{
  // Computes pixel ratio for hidpi devices
  int buf_size[2];
  int win_size[2];
  GLFWwindow* window = glfwGetCurrentContext();
  glfwGetFramebufferSize(window, &buf_size[0], &buf_size[1]);
  glfwGetWindowSize(window, &win_size[0], &win_size[1]);
  return (float) buf_size[0] / (float) win_size[0];
}

IGL_INLINE float ImGuiMenu::hidpi_scaling()
{
  // Computes scaling factor for hidpi devices
  float xscale, yscale;
  GLFWwindow* window = glfwGetCurrentContext();
  glfwGetWindowContentScale(window, &xscale, &yscale);
  return 0.5 * (xscale + yscale);
}


} // end namespace
} // end namespace
} // end namespace
} // end namespace
