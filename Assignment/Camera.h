#pragma once
#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "GamesEngineeringBase.h"
#include "Utils.h"
using namespace GamesEngineeringBase;
using namespace std;


class Camera
{	
public:
	Camera(Window& window);
	Vec getPosition()const;
	int getWidth()const;
	int getHeight()const;
	void update(const Vec& playerpos);
	Vec worldToScreen(const Vec& worldpos)const;

private:
	Vec position;
	int width;
	int height;



};
#endif
