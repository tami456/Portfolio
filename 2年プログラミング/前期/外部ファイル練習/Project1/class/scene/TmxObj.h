#pragma once
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
#include "../ImageMng.h"
#include "../Vector2D.h"
#include "../common/Collision.h"
#include "../common/Raycast.h"

using MapData = std::map<std::string, std::vector<int>>;

class TmxObj
{
public:
	//Tmx�̖���rapidxml��ʂ���xml��ǂݍ���
	//�J���̂�rapidxml�c�p�[�T�[(rapidxml)
	//Load������TmxObj�c���[�_�[(TmxObj)
	TmxObj();
	~TmxObj();

	bool LoadTMX(std::string fileName);
	bool LoadTSX(std::string fileName);
	bool SetMap(rapidxml::xml_node<>* node);

	const ColList& GetColList(void);
	const MapData& GetMapData(void);
	const Vector2D& GetWorldArea(void);
	const Vector2D& GetTileSize(void);
	const std::string& GetMapKey(void);
	Raycast raycast_;

private:
	//rapidxml�̍\���@�h�L�������g(�t�@�C��(TMX))->�m�[�h(�^�O)->�A�g���r���[�g

	//TSX
	//�e���v���[�g
	//��`�̕���char�^�Ƃ��Ē�`����Ă���̂�<>�ł��G���[�͏o�Ȃ�(�ÖٓI)
	rapidxml::xml_document<> tsxDoc_;
	

	//TMX
	//-�l���Ȃ�
	unsigned int layerSize_;
	Vector2D worldArea_;
	Vector2D tileSize_;
	ColList colList_;

	//string�̓N���X�Ȃ̂ŋN�����x���̂Ŕ�������������
	MapData mapData_;
	std::string mapKey_;
};

