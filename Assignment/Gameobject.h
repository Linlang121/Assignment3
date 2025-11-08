#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Utils.h"   
#include "GamesEngineeringBase.h"
#include "Camera.h"
using namespace GamesEngineeringBase;
using namespace std;
class Gameobject
{

public:
	Gameobject() {}
	Gameobject& operator=(const Gameobject& other);
	//get
	Vec getWorldPosition() const { return worldPosition; }
	int getHp() const { return Hp; }
	int getMaxHp() const { return maxHp; }
	int getFlicker() const { return Flicker; }
	int getSpeed() const { return speed; }
	int getSpriteWidth() const { return sprite.width; }
	string getSpritepath()const { return spritePath; }
	bool getIsDead() { return Hp <= 0; }
	bool getIsAlive() {
		return(Hp> 0);
	}

	//set
	void setFlicker(int flag) { Flicker = flag; }
	void setSprite() { sprite.load(spritePath); }

	//update
	//多态的virtual
	virtual void updateProjectTiles(float dt, Gameobject& object, Camera& camera) ;
	void updateFlicker(float dt);

	//draw
	void draw(Window& canvas);
	void drawFlicker(Window& canvas, V3d flickerColor);
	void drawHpBar(Window& canvas);
	void drawProjectTiles(Window& canvas,int projectSize,V3d color);
	void applyDamage(int damage);
protected:
	int Hp = 300;
	int maxHp = 300;
	float speed = 0.1f;
	int Flicker = 0;
	//子弹
	//ProjsManager projs;
	float ProjSpeed;
	int shootingRange;


	Vec worldPosition;
	Vec screenPosition;
	Image sprite;
	string spritePath;

	float flickerDuration = 0.2f; //闪烁总时间
	float flickerTimer = 0.0f;

};







#endif