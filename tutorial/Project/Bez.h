#pragma once
#include <external/eigen/Eigen/Core>

#define bezier_2(t, p1, p2, p3, p4) (pow((1 - t),3)*p1) + (3* pow((1-t), 2) * t * p2) + (3* pow(t, 2) * (1 - t) * p3) + pow(t,3) * p4
#define step 0.01
#define POINTS_NUM 7

class Shape 
{
private:
	Eigen::Vector2d* get_points(int section);

public:
	Shape();
	Eigen::Vector2d bez_points[POINTS_NUM];

	Eigen::Vector3d bezier(float t, int section);
	Eigen::Vector3d velocity(int t, int section);
	int pickedPoint=-1;
	bool isPicked = false; 
	bool isMiror= false;
};