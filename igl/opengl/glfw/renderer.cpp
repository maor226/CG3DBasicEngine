#include "renderer.h"
#include <GLFW/glfw3.h>

#include <igl/unproject_onto_mesh.h>
#include <utility>
#include "igl/look_at.h"

//#include <Eigen/Dense>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


Renderer::Renderer(float angle, float relationWH, float near, float far)
{

    callback_init = nullptr;
    callback_pre_draw = nullptr;
    callback_post_draw = nullptr;
    callback_mouse_down = nullptr;
    callback_mouse_up = nullptr;
    callback_mouse_move = nullptr;
    callback_mouse_scroll = nullptr;
    callback_key_down = nullptr;
    callback_key_up = nullptr;

    callback_init_data = nullptr;
    callback_pre_draw_data = nullptr;
    callback_post_draw_data = nullptr;
    callback_mouse_down_data = nullptr;
    callback_mouse_up_data = nullptr;
    callback_mouse_move_data = nullptr;
    callback_mouse_scroll_data = nullptr;
    callback_key_down_data = nullptr;
    callback_key_up_data = nullptr;
    glLineWidth(5);
    cameras.push_back(new igl::opengl::Camera(angle, relationWH, near, far));
    isPressed = false;
    isMany = false;
    xold = 0;
    yold = 0;
    xrel = 0;
    yrel = 0;
    zrel = 0;
    currentViewport = 0;
    isPicked = false;
}



void Renderer::Clear(float r, float g, float b, float a,unsigned int flags)
{
    glClearColor(r, g, b, a);

    glClear(GL_COLOR_BUFFER_BIT | ((GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) & flags));
}

void Renderer::SwapDrawInfo(int indx1, int indx2)
{
    DrawInfo* info = drawInfos[indx1];
    drawInfos[indx1] = drawInfos[indx2];
    drawInfos[indx2] = info;
}

IGL_INLINE void Renderer::draw_by_info(int info_index){
    DrawInfo* info = drawInfos[info_index];
    buffers[info->bufferIndx]->Bind();
    glViewport(viewports[info->viewportIndx].x(), viewports[info->viewportIndx].y(), viewports[info->viewportIndx].z(), viewports[info->viewportIndx].w());
    if (info->flags & scissorTest)
    {
        glEnable(GL_SCISSOR_TEST);
        int x = std::min(xWhenPress, xold);
        int y = std::min(viewports[info->viewportIndx].w() - yWhenPress, viewports[info->viewportIndx].w() - yold);
        glScissor(x, y, std::abs(xWhenPress - xold), std::abs( yWhenPress - yold));
    }
    else
        glDisable(GL_SCISSOR_TEST);

    if (info->flags & stencilTest)
    {
        glEnable(GL_STENCIL_TEST);
        if (info->flags & passStencil)
        {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        }
        else
        {
            if (info->flags & stencil2)
            {
                glStencilFunc(GL_EQUAL, 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
            }
            else
            {
                glStencilFunc(GL_EQUAL, 0, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            }
        }
    }
    else
        glDisable(GL_STENCIL_TEST);

    if (info->flags & depthTest){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LESS); //for rendering from the farest to the neerest     
    }
    else
        glDisable(GL_DEPTH_TEST);

    if (info->flags & blend)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    }
    else
        glDisable(GL_BLEND);

    Eigen::Matrix4f Proj = cameras[info->cameraIndx]->GetViewProjection().cast<float>();
    Eigen::Matrix4f View = cameras[info->cameraIndx]->MakeTransScaled().inverse().cast<float>();

    if (info->flags & toClear)
    {
        if (info->flags & blackClear)
            Clear(0, 0, 0, 0,info->flags);
        else
            Clear(info->Clear_RGBA.x(), info->Clear_RGBA.y(), info->Clear_RGBA.z(), info->Clear_RGBA.w(),info->flags);
    }

    scn->Draw(info->shaderIndx, Proj, View, info->viewportIndx, info->flags,info->property_id);
}

