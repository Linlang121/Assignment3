#include "GamesEngineeringBase.h"
#include "Tilesset.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include <filesystem>
#include <fstream>
#include <iostream>

#include "NPC.h"
using namespace std;
using namespace GamesEngineeringBase;

// ...
#include <Windows.h>


int main() 
{
    //char buffer[MAX_PATH];
    //GetCurrentDirectoryA(MAX_PATH, buffer);
   // std::cout << "Current Working Directory: " << buffer << std::endl;

    Window canvas;
    canvas.create(1024, 768, "MYGAME");
    World world(canvas);
    Player hero(672, 672, "Resources/left0.png");
    Timer timer;
    Camera camera(canvas);


    NPCmanager npcs;
    bool playerStartsAttack = false;
    bool ifStartCooldown = false;
    
    // 游戏主循环
    while (true)
    {
        // 处理输入消息
        canvas.checkInput();
        
        // 检查是否按下 ESC 键退出
        if (canvas.keyPressed(VK_ESCAPE))
        {
            break;
        }
        
        // 清空背景
        canvas.clear();
        float dt = timer.dt();
        // ---------------------- update
        hero.update(dt, canvas, world, camera); 
        Vec playerPos = hero.getWorldPosition();
        camera.update(playerPos);

        // generate NPC array and update NPC movement
        npcs.update(dt, playerPos, camera);
        // update NPC's projectiles
        npcs.setProjectiles(dt, hero, camera);

        // check collision between each NPC and player
        npcs.checkNPCPlayerCollision(hero);

        // ----------------Player Attack
       
        if (hero.getIfApplyAOE() && !ifStartCooldown) {
            // AOE attack---------------
            npcs.attackTopFiveHpNPC();
            hero.setAOE(false);
            // after player used AOE, start to cool down AOE
            ifStartCooldown = true;
        }
        else {
            // linear attack------------
            //check if any NPC is in player's attack range
            playerStartsAttack = npcs.getIfNPCinPlayerAttackRange();
            //if so, attack the closest NPC using projectiles
            if (playerStartsAttack) {
                // get one NPC which is closest to player
                NPC* closestNPC = npcs.getClosestNPCtoPlayer();
                // player LINEAR ATTACK -- launch projectiles
                hero.updateProjectTiles(dt, *closestNPC, camera);
            }
        }


		// ---------------------- draw
        // 获取相机位置并绘制世界
        Vec camPos = camera.getPosition();
        world.draw(canvas, camPos);


        hero.draw(canvas);
        hero.drawHpBar(canvas);
        // draw player's flicker effect when player is hit by projectile or collide with NPC
        hero.drawFlicker(canvas, { 255,1,1 });
        
        if (playerStartsAttack) hero.drawProjectTiles(canvas, playerProjWidth, playerProjColor);

        // draw NPC--------------------------------
        npcs.draw(canvas);
        // draw NPC's projectile if it's speed is zero
        npcs.drawProjectiles(canvas);

        // 显示画面
        canvas.present();
        
        // 短暂休眠以避免占用过多 CPU
        Sleep(16); // 约 60 FPS
    }
    
	return 0;
}

