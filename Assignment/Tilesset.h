#pragma once
#ifndef TILESET_H
#define TILESET_H
#include "GamesEngineeringBase.h"
#include "Constvalues.h"

using namespace GamesEngineeringBase;
class tile
{
public:
	//构造函数，决定了每个 tile 对象的初始状态
	tile();
	//第一个const表示返回值类型是常量，调用者不能修改返回的值
	//第二个表示该成员函数不会修改类的成员变量
    bool getPassablity()const;
	void setPassable();
	void setUnpassable();
	void loadImage(const std::string& filename);
	void draw( int x,  int y, Window& window);

private:
	bool passablity;
	Image image;
};
class tilesset
{
public:
	tilesset();
   int getTileIndex(int i, int j)const;
   int getTileswide()const;
   int getTileshigh()const;
   int load(const std::string& filename);

private:
	
	//每行每列有多少个tile
	int tileswide;
	int tileshigh;
	int** tilesarr;
	//使用智能指针管理二维数组内存
    //std::vector<std::unique_ptr<int[]>> arr2D;
	//每个tile的宽高
	int tileWidth;
	int tileHeight;
	int layer;
	int tileIndex;
};

#endif