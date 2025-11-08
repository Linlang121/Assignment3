#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Utils.h"
constexpr const int Ctileswide = 42;
constexpr const int Ctileshigh = 42;
constexpr const int CtileWidth = 32;
constexpr const int CtileHeight = 32;
constexpr const int CmapWidth = Ctileswide * CtileWidth;
constexpr const int CmapHeight = Ctileshigh * CtileHeight;
//绘制的地图块数量
constexpr const int CtilesNumX=40;
constexpr const int CtilesNumY=40;
constexpr const int CtilesType=26;
constexpr const int CindexUnpassable = 10;

//npc弹幕速度
constexpr const int CprojSpeed = 280.f;
//npc射击范围
constexpr const int CshootingRange =700;
//npc警戒范围
constexpr const int aggroRange = 600;
//npc停止追击玩家的距离
constexpr const int stopAggroDistance = 60;
//npc最大数量
constexpr const int CmaxNpcSize = 35;
//npc线性攻击范围
constexpr const int linearAttackRange = 400;
//npc闪烁颜色
constexpr const V3d NPCFlickerColor = { 255,255,255 };
//npc子弹颜色
constexpr const V3d NPCprojColor = { 1,1,255 };
//玩家AOE伤害值
constexpr const int AOEDamage = 80;
constexpr const int AOEAttackRange = 600;

const V3d playerProjColor = { 1,255,1 };

  //
constexpr const int NPCprojWidth = 6;
  //
  constexpr const int playerProjWidth = 8;


  //linearAttackRange
constexpr const int playerSpriteSize = 32;
//分数
extern int scores;
#endif 
