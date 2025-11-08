#include "NPC.h"
#include "Constvalues.h"
#include "math.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace GamesEngineeringBase;
//用于创建一只具体的 NPC
NPC::NPC(Vec _pos, std::string	_filename, int _maxHp, float _speed)
{
	worldPosition = _pos;
	Hp = _maxHp;
	maxHp = _maxHp;
	speed = _speed;
	ProjSpeed = CprojSpeed;
	shootingRange = CshootingRange;


	spritePath = _filename;
	sprite.load(spritePath);
	
	
}

//拷贝构造函数，用于从另一个 NPC 复制生成一个新的 NPC。
NPC::NPC(const NPC& other) {
	NPCPlayerDistance = other.NPCPlayerDistance;
	isAggroActive = other.isAggroActive;
	sprite.load(spritePath); 
}
//赋值运算符，用于在已有的 NPC 上赋值另一个 NPC 的全部属性。
NPC& NPC::operator=(const NPC& other) {
	if (this == &other) {
		return *this; 
	}
	Gameobject::operator=(other); 
	NPCPlayerDistance = other.NPCPlayerDistance;
	isAggroActive = other.isAggroActive;
}
//NPC 人工智能（AI）移动逻辑的核心函数
//让 NPC 每帧判断与玩家的距离，
//在特定范围内启动追击、移动到新位置，
//并实时更新它的屏幕位置与闪烁状态。
void NPC::update(float dt, Vec& playerPos, Camera& cam) 
{
    //计算 NPC 每帧可移动的距离
    int move = static_cast<int>(speed * dt);
	//计算 NPC 与玩家之间的距离
    NPCPlayerDistance = static_cast<int>(
        sqrt(pow(worldPosition.x - playerPos.x, 2) +
            pow(worldPosition.y - playerPos.y, 2)));
	//防止除以零
    if (NPCPlayerDistance == 0) NPCPlayerDistance = 1;
	//计算 NPC 朝向玩家的移动分量
    float moveX = move * (playerPos.x - worldPosition.x) / NPCPlayerDistance;
    float moveY = move * (playerPos.y - worldPosition.y) / NPCPlayerDistance;

	//计算 NPC 的新位置
    int newPosX = worldPosition.x + static_cast<int>(moveX);
    int newPosY = worldPosition.y + static_cast<int>(moveY);
	//检查新位置是否在地图边界内
    bool inside = (newPosX > 0 && newPosX + 32 < CmapWidth) && (newPosY > 0 && newPosY + 32 < CmapHeight);
	//更新 NPC 的警觉状态
    if (NPCPlayerDistance < aggroRange) isAggroActive = true;
    else isAggroActive = false;

    //控制移动范围（追击逻辑）
    if (inside && NPCPlayerDistance > stopAggroDistance && NPCPlayerDistance < aggroRange) {
        worldPosition.x = newPosX;
        worldPosition.y = newPosY;
    }
    screenPosition = cam.worldToScreen(worldPosition); 

    // 更新闪烁状态（受伤反馈）
    updateFlicker(dt);
}

//它使用 “圆形碰撞检测（距离法）” 来判断 NPC 是否与玩家相撞
bool NPC::checkCollision(Vec playerPos) {
    int x2 = (worldPosition.x - playerPos.x) * (worldPosition.x - playerPos.x);
    int y2 = (worldPosition.y - playerPos.y) * (worldPosition.y - playerPos.y);
    float r = sqrtf(x2 + y2);
    //“双方中心距离小于大约 2/3 个 sprite 大小时，算作碰撞”。
    float clash = static_cast<float>(2 * 32 / 3);
    return(r < clash);
}


//---------------------- NPCmanager 类的实现 ---------------------//
NPCmanager::NPCmanager() {}

NPCmanager::~NPCmanager() {
    for (int i = 0; i < currentNPCSize; i++) {
        delete npc_array[i];
    }
}

NPCmanager& NPCmanager::operator=(const NPCmanager& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }

    // Copy simple data members
    currentNPCSize = other.currentNPCSize;
    timeElapsed = other.timeElapsed;

    // Delete existing NPCs in npc_array to prevent memory leaks
    for (int i = 0; i < maxNpcSize; ++i) {
        delete npc_array[i];
        npc_array[i] = nullptr;
    }

    // deep copy of the npc_array
    for (int i = 0; i < maxNpcSize; ++i) {
        if (other.npc_array[i] != nullptr) {
            npc_array[i] = new NPC(*other.npc_array[i]); // use NPC's copy constructor
        }
        else npc_array[i] = nullptr; // ensure null pointers are copied properly
    }

    // copy the sort_health_array 
    for (int i = 0; i < maxNpcSize; ++i) {
        sort_health_array[i] = other.sort_health_array[i];
    }

    return *this;
}

