#include "TmxObj.h"
#include "../ImageMng.h"
#include "GetAtr.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "../../_debug/_debug/_DebugConOut.h"

TmxObj::TmxObj()
{
	
}

TmxObj::~TmxObj()
{

}

//fileName�̒���tmx�܂ł̑��΃p�X�������Ă���
bool TmxObj::LoadTMX(std::string fileName)
{
	rapidxml::file<> xmlFileData = fileName.c_str();
	tsxDoc_.parse<0>(xmlFileData.data());

	rapidxml::xml_node<>* mapNode = tsxDoc_.first_node("map");
	if (mapNode == nullptr)
	{
		return false;
	}

	auto tilesetNode = mapNode->first_node("tileset");
	if (tilesetNode == nullptr)
	{
		return false;
	}

	auto tsxSource = tilesetNode->first_attribute("source");
	if (tsxSource == nullptr)
	{
		return false;
	}

	//auto getAtr = [&](std::string atrName, auto& value) {
	//	auto atrStr = mapNode->first_attribute(atrName.c_str());
	//	if (atrStr == nullptr)
	//	{
	//		TRACE("�A�g���r���[�g�̎擾�G���[ : %s",atrName.c_str());
	//		return false;
	//	}
	//	//�������int�ɕϊ�
	//	value = atoi(atrStr->value());
	//	return true;
	//};

	auto getAtr = GetAtr();
	if (!getAtr(mapNode,"nextlayerid", layerSize_))
	{
		return false;
	}
	//nextlayerid�̒l�̓��C���[�������炷���+1����Ă��邽�ߌ��Z
	layerSize_--;
	if (!getAtr(mapNode, "width", worldArea_.x))
	{
		return false;
	}
	if (!getAtr(mapNode, "height", worldArea_.y))
	{
		return false;
	}
	if (!getAtr(mapNode, "tilewidth", tileSize_.x))
	{
		return false;
	}
	if (!getAtr(mapNode, "tileheight", tileSize_.y))
	{
		return false;
	}
	//�~�������͎̂��s������̑��΃p�X
	//substr = ������̎w�肵���ꏊ����ꏊ�܂ł̕�����̎w��(�ŏ��̕�����0�������Z)
	//�����̎d���͈�ԍŏ��Ɍ����������ƈ�ԍŌ�Ɍ����������̓������
	//�擾�����������i"./tmx/map.tmx"�j�̓�ڂ�/
	//find_last_of = �Ō�Ɍ����w�蕶�������������
	
	auto filePass = fileName.substr(0, fileName.find_last_of("/") + 1);

	//std::string tsxStr = tsxSource->value();
	
	//TMX����TSX�ւ̐ڑ�
	LoadTSX(filePass + tsxSource->value());



	/*if (!SetMap(mapNode))
	{
		return false;
	}*/

	//����ȏ㉺�ɏ������̂��Ȃ���΃��_�C���N�g�ł��ǂ�
	return SetMap(mapNode);
}

bool TmxObj::LoadTSX(std::string fileName)
{
	//����Ńt�@�C�����J����(�J��������)
	rapidxml::file<> xmlFile = fileName.c_str();
	
	//�|�C���^�������
	//xmlFile.data();
	
	//�|�C���^��n����
	tsxDoc_.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* tilesetNode = tsxDoc_.first_node("tileset");
	if (tilesetNode == nullptr)
	{
		return false;
	}
	//auto�ł��ǂ�
	//("")�̒��g���킩��Ȃ��ꍇ�͓��̕�����ǂݍ���ł���
	rapidxml::xml_attribute<>* version = tilesetNode->first_attribute("version");
	TRACE("TSX Version:%s\n", version->value());

	rapidxml::xml_attribute<>* tiledversion = tilesetNode->first_attribute("tiledversion");
	TRACE("TSX TiledVersion:%s\n", tiledversion->value());

	//�\�������������Ƃ��Ƀ����_���̕�����������
	//auto getAtr = [&](std::string atrName,int& num) {
	//	auto atr = tilesetNode->first_attribute(atrName.c_str());
	//	if (atr == nullptr)
	//	{
	//		return false;
	//	}
	//	num = atoi(atr->value());
	//	return true;
	//};
	
	int tilewidth;
	int tileheight;
	int tilecount;
	int columns;

	//���̂�����Ă��������n��
	auto getAtr = GetAtr();
	
	//1��g���̂Ă̏ꍇ��if (!GetAtr()("tilewidth", tilewidth))
	if (!getAtr(tilesetNode,"tilewidth", tilewidth))
	{
		return false;
	}
	if (!getAtr(tilesetNode, "tileheight", tileheight))
	{
		return false;
	}
	if (!getAtr(tilesetNode, "tilecount", tilecount))
	{
		return false;
	}
	if (!getAtr(tilesetNode, "columns", columns))
	{
		return false;
	}


	rapidxml::xml_node<>* imageNode = tilesetNode->first_node("image");
	if (imageNode == nullptr)
	{
		return false;
	}
	rapidxml::xml_attribute<>* source = imageNode->first_attribute("source");
	if (source == nullptr)
	{
		return false;
	}
	
	std::string sourcePass = source->value();
	//auto sourceName = "./tmx" + sourcePass.substr(1);
	mapKey_ = "mapChip";
	//map�t�@�C����ւ񂪂������������I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I
	lpImageMng.GetID("./tmx" + sourcePass.substr(1), mapKey_, { tilewidth ,tileheight },{ columns,tilecount/ columns });
	  
	return true;
}

