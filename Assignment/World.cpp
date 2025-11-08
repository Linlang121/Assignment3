#include "World.h"
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
using namespace GamesEngineeringBase;
World::World(Window& window)
{
	// 把编号存入数组
	int loadResult = tileset.load("Resources/tiles.txt");
	if (loadResult != 0)
	{
		cerr << "Error: Failed to load tileset file. The game may not work correctly." << endl;
	}
	// 加载每种地形的图片和通行性
	for (int i = 0; i < tilesType; i++)
	{
		if ((i >= 14 && i <= 22) || i == 24)//(i== CindexUnpassable)
			tiles[i].setUnpassable();
		tiles[i].loadImage("Resources/" + to_string(i) + ".png");
	}
}
//根据当前相机的位置（camPos），在窗口上绘制出当前能看到的地图部分。
void World::draw(Window& window, Vec& camPos)
{
	//计算出摄像机左上角对应的地图块索引
	//地图坐标，告诉程序相机看到的是地图的第几块 tile
	int indexX = camPos.x / CtileWidth;
	int indexY = camPos.y / CtileHeight;
	//屏幕坐标，这是绘制时的屏幕起点（左上角）位置，初始为(0, 0)。
	int tileDrawStartX = 0;
	int tileDrawStartY = 0;
	//从 Tileset 中取得地图的总宽度和高度（地图总共有多少 tile）。
	int tileswide = tileset.getTileswide();
	int tileshigh = tileset.getTileshigh();
	//绘制第一个 tile 的偏移量（对齐像素边界）
	int tileDrawStartOffsetY = CtileHeight - camPos.y % CtileHeight;
	int tileDrawStartOffsetX = CtileWidth - camPos.x % CtileWidth;
	for (int i = indexX; i < min((indexX + tilesNumX), tileswide); i++) {
		for (int j = indexY; j < min((indexY + tilesNumY), tileshigh); j++) {
			int tileNameIndex = tileset.getTileIndex(i, j);
			if (tileNameIndex == 24 || tileNameIndex == 25) {
				// draw grass tile under stone tile first(stone tile has alpha)
				tiles[0].draw(tileDrawStartX, tileDrawStartY, window);
			}
			tiles[tileNameIndex].draw( tileDrawStartX, tileDrawStartY,window );
			// adjust the starting position on the Y-axis
			if (tileDrawStartY == 0) tileDrawStartY += tileDrawStartOffsetY; // draw from pixel inside the tile png
			else tileDrawStartY += CtileHeight; // draw from pixel at the edge of tile png
		}
		// reset Y
		tileDrawStartY = 0;
		// adjust the starting position on the X-axis
		if (tileDrawStartX == 0) tileDrawStartX += tileDrawStartOffsetX;
		else tileDrawStartX += CtileWidth;
	}
}
bool World::checkTilePassablity(int x, int y)
{
	int leftX = x;
	int topY = y;
	int rightX = x + playerSpriteSize;
	int bottomY = y + playerSpriteSize;

	for (int i = leftX; i <= rightX; i = i + playerSpriteSize / 2)
		for (int j = topY; j <= bottomY; j = j + playerSpriteSize / 2) {
			int tileIndex = tileset.getTileIndex(i / CtileWidth, j / CtileWidth);
			if (!tiles[tileIndex].getPassablity()) return false;
		}
	return true;

}
