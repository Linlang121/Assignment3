#include "Gameobject.h"
#include <iostream>
#include <fstream>
using namespace GamesEngineeringBase;
//赋值运算符重载
Gameobject& Gameobject::operator=(const Gameobject& other)
{
    if (this == &other) {
        return *this;
    }
    worldPosition.x = other.worldPosition.x;
    worldPosition.y = other.worldPosition.y;
    screenPosition.x = other.screenPosition.x;
    screenPosition.y = other.screenPosition.y;
    Hp = other.Hp;
    speed = other.speed;
    Flicker = other.Flicker;
    sprite.load(other.spritePath);

    return *this;
}

void Gameobject::updateProjectTiles(float dt,Gameobject& object,Camera& camera)
{
    // 默认实现为空，子类可以重写此方法
}
//闪烁更新
void Gameobject::updateFlicker(float dt)
{
    if (Flicker > 0)
    {
        if (flickerTimer < flickerDuration) flickerTimer += dt;
        else {
            Flicker = 0;
            flickerTimer = 0.f;
        }
    }
}
//绘制函数
void Gameobject::draw(Window& canvas)
{
    for (unsigned int i = 0; i < sprite.width; i++) {
        for (unsigned int j = 0; j < sprite.height; j++) {
            if (screenPosition.x + i >= 0 && screenPosition.x + i < canvas.getWidth() &&
                screenPosition.y + j >= 0 && screenPosition.y + j < canvas.getHeight()) {
                if (sprite.alphaAt(i, j) > 200) {
                    canvas.draw(i + screenPosition.x, j + screenPosition.y, sprite.at(i, j));
                }
            }
        }
    }
}
//绘制闪烁效果
void Gameobject::drawFlicker(Window& canvas, V3d flickerColor)
{
    if (Flicker != 0) {
        for (unsigned int i = 0; i < sprite.width; i++) {
            for (unsigned int j = 0; j < sprite.height; j++) {
                if (screenPosition.x + i >= 0 && screenPosition.x + i < canvas.getWidth() &&
                    screenPosition.y + j >= 0 && screenPosition.y + j < canvas.getHeight()) {
                    if (sprite.alphaAt(i, j) > 200) {
                        if (Flicker == 1)
                            canvas.draw(i + screenPosition.x, j + screenPosition.y, flickerColor.x, flickerColor.y, flickerColor.z);
                    }
                }
            }
        }
    }
  
}
//绘制血条
void Gameobject::drawHpBar(Window& canvas) {
    int barHeight = 4;
    //draw health bar start from NPC's screen system position and higher than the top of NPC's sprite
    int startPosX = screenPosition.x;
    int startPosY = screenPosition.y - 8;
    // maxHealth will be drawn as a black background bar, represents initial health value
    // health will decrease as NPC is attacked, so the red health bar will become shorter
    for (unsigned int i = 0; i < maxHp / 6; i++) {
        for (unsigned int j = 0; j < barHeight; j++) {
            if (startPosX + i >= 0 && startPosX + i < canvas.getWidth() &&
                startPosY + j >= 0 && startPosY + j < canvas.getHeight()) {
                //draw black maxHealth bar as background
                canvas.draw(i + startPosX, j + startPosY, 0, 0, 0);
                //draw red health bar based on the current health level
                if (i <= Hp / 6) canvas.draw(i + startPosX, j + startPosY, 255, 0, 0);
                if (maxHp == 500 && i <= Hp / 3) canvas.draw(i + startPosX, j + startPosY, 255, 255, 0);
            }
        }
    }
}
//绘制子弹
void Gameobject::drawProjectTiles(Window& canvas, int projectSize, V3d color)
{
    
}
//应用伤害
void Gameobject::applyDamage(int damage)
{
    if (Hp > 0)
    Hp -= damage;
    else
    Hp = 0;
    
}
