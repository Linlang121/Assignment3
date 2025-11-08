#include "Projectile.h"
#include<iostream>
using namespace GamesEngineeringBase;
using namespace std;
//当 NPC 或 Player 发射子弹时调用
//每个 Projectile 就是一颗“带速度、目标、寿命”的子弹。
Projectile::Projectile(Vec _worldPos, Vec _target, float _speed, int _shootingRange)
{	
	worldPosition = _worldPos;
	targetPosition = _target;
	speed = _speed;
	shootingRange = _shootingRange;
	distanceProjMoved = 0;

    //-----计算出子弹与目标之间的距离，
    // -----然后根据“射程”确定子弹最终会停在哪个位置（stopPosition）
    
    //计算当前发射者到目标点之间的距离（projTargetDistance）
    //static_cast<int>转换成整数像素值（去掉小数）
	projTargetDistance = static_cast<int>(
        sqrt(pow( targetPosition.x- worldPosition.x, 2)
            + pow( targetPosition.y- worldPosition.y, 2)));
    //按这个方向延伸一段长度（shootingRange）
     //得到射程尽头的坐标（stopPosition）
    //除以距离可以“归一化”方向向量
	stopPosition.x = static_cast<int>(
        shootingRange * (targetPosition.x - worldPosition.x) / projTargetDistance)
        + worldPosition.x;  
	stopPosition.y = static_cast<int>(
        shootingRange * (targetPosition.y - worldPosition.y) / projTargetDistance)
        + worldPosition.y;

	
}
//它控制子弹每一帧沿着目标方向前进，并在飞行距离超过射程时自动消失。
void Projectile::update(float dt, Camera& cam)
{
    //计算本帧移动距离（像素）
    int move = static_cast<int>(speed * dt);
    //计算子弹到终点的剩余距离
    int distance = static_cast<int>(
        sqrt(pow(worldPosition.x - stopPosition.x, 2) +
            pow(worldPosition.y - stopPosition.y, 2)));

    if (distanceProjMoved < shootingRange) {
        float moveX = move * (stopPosition.x - worldPosition.x) / distance;
        float moveY = move * (stopPosition.y - worldPosition.y) / distance;

        worldPosition.x += static_cast<int>(moveX);
        worldPosition.y += static_cast<int>(moveY);
        screenPosition = cam.worldToScreen(worldPosition);
        distanceProjMoved += move;
    }
    else {
        isAlive = false;
    }
}

void Projectile::draw(Window& canvas, int projSize, V3d color) 
{
    for (int i = 0; i < projSize; i++)
        for (int j = 0; j < projSize; j++) {
            if (screenPosition.x + i >= 0 && screenPosition.x + i < canvas.getWidth() &&
                screenPosition.y + j >= 0 && screenPosition.y + j < canvas.getHeight())
                canvas.draw(i + screenPosition.x, j + screenPosition.y, color.x, color.y, color.z);
        }
}

//Projectile（子弹）类中的碰撞检测函数
bool Projectile::checkCollision(Vec pos, int spriteSize) {
    //hitbox这代表角色贴图在世界中的覆盖范围（相当于一个简单的矩形
    int hitbox = 2*spriteSize / 3;
    if (worldPosition.x > pos.x - hitbox && worldPosition.x < pos.x + hitbox 
        && worldPosition.y > pos.y - hitbox && worldPosition.y < pos.y + hitbox ) 
    {
        isAlive = false;
        return true;
    }
    else
    return false;
}