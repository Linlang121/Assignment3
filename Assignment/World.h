#pragma once
#ifndef WORLD_H
#define WORLD_H
#include "Constvalues.h"
#include "GamesEngineeringBase.h"
#include "Tilesset.h"
#include "Utils.h"
//从文件加载地图结构（通过 Tileset）；
//加载所有 tile 图片（每种地形的贴图）；
//每帧绘制地图、提供地图的碰撞检测。

namespace GamesEngineeringBase
{
	class World
	{
	public:
		World(Window& window);
		void draw(Window& window,Vec& CameraPos);
		bool checkTilePassablity(int x, int y);
	private:
		tilesset tileset;
		static const int tilesType = CtilesType;
		//存放不同类型的地图块
		tile tiles[tilesType];
		//横纵分别画多少块地图
		int tilesNumX= CtilesNumX;
		int tilesNumY= CtilesNumY;



	};
}




#endif
