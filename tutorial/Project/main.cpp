#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "InputManager.h"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1600;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 45.0f;
	const float NEAR = 1.0f;
	const float FAR = 120.0f;
	const int infoIndx = 2; 
	std::list<int> x, y;
	x.push_back(DISPLAY_WIDTH/2);
	x.push_back(DISPLAY_WIDTH);
	y.push_back(DISPLAY_HEIGHT);
    Display disp = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OPENGL");
    igl::opengl::glfw::imgui::ImGuiMenu* menu = new igl::opengl::glfw::imgui::ImGuiMenu();
    Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, NEAR, FAR);

	Project *scn = new Project();  //initializing scene

	//rndr->AddViewport()
    Init(disp,menu); 			//adding callback functions
	scn->Init();    			//adding shaders, textures, shapes to scene
    rndr->Init(scn,x,y,1,menu); // adding scene and viewports to the renderer
	//rndr->draw; // info 3 change camera to 1

	#define edit_viewport 1
	disp.SetRenderer(rndr);
	rndr->AddViewport(0,DISPLAY_HEIGHT/2,DISPLAY_WIDTH /2 , DISPLAY_HEIGHT/2); // added viewport 3
	rndr->CopyDraw(edit_viewport, rndr->viewport, sten_viewport); //added display info 4
	rndr->ClearDrawFlag(sten_drawinfo, rndr->toClear);
	rndr->SetDrawFlag(sten_drawinfo, rndr->blend | rndr->inAction2 | rndr->scissorTest);
	rndr->AddDraw(sten_viewport, 0, 1, 0, rndr->stencil2 | rndr->stencilTest | rndr->depthTest | rndr->scaleAbit | rndr->onPicking);

	rndr->AddViewport(0, 0, DISPLAY_WIDTH /2, DISPLAY_HEIGHT/2); // viewport 4
	rndr->CopyDraw(2, rndr->viewport, 4);


    disp.launch_rendering(rndr);

	delete scn;
	delete rndr;
	if(menu)
	delete menu;
	return 0;
}