bool TmxObj::SetMap(rapidxml::xml_node<>* node)
{
	if (node == nullptr)
	{
		return false;
	}
	
	auto tileset = node->first_node("tileset");
	if (tileset == nullptr)
	{
		return false;
	}

	auto getAttr = GetAtr();
	int firstgid;
	getAttr(tileset,"firstgid", firstgid);

	//layer���[�v , null����Ȃ������甲����next�ɍs��
	for (auto layerNode = node->first_node("layer");
		layerNode != nullptr;
		layerNode = layerNode->next_sibling())
	{
		
		if (layerNode == nullptr)
		{
			return false;
		}
		

		std::string layerName;
		getAttr(layerNode, "name", layerName);
		if (layerName == "Col")
		{
			//Col�������珈���𒆒f���ď�ɍs����next����
			continue;
		}

		Vector2D layerSize;
		getAttr(layerNode, "width", layerSize.x);
		getAttr(layerNode, "height", layerSize.y);
		

		//��ׂĈႩ������
		if (worldArea_ != layerSize)
		{
			return false;
		}

		
		auto layerData = mapData_.try_emplace(layerName);

		//first���L�[ second���f�[�^
		if (layerData.second)
		{
			//�v�f�����邽�߂̔�
			layerData.first->second.resize(worldArea_.x * worldArea_.y);
		}
		auto data = layerNode->first_node("data");
		if (data == nullptr)
		{
			return false;
		}
		auto csvData = data->first_node();

		std::stringstream dataStream;

		//=�Ɠ���
		dataStream << csvData->value();


		//std::vector��1�v�f�����o��for��
		for (auto& vecData : layerData.first->second)
		{
			std::string numStr;

			//�ǂݍ��ޕ���,�ǂݍ��񂾕���������ꏊ,�����̋�؂�
			getline(dataStream, numStr, ',');

			//�����1������荞��ŕۑ����J��Ԃ���
			//���̂܂܂��Ƃ���Ă��܂��̂�firstgid������
			vecData = atoi(numStr.c_str()) - firstgid;
		}
	}
	for (auto colNode = node->first_node("objectgroup");
		colNode != nullptr;
		colNode = colNode->next_sibling())
	{
		std::string grpName;
		getAttr(colNode, "name", grpName);
		if (grpName != "Col")
		{
			continue;
		}

		for (auto objNode = colNode->first_node("object");
			objNode != nullptr;
			objNode = objNode->next_sibling())
		{
			double x, y, w, h;
			getAttr(objNode, "x", x);
			getAttr(objNode, "y", y);
			getAttr(objNode, "width", w);
			getAttr(objNode, "height", h);
			colList_.push_back(
				Collision{
					//first��x,y
					Vector2DDouble{x,y},
					//second��w,h������
					Vector2DDouble{w,h},
				}
			);

			//GetAtr��double��atoi��atof�ɕύX
			//���������_�ɕϊ�����Vector2Ddouble�Ɋi�[���ăR���W�����̏�Ԃ���pushback�ŃR�����X�g�ɓ����
		}

	}
	return true;
}


const ColList& TmxObj::GetColList(void)
{
	return colList_;
}

const MapData& TmxObj::GetMapData(void)
{
	return mapData_;
}

const Vector2D& TmxObj::GetWorldArea(void)
{
	return worldArea_;
}

const Vector2D& TmxObj::GetTileSize(void)
{
	return tileSize_;
}

const std::string& TmxObj::GetMapKey(void)
{
	return mapKey_;
}

//UTF-8�Ƃ��������R�[�h��ǂݍ���ł���Ƃ����Ӗ�
//encoding = UTF-8