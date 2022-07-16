#include "Project.h"
#include <iostream>


static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

Project::Project()
{
}

//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

void Project::Init()
{		
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };
	
	AddShader("shaders/pickingShader");
	// AddShader("shaders/cubemapShader");
	AddShader("shaders/bezierShader");
	AddShader("shaders/basicShaderTex");
	AddShader("shaders/basicShader");
	
	AddTexture("textures/plane.png",2);
	AddTexture("textures/cubemaps/Daylight Box_", 3);
	AddTexture("textures/grass.bmp", 2);
	//AddTexture("../res/textures/Cat_bump.jpg", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);

	
	AddShape(Cube, -2, TRIANGLES,1);
	AddShape(zCylinder, -1, TRIANGLES);
	AddShape(yCylinder, 1, TRIANGLES);
	AddShape(xCylinder, 2, TRIANGLES);
	AddShape(Axis, -1, LINES);
	AddShape(Plane,1,TRIANGLES);
	//AddShapeFromFile("../res/objs/Cat_v1.obj", -1, TRIANGLES);
	
	SetShapeShader(0, 1);
	SetShapeShader(1, 3);
	SetShapeShader(2, 3);
	SetShapeShader(3, 3);
	SetShapeShader(4, 3);
	SetShapeShader(5,1);


	SetShapeMaterial(1, 0);
	SetShapeMaterial(2, 0);	
	SetShapeMaterial(3, 0);	
	SetShapeMaterial(4, 0);
	SetShapeMaterial(5, 1);
	SetShapeMaterial(0, 1);


	selected_data_index = 0;
	float cylinderLen = 1.6f;
	float s = 60;
	ShapeTransformation(scaleAll, s,0);
	selected_data_index = 5;
	ShapeTransformation(scaleAll, s,0);

	selected_data_index = 1;
	data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	ShapeTransformation(zTranslate, cylinderLen / 2.0, 1);
	
	selected_data_index = 2;
	ShapeTransformation(zTranslate, cylinderLen , 1);
	data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	
	selected_data_index = 3;
	ShapeTransformation(zTranslate, cylinderLen, 1);
	data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));

	selected_data_index = 0;
	SetShapeStatic(0);
	selected_data_index = 5;
	SetShapeStatic(5);
	


	//SetShapeViewport(6, 1);
//	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;


	s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	s->SetUniform4f("coeffs",3,2,2,1);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
		// materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 60 / 100.0f, 99 / 100.0f, 0.5f);
	//textures[0]->Bind(0);
	
	// if(data_list[shapeIndx]->type == Axis){
	// 	igl::opengl::ViewerData * data  = data_list[shapeIndx];
	// 	for (float t =0 ; t<=1 ;t+=0.001)
	// 	{
	// 		float p = t+=0.001;
	// 		data ->set_edges(Eigen::RowVector3d(t,(t*(1+t*(1+t))),0),Eigen::RowVector3d(p,(p*(1+p*(1+p))),0),Eigen::RowVector3d(1/2,1/2,1/2));
	// 	}
	// }
	
	

	// s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else 
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}


void Project::WhenRotate()
{
}

void Project::WhenTranslate()
{
}

void Project::Animate() {
	int t=0,dt=0,segment=0; //todo make global
    if(isActive)
	{
		int maxSegmentNum = 2;//=((Bezier1D*)data_list[currIndx])->GetSegmentsNum();
		
		t+=dt;

		if(t>1 && segment < maxSegmentNum ){
			segment ++;
			t=dt;
		}
		else if(t>1){
			segment = 0 ;
			t = 1;
			dt = -dt;
		}
		else if(t<0 &&segment <maxSegmentNum){
			segment++;
			t=1;
		}
		else if (t<0){
			segment=0;
			t=0;
			dt=-dt;
		}

		// if(dt<0)
		// 	data_list[last]->MyTranslate(((Bezier1D*)data_list[curIndx])->GetVelosity(segment,1-t,dt),1); //todo getVelosity === שיפוע
		// else
		// 	data_list[last]->MyTranslate(((Bezier1D*)data_list[curIndx])->GetVelosity(segment,t,dt),1);

	}
	else{
		t=0;
		segment =0;
		dt = std::abs(dt);
	}
}

float Project::GetVelosity(int segment, float t, float dt){

	return 0.0;
}


void Project::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Project::~Project(void)
{
}


int Project::IsPicked(int x, int y){
	return pickedPoint;
}
void Project::UnPicked(){
	pickedPoint = -1;
	isPicked = false;
}

