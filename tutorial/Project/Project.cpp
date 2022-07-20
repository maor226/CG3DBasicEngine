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

Project::Project() {
}

//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}



void Project::Init()
{	float x = 0.5528;
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };
	bez_points[0] = Eigen::Vector4f(-4, 0, 0, 0);
	bez_points[1] = Eigen::Vector4f(-1, x, 0, 0);
	bez_points[2] = Eigen::Vector4f(-x, 1, 0, 0);
	bez_points[3] = Eigen::Vector4f(0, 4, 0, 0);
	bez_points[4] = Eigen::Vector4f(x, 1, 0, 0);
	bez_points[5] = Eigen::Vector4f(1, -x, 0, 0);
	bez_points[6] = Eigen::Vector4f(-4, 0, 0, 0);
	
	AddShader("shaders/pickingShader");
	// AddShader("shaders/cubemapShader");
	AddShader("shaders/bezierShader");
	AddShader("shaders/basicShaderTex");
	AddShader("shaders/basicShader");
	
	AddTexture("textures/plane.png",2);
	AddTexture("textures/cubemaps/kisspng-skybox-texture-mapping-panorama-5b2a44a372ea72.7553873615294967394707.png", 0);
	AddTexture("textures/grass.bmp", 2);
	//AddTexture("../res/textures/Cat_bump.jpg", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);

	


	AddShape(Cube, -2, TRIANGLES);
	SetShapeMaterial(0, 1);
	SetShapeStatic(0);

	// AddShape(zCylinder, -1, TRIANGLES);
	// AddShape(zCylinder, 1, TRIANGLES);
	// AddShape(zCylinder, 2, TRIANGLES);

	AddShape(Axis, -1, TRIANGLES,1);
	SetShapeShader(1, 0);
	selected_data_index = 1;
	ShapeTransformation(scaleAll, 0, 0);
	SetShapeStatic(1);
	//SetShapeMaterial(1, 0);
	AddShape(Plane, -1, TRIANGLES,1);
	SetShapeShader(2,1);
	SetShapeMaterial(2,1);
	selected_data_index = 2;
	ShapeTransformation(scaleAll, 60, 0);
	SetShapeStatic(2);
	// AddShape(Plane, -1, TRIANGLES); //background;
	// SetShapeShader(3, 3);
	// SetShapeMaterial(3, 1);
	// SetShapeStatic(3);
	AddShape(Sphere, -1, TRIANGLES, 0);
	SetShapeShader(3,3);
	SetShapeMaterial(3,2);



	
	//AddShapeFromFile("../res/objs/Cat_v1.obj", -1, TRIANGLES);
	
	// SetShapeShader(0, 1);
	// SetShapeShader(1, 3);
	// SetShapeShader(2, 3);
	// SetShapeShader(3, 3);
	// SetShapeShader(4, 3);
	// SetShapeShader(5, 1);


	// SetShapeMaterial(1, 0);
	// SetShapeMaterial(2, 0);	
	// SetShapeMaterial(3, 0);	
	// SetShapeMaterial(4, 0);
	// SetShapeMaterial(5, 1);
	// SetShapeMaterial(0, 1);


	// selected_data_index = 0;
	// float cylinderLen = 1.6f;
	// float s = 60;
	// ShapeTransformation(scaleAll, s,0);
	// selected_data_index = 5;
	// ShapeTransformation(scaleAll, s,0);

	// selected_data_index = 1;
	// data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	// ShapeTransformation(zTranslate, cylinderLen / 2.0, 1);
	// selected_data_index = 0;
	// float cylinderLen = 1.6f;
	// float s = 60;
	// ShapeTransformation(scaleAll, s,0);
	// selected_data_index = 1;
	// data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	// ShapeTransformation(zTranslate, cylinderLen / 2.0, 1);
	// selected_data_index = 2;
	// ShapeTransformation(zTranslate, cylinderLen , 1);
	// data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	
	// selected_data_index = 3;
	// ShapeTransformation(zTranslate, cylinderLen, 1);
	// data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));


	// selected_data_index = 0;
	// SetShapeStatic(0);
	// selected_data_index = 5;
	// SetShapeStatic(5);
	


	//SetShapeViewport(6, 1);
	//	ReadPixel(); //uncomment when you are reading from the z-buffer
	drawBezier(1);
}
void Project::drawBezier(int shapeIndx){
	data_list[shapeIndx]->clear();
	drawSection(shapeIndx,bez_points[0],bez_points[1], bez_points[2], bez_points[3]);
	drawSection(shapeIndx,bez_points[3],bez_points[4], bez_points[5], bez_points[6]);
}

