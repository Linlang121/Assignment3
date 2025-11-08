#include "Projsmanager.h"
#include <iostream>
using namespace std;
using namespace GamesEngineeringBase;


ProjsManager::ProjsManager()
{
}
//把所有还存在的子弹（Projectile）对象的内存手动释放掉
ProjsManager::~ProjsManager() {
    for (int i = 0; i < currentSize; i++) {
        delete projs[i];
    }
}
//什么时候可以发射一颗子弹、子弹的初始位置与目标、速度、射程等属性
void ProjsManager::createProjs(float dt, Vec worldPos, Vec target, float speed, int shootingRange) {
   ///这是个计时器，用来计算“距离上次发射过去了多久”。
    timeElapsed += dt;
    //检查当前场上子弹数量是否达到上限
    if (currentSize < maxProjSize)
    {
		//冷却时间到，可以发射新子弹
        if (timeElapsed > timeThreshold)
        {
            Projectile* proj = new Projectile(worldPos, target, speed, shootingRange);
            //把新生成的子弹指针存入子弹数组中
            projs[currentSize++] = proj;
            //重置冷却计时器
            timeElapsed = 0.f;      
        }
    }
}
//专门负责清理失效的子弹对象
void ProjsManager::checkDeleteProj(unsigned int i) 
{
    if (!projs[i]->getIsAlive())
    { 
        Projectile* _p = projs[i];
        projs[i] = nullptr;
        delete _p;
    }
}


void ProjsManager::update(float dt, Vec worldPos, Vec objWorldPos, Camera& camera, float speed, int shootingRange) {
    createProjs(dt, worldPos, objWorldPos, speed, shootingRange);

    for (int i = 0; i < currentSize; i++) 
    {
        if (projs[i] != nullptr)
        {
            //通过指针调用对象的成员函数
            //ptr->update(dt, camera);
            projs[i]->update(dt, camera);
            checkDeleteProj(i);
        }
    }
}


void ProjsManager::draw(Window& canvas, int projSize, V3d color) {
    for (int i = 0; i < currentSize; i++) {
        if (projs[i] != nullptr) {
            projs[i]->draw(canvas, projSize, color);
            //projs[i]->draw(canvas);
        }
    }
}