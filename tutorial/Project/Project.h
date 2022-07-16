#pragma once
#include "igl/opengl/glfw/Viewer.h"

class Project : public igl::opengl::glfw::Viewer
{
	
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
	int IsPicked(int x, int y);
	void UnPicked();
	
	
	~Project(void);
};


