#pragma once
#pragma once
#ifndef PROJSMANAGER_H
#define PROJSMANAGER_H

#include "Projectile.h"
#include "Utils.h"
#include "GamesEngineeringBase.h"
using namespace GamesEngineeringBase;
//用来统一管理场上所有子弹（Projectile）
class ProjsManager
{
public:
	ProjsManager();
	~ProjsManager();

	//当发射者（Player/NPC）冷却完毕后，生成一个新的 Projectile 对象。
	void createProjs(float dt, Vec worldPos, Vec target, float speed, int shootingRange);
	//检查第 i 个子弹是否还活着，如果 isAlive == false，就删除并把指针置空。
	void checkDeleteProj(unsigned int i);
	//更新所有子弹飞行、生命周期和发射逻辑
	void update(float dt, Vec worldPos, Vec objWorldPos, Camera& camera, float speed, int shootingRange);
	//检查场上任意一颗子弹是否击中目标（Player/NPC）
	bool checkProjEntityCollision(Vec objWorldPos, int spriteSize);
	//绘制所有还存在的子弹
	void draw(GamesEngineeringBase::Window& canvas, int projSize, V3d color);
private:


	//static在函数内部	变量在函数退出后仍然存在（静态存储期）
	//static在文件作用域（函数外）	变量或函数的作用域仅限当前源文件（内部链接）
	//static在类中（C++）	所有对象共享同一个静态成员变量
	//最大可同时存在的子弹数
	static const int maxProjSize = 10000;
	//存放所有正在飞行的子弹的指针数组；projs[i] 是第 i 颗子弹的指针。
	// 初始化为 {nullptr} 表示开始时没有子弹
	Projectile* projs[maxProjSize] = { nullptr };
	//记录当前场上活跃的子弹数量（便于循环更新或绘制）
	int currentSize = 0;
	//自上次发射后经过的时间；
	float timeElapsed = 0.f;
	//发射间隔时间（冷却时间）
	float timeThreshold = 1.f;




};





#endif
