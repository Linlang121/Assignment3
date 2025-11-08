#include "Tilesset.h"
#include "GamesEngineeringBase.h"
#include "Constvalues.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
using namespace GamesEngineeringBase;
// tile 类的成员函数实现
tile::tile()
{
	passablity = true;
}
 bool tile::getPassablity()const
{
	return passablity;
}
void tile::setUnpassable()
{
	passablity = false;
}
void tile::setPassable()
{
	passablity = true;
}

	void tile::loadImage(const std::string & filename)
	{
		if (!image.load(filename))
		{
			cerr << "❌ Failed to load image: " << filename << endl;
		}
	}

void tile::draw( int x, int y, Window& window)
{
	// 如果图像未加载或数据为空，直接返回
	if (image.width == 0 || image.height == 0 || image.data == nullptr)
	{
		return;
	}
	
	for (unsigned int i = 0; i < image.width; i++) {
		if (x + i >= 0 && x + i < window.getWidth()) {
			for (unsigned int j = 0; j < image.height; j++) {
				if (y + j >= 0 && y + j < window.getHeight()) {
					if (image.alphaAt(i, j) > 200)
						window.draw(x + i, y + j, image.at(i, j));
				}
			}
		}
	}
}
// tilesset 类的成员函数实现
tilesset::tilesset()	
{
	tileswide = Ctileswide;
	tileshigh = Ctileshigh;
	tileWidth = CtileWidth;
	tileHeight= CtileHeight;
	layer = 0;
	tilesarr = nullptr; // 初始化为 nullptr
}
int tilesset::getTileshigh()const
{
	return tileshigh;
}
int tilesset::getTileswide()const
{
	return tileswide;
}

int tilesset::load(const std::string& filename)
{
	ifstream file(filename);
	string index;
	if (!file.is_open())
	{
		cerr << "Failed to open tileset file: " << filename << endl;
		return -1;
	}

	//数组名可以看作“指向第一个元素的常量指针，语法是：指针=new 数据类型 [元素个数];
	//二维数组可以看作“指向一维数组的常量指针”，语法是：指针=new 数据类型* [一维数组个数];
	tilesarr = new int*[tileshigh];
for (int i = 0; i < tileshigh; i++)
    tilesarr[i] = new int[tileswide];

	//每次 getline(infile, index, ',') 执行时，都会把文件里读到的 下一个数字（字符串形式） 存进 index;
	// tilesarr 是 [行][列]，即 tilesarr[tileshigh][tileswide]
	// j 是行号，i 是列号
	for (int j = 0; j < tileshigh; j++)
	{
		for (int i = 0; i < tileswide; i++)
		{
			getline(file, index, ',');
			tilesarr[j][i] = stoi(index);
			tileIndex = tilesarr[j][i];
		}
		
	}
	return 0;
}
int tilesset::getTileIndex(int i, int j)const
{	
	if (tilesarr == nullptr)
	{
		return 0; // 如果数组未初始化，返回默认值 0
	}
	return tilesarr[j][i];
}

