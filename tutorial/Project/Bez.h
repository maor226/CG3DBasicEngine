#pragma once

#define bezier(t, p1, p2, p3, p4) (pow((1 - t),3)*p1) + (3* pow((1-t), 2) * t * p2) + (3* pow(t, 2) * (1 - t) * p3) + pow(t,3) * p4

class Bez 
{
private:
	
public:
	int pickedPoint=-1;
	bool isPicked = false; 
};