//it might be quick when map size super larger than camera view
Vec NPCmanager::generateStartPos(Camera& camera) {
    Vec camPos = camera.getPosition();
    int camPosX = camPos.x;
    int camPosY = camPos.y;
    int width = camera.getWidth();
    int height = camera.getHeight();
    bool t1, t2, flag = true;
    int x, y;
    //keep generating random position until find a position outside the camera view
    do {
        x = rand() % CmapWidth;
        y = rand() % CmapHeight;
        //inside map
        t1 = (x > 0 && x + 32 < CmapWidth) && (y > 0 && y + 32< CmapHeight);
        //inside camera view
        t2 = (x + 32 >= camPosX && x <= camPosX + width) && (y + 32 >= camPosY && y <= camPosY + height);
        //if inside map and outside the camera view, break
        if (t1 == true && t2 == false) {
            flag = false;
        }
        //if inside map and outside camera view then break
    } while (flag);

    Vec t;
    t.x = x;
    t.y = y;

    return t;
}

void NPCmanager::generateNPC(Camera& camera, float& dt) {
    // generate 4 types NPC with random start position, sprite, health and speed
    // generate in an increasing frequency
    timeElapsed += dt;
    if (currentNPCSize < maxNpcSize) { // max NPC number is 40
        if (timeElapsed > timeThreshold) {
            Vec pos = generateStartPos(camera);
            // choose one type of NPC from 4 types randomly
            int NPCtypeIndex = rand() % 4;
            int health = 0;
            float speed = 0.f;
            string filename = "Resources/npc" + to_string(NPCtypeIndex) + ".png";
            switch (NPCtypeIndex) {
            case 0:
                health = 360;
                speed = 0.f; //shooter
                break;
            case 1:
                health = 180;
                speed = 430.f;
                break;
            case 2:
                health = 240;
                speed = 300.f;
                break;
            case 3:
                health = 300;
                speed = 250.f;
                break;
            }
            // add new NPC pointer to array
            NPC* _npc = new NPC(pos, filename, health, speed);
            npc_array[currentNPCSize++] = _npc;
            timeElapsed = 0.f;
            // frequency of generation increases over time, as time threshold reduces
            timeThreshold -= 0.1f;
            //threshold keep reducing till it is 0.3f
            timeThreshold = max(0.3f, timeThreshold);
        }
    }
}
void NPCmanager::checkDeleteNPC(unsigned int i) {
    if (!npc_array[i]->getIsAlive()) { //dead
        NPC* _npc = npc_array[i];
        scores += npc_array[i]->getMaxHp() / 10;
        npc_array[i] = nullptr;
        delete _npc;

        cout << "SCORE : " << scores << "      kILLED enemy No." << i << endl;
    }
}

void NPCmanager::update(float& dt, Vec& playerPos, Camera& camera) {
    //generate random NPC outside camera view
    generateNPC(camera, dt);

    for (int i = 0; i < currentNPCSize; i++) {
        sort_health_array[i].x = i;
        if (npc_array[i] != nullptr) {
            npc_array[i]->update(dt, playerPos, camera);
            // update NPC index and health in this array for sorting later
            sort_health_array[i].y = npc_array[i]->getHp();
            // if NPC's alive state is false, delete it
            checkDeleteNPC(i);
        }
        else sort_health_array[i].y = 0;

    }
}

void NPCmanager::draw(Window& canvas) {
    for (int i = 0; i < currentNPCSize; i++) {
        if (npc_array[i] != nullptr) {
            npc_array[i]->draw(canvas); // draw each NPC
            npc_array[i]->drawHpBar(canvas); // draw each NPC's health bar
            npc_array[i]->drawFlicker(canvas, NPCFlickerColor); // draw each NPC's flicker effect
        }
    }
}

// if NPC collides with player, player will flicker as red color
// called in main game loop; if true, affect player flickering
// (more complex behaviors : if true, NPCs moving back)
void NPCmanager::checkNPCPlayerCollision(Player& player) {
    for (int i = 0; i < currentNPCSize; i++) {
        if (npc_array[i] != nullptr) {
            if (npc_array[i]->checkCollision(player.getWorldPosition())) {
                player.setFlicker(1);
                player.applyDamage(1);
            }
        }
    }
}

