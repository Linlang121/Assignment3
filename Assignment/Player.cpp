#include <iostream>
#include <fstream>
#include "Player.h"
#include "World.h"
#include "Constvalues.h"
using namespace std;
using namespace GamesEngineeringBase;
// 初始化玩家对象
Player::Player(int _x, int _y, std::string filepath)
{
	worldPosition.x = _x;
	worldPosition.y = _y;
	spritePath = filepath;
	sprite.load(spritePath);
	speed = 100.0f; // 设置玩家移动速度
	ProjSpeed = 300.0f; // 设置子弹速度
	shootingRange = 200; // 设置射程
}
// 赋值运算符重载
Player& Player::operator=(const Player& other)
{
	if (this != &other)
	{
		// 调用基类的赋值运算符
		Gameobject::operator=(other);
		// 复制玩家特有的成员变量
		ifApplyAOE = other.ifApplyAOE;
		timeElapsed_anim = other.timeElapsed_anim;		
	}
	return *this;
}
// 更新玩家行走动画
void Player::updateMovementAnim(std::string name)
{
	// 根据传入的名称更新玩家的行走动画
	string basePath1 = "Resources/"+name+"0.png";
	string basePath2 = "Resources/" + name + "1.png";
	//string basePath3 = "Resource/" + name + "2.png";

	if (timeElapsed_anim < timeThreshold_anim)
	{
		sprite.load(basePath1);
	}
	else if (timeElapsed_anim < 2 * timeThreshold_anim)
	{
		sprite.load(basePath2);
	}
	else
	{
		timeElapsed_anim = 0.f; // 重置动画时间
	}

}
// 玩家主循环更新函数j
void Player::update(float dt, GamesEngineeringBase::Window& canvas, World& world, Camera& cam)
{
	// 更新玩家状态
	timeElapsed_anim += dt; // 更新动画时间
	// 处理玩家移动输入
	Vec movement;
	movement.x = 0;
	movement.y = 0;
	int move = speed * dt;

	if (canvas.keyPressed('W'))
	{
		movement.y -= move;
		updateMovementAnim("up");
	}
	if (canvas.keyPressed('S'))
	{
		movement.y += move;
		updateMovementAnim("down");
	}
	if (canvas.keyPressed('A'))
	{
		movement.x -= move;
		updateMovementAnim("left");
	}
	if (canvas.keyPressed('D'))
	{
		movement.x += move;
		updateMovementAnim("right");
	}
	// 归一化移动向量以防止斜向移动过快
	int posXNew = worldPosition.x + movement.x;
	int posYNew = worldPosition.y + movement.y;

	if (world.checkTilePassablity(posXNew, posYNew)) {
		worldPosition.x = posXNew;
		worldPosition.y = posYNew;
		if (worldPosition.x < 0) worldPosition.x = 0;
		if (worldPosition.y < 0) worldPosition.y = 0;
		if ((worldPosition.x + 32) >1344) worldPosition.x = 1344 -32;
		if ((worldPosition.y + 32) >1344) worldPosition.y =1344 - 32;
	}

	screenPosition = cam.worldToScreen(worldPosition);

	// checks if the flicker effect should stop based on elapsed time 
	updateFlicker(dt);
}