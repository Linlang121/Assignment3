#pragma once
#ifndef NPC_H
#define NPC_H
#include "Constvalues.h"
#include "Utils.h"   
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Gameobject.h"
#include "Player.h"
class NPC : public Gameobject
{
public:
	//空的构造函数，方便在容器或数组中先声明再初始化
	NPC() {}
	//用于创建一只具体的 NPC
	NPC(Vec _pos, std::string filename, int _maxHp, float _speed);
	//拷贝构造函数，用于从另一个 NPC 复制生成一个新的 NPC。
	//会复制位置、血量、图片名、状态等。
	NPC(const NPC& other);
	//赋值运算符，用于在已有的 NPC 上赋值另一个 NPC 的全部属性。
	NPC& operator=(const NPC& other);

	int getHp() const {	return Hp;	};
	//返回 NPC 与玩家之间的距离。
	int getNPCPlayerDistance() const { return NPCPlayerDistance; }
	//返回 NPC 的移动速度。
	int getSpeed()const{ return speed; }
	//是 NPC 的“AI 核心逻辑”，每帧都会调用一次。
	void update(float dt, Vec& playerPos, Camera& cam);
	//判断 NPC 是否与玩家相撞（近战接触）
	bool checkCollision(Vec playerPos);
	//返回 isAggroActive（NPC 是否处于攻击/警觉状态）
	bool getIsAggroActive() { return isAggroActive; }

	
private:
	int NPCPlayerDistance;
	//NPC 是否处于攻击/警觉状态
	bool isAggroActive = false;
};


//---------------------- NPCmanager 类的声明 ---------------------//
class NPCmanager
{
public:
	 NPCmanager();
	 ~NPCmanager();
	 //拷贝构造函数，禁止复制 NPCmanager 实例
	 NPCmanager& operator=(const NPCmanager& other);



	//用于生成新的 NPC 并将其添加到 npc_array 中
	//void createNPC(Camera& camera, float& dt);
	//删除已死亡（血量 <= 0）的 NPC
	//void checkDeleteNPC(unsigned int i);
	//生成 NPC 出生的世界坐标
	//Vec createStartPos(Camera& camera);

	void update(float& dt, Vec& playerPos, Camera& camera);
	void draw(GamesEngineeringBase::Window& canvas);
	void checkNPCPlayerCollision(Player& player);
	void setProjectiles(float dt, Gameobject& obj, Camera& camera);
	void drawProjectiles(GamesEngineeringBase::Window& canvas);

	bool getIfNPCinPlayerAttackRange();
	NPC* const getClosestNPCtoPlayer();
	void attackTopFiveHpNPC();


	void generateNPC(Camera& camera, float& dt);
	void checkDeleteNPC(unsigned int i);
	Vec generateStartPos(Camera& camera);



private:
	
	static const int maxNpcSize = CmaxNpcSize;
    //存放所有正在游戏中活着的 NPC 指针。
	NPC* npc_array[maxNpcSize] = { nullptr };
	//存储每个 NPC 的“索引 + 血量”信息,用来快速找出前3个血量最高的 NPC
	Vec sort_health_array[maxNpcSize];
	//当前游戏中活着的 NPC 数量
	int currentNPCSize = 0;
	//计时器，用于控制 NPC 生成频率
	float timeElapsed = 0.f;
	//生成 NPC 的时间间隔阈值（秒）
	float timeThreshold = 3.f;
	
	
}; 
#endif