void Project::drawSection(int shapeIndx ,Eigen::Vector4f p1,Eigen::Vector4f p2,Eigen::Vector4f p3,Eigen::Vector4f p4){
	auto shape = data_list[shapeIndx];
	float y_bez_t = bezier(0,p1[1],p2[1],p3[1],p4[1]);
	float x_bez_t = bezier(0,p1[0],p2[0],p3[0],p4[0]);
	Eigen::RowVector3d vec_t = Eigen::RowVector3d(x_bez_t,y_bez_t,0);
	Eigen::RowVector3d vec_p, helf_vec = Eigen::RowVector3d(1/2,1/2,1/2);
	float p, y_bez_p, x_bez_p;
	for (float t = 0; t < 0.995; t += 0.005) {
			p = t + 0.005; //p is the point after t
		 	y_bez_p = bezier(p,p1[1],p2[1],p3[1],p4[1]); 
		 	x_bez_p = bezier(p,p1[0],p2[0],p3[0],p4[0]); 
			vec_p = Eigen::RowVector3d(x_bez_p,y_bez_p,0);
			shape->add_edges(vec_t,vec_p,helf_vec);
			vec_t = vec_p;
			y_bez_t = y_bez_p;
			x_bez_t = x_bez_p;
	} 
}

Eigen::RowVector3d   velocity(float t, float dt, Eigen::Vector4f p1,Eigen::Vector4f p2,Eigen::Vector4f p3,Eigen::Vector4f p4){
	float y_bez_t = bezier(t,p1[1],p2[1],p3[1],p4[1]);
	float x_bez_t = bezier(t,p1[0],p2[0],p3[0],p4[0]);
	float p = t + dt; //p is the point after t
	float y_bez_p = bezier(p,p1[1],p2[1],p3[1],p4[1]); 
	float x_bez_p = bezier(p,p1[0],p2[0],p3[0],p4[0]); 
	return Eigen::RowVector3d(x_bez_p - x_bez_t, y_bez_p - y_bez_t ,0); 
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
	s->SetUniform4f("coeffs",1,1,1,1);
	s->SetUniform1i("POINTS_NUM", POINTS_NUM);
	s->SetUniform4fv("bez_points", &bez_points[0], POINTS_NUM);


	if(isPicked&&data_list[shapeIndx]->type == Axis){
		drawBezier(shapeIndx);
	}
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
	 //todo make global
	//bez_points[0][1] += 0.1;
    if(isActive)
	{
		int maxSegmentNum = 1;//=((Bezier1D*)data_list[currIndx])->GetSegmentsNum();
		if(t<=1 && t >=0){
			// std::cout << "Animate" <<GetVelosity(segment, t, dt) <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;
			data_list[3]->MyTranslate(GetVelosity(segment, t, dt),1);
			//data_list[3]->MyTranslate(((Bezier1D*)data_list[curIndx])->GetVelosity(segment,1-t,dt),1); //todo getVelosity === שיפוע
			t+=dt;
		}else if(t>=1){
			if(segment == maxSegmentNum){
				std::cout << "Animate 1 1 - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;
				dt = -dt;
				t =1;
				std::cout << "---- - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;

			}else{
				std::cout << "Animate 1 2 - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;

				segment++;
				t = dt;
				std::cout << "---- - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;

			}
		}else if(t<=0){
			if( segment == 0){
				std::cout << "Animate 2 1 - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;

				t = 0;
				dt = -dt;
				std::cout << "---- - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;
			}else{
				std::cout << "Animate 2 2 - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;

				segment--;
				t = 1;
				std::cout << "---- - "  <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;

			}
		}
		else std::cout << "Eror whail t:" <<t <<std::endl;
	}
	else{
		t=0;
		segment =0;
		dt = std::abs(0.01);
	}
}

Eigen::Vector3d Project::GetVelosity(int segment, float t, float dt){
	Eigen::Vector3d v = velocity(t,dt, bez_points[segment*3], bez_points[segment*3+1], bez_points[segment*3+2], bez_points[segment*3+3]);
	if (dt <0)
		return Eigen::Vector3d(v[0],-1*v[1],0);
	return v;
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


int Project::IsPicked(float x, float y){
	x = (x-1200)/100;
	y=(y-400)/-100;
	std::cout << "piced" << x << "," << y << std::endl;
	pickedPoint = -1;
	isPicked = false;
	for(int i= 0 ; i<POINTS_NUM ; i ++){
		if(pow(x-bez_points[i][0],2) +pow(y-bez_points[i][1],2)<Radius*Radius){
			std::cout << "found " << i  << std::endl;
			isPicked =true;
			pickedPoint = i;
			break;
		}
	}
	return pickedPoint;
}

void Project::UnPicked(){
		std::cout << "unpiced" << std::endl;

	pickedPoint = -1;
	isPicked = false;
}

void Project::SetPicked(float x,float y){
	if(isPicked){
		x = (x-1200)/100;
		y=(y-400)/-100;
		std::cout << "setpoints" << pickedPoint<<":"<< x << "," << y << std::endl;

		bez_points[pickedPoint][0]=x;
		bez_points[pickedPoint][1]=y;
	}
}