IGL_INLINE void Renderer::draw( GLFWwindow* window)
{
	using namespace std;
	using namespace Eigen;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	int width_window, height_window;
	glfwGetWindowSize(window, &width_window, &height_window);
	
	auto highdpi_tmp = (width_window == 0 || width == 0) ? highdpi : (width / width_window);

	if (fabs(highdpi_tmp - highdpi) > 1e-8)
	{
		post_resize(window,width, height);
		highdpi = highdpi_tmp;
	}
	if (menu)
	{
		menu->pre_draw();
		menu->callback_draw_viewer_menu();
    }
    int indx = 0;
    for (auto& info : drawInfos)
    {
        if (!(info->flags & (inAction | inAction2)) || ((info->flags & inAction2) && !(info->flags & stencilTest) && isPressed && !isPicked) || ((info->flags & inAction2) && (info->flags & stencilTest)  && isPicked ))
            draw_by_info(indx);
        indx++;
    }

    if (menu)
	{
		menu->post_draw();

	}


}

void Renderer::SetScene(igl::opengl::glfw::Viewer* viewer)
{
	scn = viewer;
}


void Renderer::UpdatePosition(double xpos, double ypos)
{
	xrel = xold - xpos;
	yrel = yold - ypos;
	xold = xpos;
	yold = ypos;
}

void Renderer::TranslateCamera(int idx, Eigen::Vector3d amt, bool prerotation)
{
    if(idx == edit_camera || idx == animate_camera) {
        reserve_c[idx / 2]->MyTranslate(amt.cast<double>(), prerotation); //solves both cases
    }

	cameras[idx]->MyTranslate(amt.cast<double>(), prerotation);
}


void Renderer::TranslateCamera(Eigen::Vector3f amt)
{
	cameras[0]->MyTranslate(amt.cast<double>(), true);
}

//void Renderer::RotateCamera(float amtX, float amtY)
//{
//	core().camera_eye = core().camera_eye + Eigen::Vector3f(0,amtY,0);
//	Eigen::Matrix3f Mat;
//		Mat << cos(amtY),0,sin(amtY),  0, 1, 0 ,  -sin(amtY), 0, cos(amtY) ;
//	core().camera_eye = Mat* core().camera_eye;
//
//}


float Renderer::UpdatePosition(float xpos, float ypos)
{
    xrel = xold - xpos;
    yrel = yold - ypos;
    xold = xpos;
    yold = ypos;
    return yrel;
}

void Renderer::UpdatePress(float xpos, float ypos)
{
    xWhenPress = xpos;
    yWhenPress = ypos;
}

void Renderer::AddCamera(const Eigen::Vector3d& pos, float fov, float relationWH, float zNear, float zFar, int infoIndx)
{
    if (infoIndx > 0 && infoIndx < drawInfos.size())
    {
        //drawInfos[infoIndx]->SetCamera(cameras.size());
    }
    cameras.push_back(new igl::opengl::Camera(fov, relationWH, zNear, zFar));
    cameras.back()->MyTranslate(pos, false);
}

void Renderer::AddViewport(int left, int bottom, int width, int height)
{
    viewports.emplace_back(Eigen::Vector4i(left, bottom, width, height));
    glViewport(left, bottom, width, height);

}

void Renderer::AddDraw(int viewportIndx, int cameraIndx, int shaderIndx, int buffIndx, unsigned int flags)
{
    drawInfos.emplace_back(new DrawInfo(viewportIndx, cameraIndx, shaderIndx, buffIndx, flags,next_property_id));
    next_property_id <<= 1;
}

// void Renderer::UpdateDrawCamera(int infoIndx, int new_camera) {

// }

void Renderer::CopyDraw(int infoIndx, int property, int indx)
{
    DrawInfo* info = drawInfos[infoIndx];
    switch (property)
    {
        case non:
            drawInfos.emplace_back(new DrawInfo(info->viewportIndx, info->cameraIndx, info->shaderIndx, info->bufferIndx, info->flags,next_property_id));
            break;
        case viewport:
            drawInfos.emplace_back(new DrawInfo(indx, info->cameraIndx, info->shaderIndx, info->bufferIndx, info->flags,next_property_id));
            break;
        case camera:
            drawInfos.emplace_back(new DrawInfo(info->viewportIndx, indx, info->shaderIndx, info->bufferIndx, info->flags,next_property_id));
            break;
        case shader:
            drawInfos.emplace_back(new DrawInfo(info->viewportIndx, info->cameraIndx, indx, info->bufferIndx, info->flags,next_property_id));
            break;
        case buffer:
            drawInfos.emplace_back(new DrawInfo(info->viewportIndx, info->cameraIndx, info->shaderIndx, indx, info->flags,next_property_id));
            break;
    }
    next_property_id <<= 1;
}

