#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "imgui/imgui.h"

using namespace std;

	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		if (action == GLFW_PRESS)
		{	
			double x2, y2;
			
			glfwGetCursorPos(window, &x2, &y2);
			rndr->UpdatePress((float)x2, (float)y2);
			if(rndr->Picking((int)x2, (int)y2)) {
				
			}
			if ( scn->IsPicked((float)x2,(float)y2)>=0)
			{
				rndr->UpdatePosition(x2, y2);
				if(button == GLFW_MOUSE_BUTTON_LEFT) {
					rndr->Pressed();
					rndr->PickMany(0);
				}
			}
			else
			{
				rndr->UnPick(0);
			}
		
		}
		else
		{				
			scn->UnPicked();

			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			rndr->UnPick(0);
		}
	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		
		if (rndr->IsPicked())
		{
			rndr->UpdateZpos((int)yoffset);
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
		}
		else
		{
			rndr->MoveCamera(0, rndr->zTranslate, (float)yoffset);
			scn->edit_camera_dist += yoffset;
		}
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);
		scn->SetPicked((float)xpos,(float)ypos);
		
		if (rndr->CheckViewport((int)xpos, (int)ypos, 0))
		{

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany())
					rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);

		}
		else if(rndr->CheckViewport((int)xpos, (int)ypos,1)){
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				
				
			}
			
		}
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

        rndr->resize(window,width,height);
		
	}
	
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		//rndr->FreeShapes(2);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			Eigen::Vector3d mov;
			switch (key)
			{
			case GLFW_KEY_Z:
				if(scn->zoomed_in) {
					mov = scn->zoomOut();
				}
				else {
					double x,y;
					glfwGetCursorPos(window, &x, &y);
					if(!rndr->CheckViewport((int)x, (int)y, edit_viewport))
						break;
					mov = scn->zoomIn(x, y);
				}
				rndr->TranslateCamera(edit_camera, mov, false);
				scn->zoomed_in = !scn->zoomed_in;			
				break;
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
				
			case GLFW_KEY_SPACE:
				if (scn->IsActive())
					scn->Deactivate();
				else
					scn->Activate();
				break;

			case GLFW_KEY_UP:
				rndr->MoveCamera(0, scn->xRotate, 0.05f);
				
				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				rndr->MoveCamera(0, scn->xRotate, -0.05f);
				break;
			case GLFW_KEY_LEFT:
				rndr->MoveCamera(0, scn->xTranslate, -0.25f);
				break;
			case GLFW_KEY_RIGHT:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				rndr->MoveCamera(0, scn->xTranslate, 0.25f);
				break;
			case GLFW_KEY_U:
				rndr->MoveCamera(0, scn->yTranslate, 0.25f);
				break;
			case GLFW_KEY_D:
				rndr->MoveCamera(0, scn->yTranslate, -0.25f);
				break;
			case GLFW_KEY_L:
				rndr->MoveCamera(0, scn->xTranslate, -0.25f);
				break;
			
			case GLFW_KEY_R:
				rndr->MoveCamera(0, scn->xTranslate, 0.25f);
				break;
			
			case GLFW_KEY_B:
				rndr->MoveCamera(0, scn->zTranslate, 0.5f);
				scn->edit_camera_dist += 0.5f;
				break;
			case GLFW_KEY_F:
				rndr->MoveCamera(0, scn->zTranslate, -0.5f);
				scn->edit_camera_dist -= 0.5f;
				break;
			case GLFW_KEY_1:
				std::cout << "picked 1\n";
				scn->selected_data_index = 1;
				break;
			case GLFW_KEY_2:
				std::cout << "picked 2\n";
				scn->selected_data_index = 2;
				break;
			case GLFW_KEY_3:
				std::cout << "picked 3\n";
				scn->selected_data_index = 3;
				break;
			default:
				break;

			}
		}
	}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    menu->init(&display);
}
