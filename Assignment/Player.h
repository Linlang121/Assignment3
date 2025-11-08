#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Utils.h"   
#include "World.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Gameobject.h"
class Player : public Gameobject
{
public:
    //创建一个玩家对象，并设置初始位置与贴图。
    Player(int _x, int _y, std::string filepath);
    //用于玩家对象之间的赋值。
    //与 GameObject::operator= 类似，会拷贝玩家的状态、贴图、AOE标志等。
    Player& operator=(const Player& other);
    //提供外部访问与修改 AOE 状态的接口。
    bool getIfApplyAOE() { return ifApplyAOE; }
    void setAOE(bool flag) { ifApplyAOE = flag; }
    //用于切换玩家行走动画的帧（不同方向或动作）
    void updateMovementAnim(std::string name); 
    //玩家主循环更新函数（核心）
    //
    void update(float dt, GamesEngineeringBase::Window& canvas, World& world, Camera& cam);

private:
    //用于记录动画经过了多少时间。
    float timeElapsed_anim = 0.f;
    //控制动画切换的速度（单位：秒）
    float timeThreshold_anim = 0.3f;
    //表示当前是否触发了 范围攻击（AOE）技能
    //它是“Q技能的开关”
    bool ifApplyAOE = false;

};










 #endif