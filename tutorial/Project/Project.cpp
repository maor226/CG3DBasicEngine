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
	//AddShapeFromFile("../res/objs/Cat_v1.obj", -1, TRIANGLES);
	
	SetShapeShader(1, 2);
	SetShapeShader(2, 3);
	SetShapeShader(3, 3);
	SetShapeShader(4, 3);


	SetShapeMaterial(1, 0);
	SetShapeMaterial(2, 0);	
	SetShapeMaterial(3, 0);	
	SetShapeMaterial(4, 0);

	SetShapeMaterial(0, 1);


	selected_data_index = 0;
	float cylinderLen = 1.6f;
	float s = 60;
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
	s->SetUniform4f("coeffs",5,-1,3,1);
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
	int t,dt,segment; //todo make global
    if(isActive)
	{
		int maxSegmentNum;//=((Bezier1D*)data_list[currIndx])->GetSegmentsNum();
		
		t+=dt;

		if(t>1 && segment< maxSegmentNum -1){
			segment ++;
			t=dt;
		}
		else if(t>1){
			segment = 0 ;
			t = 1;
			dt = -dt;
		}
		else if(t<0 &&segment <maxSegmentNum-1){
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

