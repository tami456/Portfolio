#pragma once
#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "../common/ImageCtl.h"
#include "../Scene/MapID.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

//�Ԃ�l��std::vector<int>;�ŏ����ƒ������߁Ausing�ŒZ�����O�ɂ���
using LayerData = std::vector<int>;
using MapData = std::map<std::string, LayerData>;
using Vec2List = std::list<Vector2D>;

class tmxObj
{
public:
	tmxObj();
	~tmxObj();
	bool LoadTSX(std::string fileName);
	bool LoadTMX(std::string fileName);
	const Vector2D& GetWorldArea(void);		//�Q�ƂȂ̂́A�I�u�W�F�N�g�𐶐��������Ȃ�����
	const Vector2D& GetTileSize(void);
	void Draw(void);		//�e�X�g�p
	//int GetAtrInt(rapidxml::xml_node<>* node, std::string str);
	bool SetMap(rapidxml::xml_document<>& tmxDoc);
	void DrawMapData(std::string layer, int x, int y, const Vector2D& camOffset);
	const MapData& GetMapData(void);

	const int GetMapData(std::string layer, const Vector2D& pos);
	const int GetMapData(std::string layer, int x, int y);	//GetMapData������

	bool SetMapData(int id,std::string layer,Vector2D pos);	//pos�͍��W
	bool SetMapData(int id,std::string layer,int x,int y);	//x,y�̓}�X�ڔ�
	Vec2List SearchID(MapID mapID,std::string layer);
private:
	ImageVec imageObj_;		//vector
	
	//TSX
	std::map<std::string, int> virsionTSX_;

	//TMX
	std::map<std::string, int> virsionTMX_;
	unsigned int firstGID_;
	unsigned int layerSize_;
	Vector2D worldArea_;
	Vector2D tileSize_;
	MapData mapData_;
};
