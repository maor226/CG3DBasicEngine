#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include <stdlib.h>

#define bezier_1(t, p1, p2, p3, p4) (pow((1 - t),3)*p1) + (3* pow((1-t), 2) * t * p2) + (3* pow(t, 2) * (1 - t) * p3) + pow(t,3) * p4
#define Radius 0.22

class Project : public igl::opengl::glfw::Viewer
{
private:
	int picked_shape = 0;

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
	Eigen::Vector3d GetVelocity(int segment, float t, float dt);
	int IsPicked(float x, float y);
	void UnPicked();
	void SetPicked(float x,float y);
	void drawBezier(int index);
	void drawSection(int shapeIndx ,int section);

	~Project(void);
};