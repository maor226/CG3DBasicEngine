#pragma once
#include <external/eigen/Eigen/Core>

#define bezier_2(t, p1, p2, p3, p4) (pow((1 - t),3)*p1) + (3* pow((1-t), 2) * t * p2) + (3* pow(t, 2) * (1 - t) * p3) + pow(t,3) * p4
#define step 0.01
#define POINTS_NUM 7

class Bez 
{
private:
	Eigen::Vector4f* get_points(int section);
	
public:
	Bez();
	Eigen::Vector4f bez_points[POINTS_NUM];

	float bezier_1(float t, int section);
	Eigen::Vector3d velocity(int t, int section);
	int pickedPoint=-1;
	bool isPicked = false; 
};