// generate & update projectiles for those NPC(shooter) whose speed is 0
void NPCmanager::setProjectiles(float dt, Gameobject& obj, Camera& camera) {
    for (int i = 0; i < currentNPCSize; i++) {
        if (npc_array[i] != nullptr &&/* npc_array[i]->getSpeed() == 0.f &&*/ npc_array[i]->getIsAggroActive()) {
            npc_array[i]->updateProjectTiles(dt, obj, camera);
        }
    }
}
// draw projectiles for those NPC(shooter) whose speed is 0
void NPCmanager::drawProjectiles(Window& canvas) {
    for (int i = 0; i < currentNPCSize; i++) {
        if (npc_array[i] != nullptr && /* npc_array[i]->getSpeed() == 0.f &&*/ npc_array[i]->getIsAggroActive()) {
            npc_array[i]->drawProjectTiles(canvas, NPCprojWidth, NPCprojColor);
        }
    }
}
// check if any NPC is in the linear attack range of player
bool NPCmanager::getIfNPCinPlayerAttackRange() {
    for (int i = 0; i < currentNPCSize; i++) {
        if (npc_array[i] != nullptr &&
            npc_array[i]->getNPCPlayerDistance() < linearAttackRange) {
            return true;
        }
    }
    return false;
}

// return NPC pointer , which is closest to player, to trigger player's linear attack
NPC* const NPCmanager::getClosestNPCtoPlayer() {
    int min = 1000;
    int index = -1;
    for (int i = 0; i < currentNPCSize; i++) {
        if (npc_array[i] != nullptr && npc_array[i]->getIsAggroActive()) {
            if (min > npc_array[i]->getNPCPlayerDistance()) {
                min = npc_array[i]->getNPCPlayerDistance();
                index = i;
            }
        }
    }
    if (index == -1) {
        //cout << " no closest NPC to player" << endl;
        return nullptr;
    }
    return npc_array[index];
}

void swap(Vec& a, Vec& b) {
    Vec tmp = a;
    a = b;
    b = tmp;
}

// QuickSortByHealth function: sorts an array of Vec2 structures based on 
// the 'y' value (representing health) in descending order.
// 
// arr: pointer to array of Vec2 structures to be sorted
// start: starting index of sorting
// end: ending index
void QuickSortByHp(Vec* arr, int start, int end)
{
    // base case : if there's only one or none element in sub_array
    if (start >= end) return;
    int left = start;
    int right = end;
    int key = start; //set the pivot
    //partition array
    while (start < end)
    {
        // select the bigger one in the left
        // move [end] to left if [end]'s value less than pivot's
        while (arr[end].y <= arr[key].y && start < end) end--;
        // select the smaller one in the right
        // move [start] to right if [start]'s value bigger than pivot's
        while (arr[start].y >= arr[key].y && start < end) start++;
        // swap those out of order 
        swap(arr[start], arr[end]);
    }
    // place pivot
    swap(arr[key], arr[end]);
    key = end;
    // sort subarray
    QuickSortByHp(arr, left, key - 1);
    QuickSortByHp(arr, key + 1, right);
}


void NPCmanager::attackTopFiveHpNPC() {
    // Vec2 sort_health_array[40]: each element is a Vec2{x,y} object
    // x: stores NPC* npc_array index
    // y: stores corresponding health value

    //sort sort_health_array in descending order based on 'y' value (health)
    QuickSortByHp(sort_health_array, 0, 39);

    int cnt = 0;
    // for test: output all the sorted health value
    //cout << "health of all the NPCs generated currently: ";
    //for (int i = 0; i < currentSize; i++) cout << sort_health_array[i].y << " ";
    //cout << endl;
    //cout << "health of NPCs inside AOE range: ";
    for (int i = 0; i < currentNPCSize; i++) {
        int index = sort_health_array[i].x;
        if (cnt < 5 && npc_array[index] != nullptr) {
            // for those NPCs whose health is highest, select at most 5 NPCs who is inside AOEAttackRange
            if (npc_array[index]->getNPCPlayerDistance() < AOEAttackRange) {
                // reduce NPC health level by AOEDamage value, which is less than linear attack Damage
                npc_array[index]->applyDamage(AOEDamage);
                // the NPC hit by AOE attack would flicker green to indicate damage taken
                npc_array[index]->setFlicker(2);
                cnt++;
                //cout << sort_health_array[i].y << " ";
            }
        }
    }
    //cout << endl;
}