void Renderer::SetViewport(int left, int bottom, int width, int height, int indx)
{
    viewports[indx] = Eigen::Vector4i(left, bottom, width, height);
    glViewport(left, bottom, width, height);
}



Renderer::~Renderer()
{
	//if (scn)
	//	delete scn;
}


bool Renderer::Picking(int x, int y){
    Eigen::Vector4d pos;

    unsigned char data[4];
    int viewport[4];
    ActionDraw(0);

    glGetIntegerv(GL_VIEWPORT, viewport); //reading viewport parameters

    //cout << (int)viewport[0] << " " << (int)viewport[1] << " " << (int)viewport[2] << " " << (int)viewport[3] << endl;
    glReadPixels(x, viewport[1] + viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    int i = 0;
    isPicked = scn->Picking(data, i); //false
    return isPicked;
}

void Renderer::OutLine()
{
    ActionDraw(0);
}

void Renderer::PickMany(int viewportIndx)
{
    //if (!isPicked)
   // {
        int viewportCurrIndx = 0;
        int xMin = std::min(xWhenPress, xold);
        int yMin = std::min(viewports[viewportCurrIndx].w() - yWhenPress, viewports[viewportCurrIndx].w() - yold);
        int xMax = std::max(xWhenPress, xold);
        int yMax = std::max(viewports[viewportCurrIndx].w() - yWhenPress, viewports[viewportCurrIndx].w() - yold);
		depth = scn->AddPickedShapes(cameras[0]->GetViewProjection().cast<double>() * (cameras[0]->MakeTransd()).inverse(), viewports[viewportCurrIndx], viewportCurrIndx, xMin, xMax, yMin, yMax,viewportIndx);
        if (depth != -1)
        {
            depth = (depth*2.0f - cameras[0]->GetFar()) / (cameras[0]->GetNear() - cameras[0]->GetFar());
            isMany = true;
            isPicked = true;
        }
        else
            depth = 0;

    //}
}

void Renderer::ActionDraw(int viewportIndx)
{
    if (menu)
    {
        menu->pre_draw();
        menu->callback_draw_viewer_menu();
    }
    for (int i = 0; i < drawInfos.size(); i++)
    {
        if ((drawInfos[i]->flags & inAction) && viewportIndx == drawInfos[i]->viewportIndx)
            draw_by_info(i);
    }
    if (menu)
    {
        menu->post_draw();

    }
}
IGL_INLINE void Renderer::resize(GLFWwindow* window,int w, int h)
	{
		post_resize(window,w, h);
	}
// _________________________
// |           *            |
// |           *            |
// |************************|
// |           *            |
// |           *            |
// _________________________

IGL_INLINE void Renderer::post_resize(GLFWwindow* window, int w, int h)
	{
        // hold old windows size
        int x = 0;
        int y = 0;
        for(auto & viewport : viewports){
            x = std::max(x,viewport.x()+viewport.z());
            y = std::max(y,viewport.y()+viewport.w());
        }
        float ratio_x = (float)w/(float)x;
        float ratio_y = (float)h/(float)y;
        std::cout << "called" << std::endl;
        for(auto & viewport : viewports){
            viewport = Eigen::Vector4i((int)((float)viewport.x()*ratio_x),(int)((float)viewport.y()*ratio_y),(int)((float)viewport.z()*ratio_x),(int)((float)viewport.w()*ratio_y));
        }
		if (callback_post_resize)
		{
			callback_post_resize(window, w, h);
		}
	}

void Renderer::changeCamera(int infoIdx, int cameraIdx) {
    drawInfos[infoIdx]->cameraIndx = cameraIdx;
}

void Renderer::MoveCamera(igl::opengl::Camera * c, int type, float amt) {
    switch (type)
    {
        case xTranslate:
            c->MyTranslate( Eigen::Vector3d(amt, 0, 0),1); //MakeTransNoScale was here
            break;
        case yTranslate:
            c->MyTranslate( Eigen::Vector3d(0, amt, 0),1); //MakeTransNoScale was here
            break;
        case zTranslate:
            c->MyTranslate(Eigen::Vector3d(0, 0, amt),1); //MakeTransNoScale was here
            break;
        case xRotate:
            c->MyRotate(Eigen::Vector3d(1, 0, 0), amt);
            break;
        case yRotate:
            c->MyRotate(Eigen::Vector3d(0, 1, 0), amt);
            break;
        case zRotate:
            c->MyRotate(Eigen::Vector3d(0, 0, 1), amt);
            break;
        case scaleAll:
            c->MyScale( Eigen::Vector3d(amt, amt,  amt));
            break;
        default:
            break;
    }
}

void Renderer::MoveCamera(int cameraIndx, int type, float amt)
{
    if(cameraIndx == edit_camera || cameraIndx == animate_camera) {
        MoveCamera(reserve_c[cameraIndx/animate_camera], type, amt); //solves both cases
    }
    MoveCamera(cameras[cameraIndx], type, amt);
}

bool Renderer::CheckViewport(int x, int y, int viewportIndx)
{
    if(scn->splitScreenMode)
        return (viewports[viewportIndx].x() < x && viewports[viewportIndx].w() > y && viewports[viewportIndx].z() + viewports[viewportIndx].x() > x && viewports[viewportIndx].w() - viewports[viewportIndx].y() < y);
    else
        return (viewports[viewportIndx].x() < x && viewports[viewportIndx].w() > y && viewports[viewportIndx].z() + viewports[viewportIndx].x() > x && viewports[viewportIndx].y() < y);
}

bool Renderer::UpdateViewport(int viewport)
{
    if (viewport != currentViewport)
    {
        isPicked = false;
        currentViewport = viewport;
        Pressed();
		scn->UnPick();
        return false;
    }
    return true;
}

void Renderer::MouseProccessing(int button, int mode, int viewportIndx)
{
    //PickMany(viewportIndx);
    
    if (scn->pick || button == 0)
    {
		if(button == 2)
			scn->MouseProccessing(button, zrel, zrel, CalcMoveCoeff(mode & 7, viewports[viewportIndx].w()), cameras[0]->MakeTransd(), viewportIndx);
		else
			scn->MouseProccessing(button, xrel, yrel, CalcMoveCoeff(mode & 7, viewports[viewportIndx].w()), cameras[0]->MakeTransd(), viewportIndx);
    }

}

float Renderer::CalcMoveCoeff(int cameraIndx, int width)
{
    return cameras[cameraIndx]->CalcMoveCoeff(depth,width);
}

unsigned int Renderer::AddBuffer(int infoIndx)
{
    CopyDraw(infoIndx, buffer, buffers.size());

    DrawInfo* info = drawInfos.back();
    info->SetFlags(stencilTest );
 
    info->SetFlags( clearDepth | clearStencil);
    int width = viewports[info->viewportIndx].z(), height = viewports[info->viewportIndx].w();

    unsigned int texId;
    texId = scn->AddTexture(width, height, 0, COLOR);
    scn->AddTexture(width, height, 0, DEPTH);
    buffers.push_back(new igl::opengl::DrawBuffer(width, height, texId));

    return texId;
}

int Renderer::Create2Dmaterial(int infoIndx, int code)
{
    std::vector<unsigned int> texIds;
    std::vector<unsigned int> slots;
    
    unsigned int texId = AddBuffer(infoIndx);
    texIds.push_back(texId);
    slots.push_back(texId);
    texIds.push_back(texId + 1);
    slots.push_back(texId + 1);
    
    materialIndx2D = scn->AddMaterial((unsigned int*)&texIds[0], (unsigned int*)&slots[0], 2);

    return materialIndx2D;
}


void Renderer::SetBuffers()
{
    AddCamera(Eigen::Vector3d(0, 0, 1), 0, 1, 1, 10,2);
    int materialIndx = Create2Dmaterial(1,1);
    scn->SetShapeMaterial(6, materialIndx);
    SwapDrawInfo(2, 3);
}

IGL_INLINE void Renderer::Init(igl::opengl::glfw::Viewer* scene, std::list<int>xViewport, std::list<int>yViewport,int pickingBits,igl::opengl::glfw::imgui::ImGuiMenu *_menu)
{
    scn = scene;
    menu = _menu;
    Eigen::Vector4i viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    buffers.push_back(new igl::opengl::DrawBuffer());
    maxPixX = viewport.z();
    maxPixY = viewport.w();
    
    xViewport.push_front(0);
    yViewport.push_front(0);
    std::list<int>::iterator xit = xViewport.begin();
    int indx = 0;
    auto xval = *(++(++xViewport.begin()));
    auto yval = *(++yViewport.begin());

    float CAMERA_ANGLE = 45.0f, NEAR = 1.0F, FAR = 45.0f;

    viewports.emplace_back(0, yval/2, xval/2, yval/2);
    viewports.emplace_back(xval/2, 0, xval/2, yval);
    viewports.emplace_back(0, 0, xval/2, yval/2);

    //set up reserve for split screen
    reserve_v.emplace_back(0, 0, xval/2, yval); //main viewport
    reserve_v.emplace_back(viewports[1]); // bez viewport just so the vectors will be same length
    reserve_v.emplace_back(0, 0, 0, 0); //animate viewport
    reserve_v.emplace_back(0, 0, xval/2, yval); //stencil viewport
    reserve_v.emplace_back(0, 0, xval/2, yval); //stencil viewport

    reserve_c.push_back(new igl::opengl::Camera(CAMERA_ANGLE, (float)xval/(float)yval/2.0f, NEAR, FAR));
    MoveCamera(0, zTranslate, 10);
    reserve_c.push_back(new igl::opengl::Camera(CAMERA_ANGLE, (float)xval/(float)yval/2.0f, NEAR, FAR));

    AddCamera(Eigen::Vector3d(0, 0, 10), CAMERA_ANGLE,(float)xval/(float)yval/2.0f, NEAR, FAR);
    // pos is vec (0, 0, 10) rotated 30 degrees around y axis
    AddCamera(Eigen::Vector3d(0, 0, 0), CAMERA_ANGLE, (float)xval/(float)yval ,NEAR ,FAR);
    TranslateCamera(animate_camera, Eigen::Vector3d(5, 0, 8.6603), false);
    MoveCamera(animate_camera, yRotate, -70);
    TranslateCamera(animate_camera,Eigen::Vector3d(-4, 0, 0), 0);

    switch_camera = animate_camera_idx;
    //for stencil and picking and shit
    DrawInfo* new_draw_info = new DrawInfo(0, 0, 0, 0,
                                                1 | inAction | depthTest | stencilTest | passStencil | blackClear |
                                                clearStencil | clearDepth | onPicking ,
                                                next_property_id);
    next_property_id <<= 1;
    drawInfos.emplace_back(new_draw_info);

    //editor
    new_draw_info = new DrawInfo(0, 0, 1, 0,
                                                1 | depthTest | clearDepth |stencilTest | passStencil | clearStencil ,next_property_id);
    next_property_id <<= 1;
    drawInfos.emplace_back(new_draw_info);

    //bez curves
    new_draw_info = new DrawInfo(1, 1, 1, 0,
                                                depthTest | clearDepth |stencilTest | passStencil | clearStencil ,next_property_id);
    next_property_id <<= 1;
    drawInfos.emplace_back(new_draw_info);

    //animate
    new_draw_info = new DrawInfo(2, 2, 1, 0,
                                                depthTest | clearDepth |stencilTest | passStencil | clearStencil ,next_property_id);
    next_property_id <<= 1;
    drawInfos.emplace_back(new_draw_info);

    // for (++xit; xit != xViewport.end(); ++xit) {
    //     std::list<int>::iterator yit = yViewport.begin();
    //     for (++yit; yit != yViewport.end(); ++yit)
    //     {
    //         viewports.emplace_back(*std::prev(xit), *std::prev(yit), *xit - *std::prev(xit), *yit - *std::prev(yit));

    //         if ((1 << indx) & pickingBits) {
    //             DrawInfo* new_draw_info = new DrawInfo(indx, indx, 0, 0,
    //                                               1 | inAction | depthTest | stencilTest | passStencil | blackClear |
    //                                               clearStencil | clearDepth | onPicking ,
    //                                               next_property_id);
    //             next_property_id <<= 1;

    //             drawInfos.emplace_back(new_draw_info);
    //         }
    //         DrawInfo* temp = new DrawInfo(indx, indx, 1, 0, (int)(indx < 1) | depthTest | clearDepth ,next_property_id);
    //         next_property_id <<= 1;
    //         drawInfos.emplace_back(temp);
    //         indx++;
    //     }
    // }

    if (menu)
    {
        menu->callback_draw_viewer_menu = [&]()
        {
            // Draw parent menu content
            auto temp = Eigen::Vector4i(0,0,0,0); // set imgui to min size and top left corner
            menu->draw_viewer_menu(scn,cameras,temp, drawInfos);
        };
    }
}
