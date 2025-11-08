#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Utils.h"
#include "Camera.h"
#include "GamesEngineeringBase.h"
#include "Constvalues.h"

using namespace GamesEngineeringBase;
using namespace std;
class Projectile
{
	
public:
	//当 NPC 或 Player 发射子弹时调用
	Projectile(Vec _worldPos, Vec _target, float _speed, int _shootingRange);
	//每帧更新子弹状态。
	void update(float dt, Camera& cam);
	//在屏幕上绘制子弹
	void draw(GamesEngineeringBase::Window& canvas, int projSize, V3d color);
	//检查子弹是否撞到了一个对象（玩家或 NPC）
	bool checkCollision(Vec pos, int spriteSize);
	//获取子弹是否还存活
	bool getIsAlive() const { return isAlive; }
	//返回子弹是否命中目标（用于处理伤害）
	bool getIsCollided() const { return isCollided; }
private:

	Vec worldPosition;
	Vec screenPosition;
	//目标位置
	Vec targetPosition;
	//如果子弹没有击中任何目标，到射程末端会停下来的位置。
	Vec stopPosition;
	//子弹图片
	Image sprite;
	//子弹速度，单位是像素/秒。
	float speed;
	//子弹已经飞了多远（距离累计器）。
	int distanceProjMoved;
	//子弹与目标之间的距离。
	int projTargetDistance;
	//子弹最大射程。
	int shootingRange;
	// 是否撞到目标,isCollided,如果撞到 Player/NPC，则设置为 true；
	bool isCollided = false;
	// 是否还存在,如果飞到射程外或击中目标，则 false；（false 时会从列表删除）
	bool isAlive = true;

};





#endif
