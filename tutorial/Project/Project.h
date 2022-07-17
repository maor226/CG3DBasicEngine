#pragma once
#include "igl/opengl/glfw/Viewer.h"

#define bezier(t, p1, p2, p3, p4) (pow((1 - t),3)*p1) + (3* pow((1-t), 2) * t * p2) + (3* pow(t, 2) * (1 - t) * p3) + pow(t,3) * p4
#define POINTS_NUM 7
#define Radius 0.22
#define NUM_POL (POINTS_NUM - 1)/3

class Project : public igl::opengl::glfw::Viewer
{
private:
	Eigen::Vector4f bez_points[POINTS_NUM];
	
public:
	int pickedPoint=-1;
	bool isPicked = false; 
	Project();
//	Project(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
	float GetVelosity(int segment, float t, float dt);
	int IsPicked(float x, float y);
	void UnPicked();
	void SetPicked(float x,float y);
	void drawBezier(int index);
	void drawSection(int shapeIndx ,Eigen::Vector4f p1,Eigen::Vector4f p2,Eigen::Vector4f p3,Eigen::Vector4f p4);

	~Project(void);
};


