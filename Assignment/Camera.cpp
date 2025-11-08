#include "Camera.h"
#include "Constvalues.h"
#include "GamesEngineeringBase.h"
#include <fstream>
using namespace std;
using namespace GamesEngineeringBase;

Camera::Camera(Window& window)
{
	height = window.getHeight();
	width = window.getWidth();
	// 初始化相机位置为 (0, 0)
	position.x = 0;
	position.y = 0;
}
Vec Camera::getPosition()const
{
	return position;
}
int Camera::getWidth()const
{
	return width;
}
int Camera::getHeight()const
{
	return height;
}
//让摄像机跟随玩家移动，同时确保画面不会超出地图边界
void Camera::update(const Vec& playerpos)
{
	position.x = playerpos.x -getWidth() / 2;
	position.y = playerpos.y - getHeight() / 2;
	if(position.x < 0)
		position.x = 0;
	if(position.y < 0)
		position.y = 0;
	//mapwide=tileswide1 * tileWidth1;
	//确保摄像机不会超出地图右边界和下边界
	if (position.x > Ctileswide * CtileWidth - getWidth())
		position.x = Ctileswide * CtileWidth - getWidth();
	if (position.y > Ctileshigh * CtileHeight - getHeight())
		position.y = Ctileshigh * CtileHeight - getHeight();

}
//将世界坐标转换为屏幕坐标
Vec Camera::worldToScreen(const Vec& worldpos)const
{
	Vec screenpos;
	screenpos.x = worldpos.x - position.x;
	screenpos.y = worldpos.y - position.y;
	return screenpos;
}
/*存档读档
void Camera::serialize(ofstream& out) const {
	out.write(reinterpret_cast<const char*>(&position), sizeof(position));
}

void Camera::deserialize(ifstream& in) {
	in.read(reinterpret_cast<char*>(&position), sizeof(position));
}*/