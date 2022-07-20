#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include <stdlib.h>

#define bezier_1(t, p1, p2, p3, p4) (pow((1 - t),3)*p1) + (3* pow((1-t), 2) * t * p2) + (3* pow(t, 2) * (1 - t) * p3) + pow(t,3) * p4
#define POINTS_NUM 7
#define Radius 0.22
#define NUM_POL (POINTS_NUM - 1)/3

class Bez 
{
private:
	Eigen::Vector2d* get_points(int section){
		return bez_points + section*3;
	}

public:
	Eigen::Vector2d bez_points[POINTS_NUM];
	
	Bez() {
		float x = 0.5528;
		bez_points[0] = Eigen::Vector2d(-4, 0);
		bez_points[1] = Eigen::Vector2d(-1, x);
		bez_points[2] = Eigen::Vector2d(-x, 1);
		bez_points[3] = Eigen::Vector2d(0, 4);
		bez_points[4] = Eigen::Vector2d(x, 1);
		bez_points[5] = Eigen::Vector2d(1, -x);
		bez_points[6] = Eigen::Vector2d(-4, 0);
	}

	Eigen::Vector2d bezier(float t, int section){
		Eigen::Vector2d* points = get_points(section);
		float t_1 = t, t_2 = t*t, t_3 = t_1 * t_2;
		float tc_1 = (1 -t), tc_2 = (1 -t)*(1 - t), tc_3 = tc_1 * tc_2;

		return tc_3 * points[0] + 3 *tc_2 * t_1 * points[1] + 3* tc_1 * t_2 * points[2] + t_3 * points[3];
	}

	Eigen::Vector3d velocity(float t, int section, float dt){
		Eigen::Vector2d bez_t = bezier(t, section);
		Eigen::Vector2d bez_p = bezier(t + dt, section);
		Eigen::Vector3d temp(bez_p[0] - bez_t[0], bez_p[1] - bez_t[1] ,0);
		std::cout<<t<< "\n"<<dt<< "\n" << temp << "\n\n";
		return temp; 
	}

	int pickedPoint=-1;
	bool isPicked = false; 
};

class Project : public igl::opengl::glfw::Viewer
{
private:
	Eigen::Vector4f bez_points[POINTS_NUM];
	float t=0, dt=0.01;
	int segment=0;
	std::vector<Bez> bez;

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