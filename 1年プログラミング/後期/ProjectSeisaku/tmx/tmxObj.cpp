#include <dxlib.h>
#include "tmxObj.h"
tmxObj::tmxObj()
{
	virsionTSX_["1.5"] = 1;
	virsionTMX_["1.5"] = 1;		//�o�[�W�����������Ă����瑝�₷
	//����m�F�����ĂȂ�������e���@�z���C�g���X�g����
	firstGID_ = 0;
	layerSize_ = 0;
}

tmxObj::~tmxObj()
{
}

bool tmxObj::LoadTSX(std::string fileName)
{
	rapidxml::file<> xmlFileData = fileName.c_str();		//xml�`���̃o�C�i���f�[�^�ɂȂ�

	//��̓f�[�^���h�L�������g�Ɋi�[����
	rapidxml::xml_document<> tsxDoc_;
	tsxDoc_.parse<0>(xmlFileData.data());						//xmlFileData��vector�^�Ȃ̂ŁA.data()������

	//���
	auto tileset = tsxDoc_.first_node("tileset");				//�擪�̃m�[�h
	auto version = tileset->first_attribute("version");			//tileset�̃A�g���r���[�g�Aversion���i�[
	
	//�t�@�C����:			�^�O:image �A�g���r���[�g:source="map.png"
	//���������E�c:			�^�O:image �A�g���r���[�g:width="80",height="60"
	//						�^�O:tileset �A�g���r���[�g:tilewidth="20",tileheight="20"
	//�����T�C�Y���E����:	�^�O:tileset �A�g���r���[�g:tilewidth="20",tileheight="20"

	auto imageNode = tileset->first_node("image");				//image�m�[�h
	if (!imageNode)
	{
		return false;
	}
	std::string source = imageNode->first_attribute("source")->value();				//image�m�[�h
	auto tilewidth = atoi(tileset->first_attribute("tilewidth")->value());
	auto tileheight = atoi(tileset->first_attribute("tileheight")->value());
	//map.png�̕��E����
	auto divCntX = atoi(imageNode->first_attribute("width")->value()) / tilewidth;
	auto divCntY = atoi(imageNode->first_attribute("height")->value()) / tileheight;

	imageObj_ = ImageLoader().execute(source, Vector2D{ divCntX,divCntY }, Vector2D{tilewidth,tileheight});
	
	if (!virsionTSX_.count(version->value()))
	{
		return false;
	}
	if (virsionTSX_[version->value()] != 1)
	{
		//DrawFormatString(0, 0, 0xff0000, "version:%s\n", version->value());
		//TRACE("version�ǂݍ��ݎ��s ���݂�version:%d\n",version);
		return false;
	}
	return true;
}

bool tmxObj::LoadTMX(std::string fileName)
{
	rapidxml::file<> xmlFileData = fileName.c_str();		//xml�`���̃o�C�i���f�[�^�ɂȂ�
	rapidxml::xml_document<> tmxDoc_;
	tmxDoc_.parse<0>(xmlFileData.data());					//xmlFileData��vector�^�Ȃ̂ŁA.data()������

	//�m�[�h���擾
	//�擪�̃m�[�h���擾�@���O���킩��Ȃ��Ă��擾���邱�Ƃ��ł���
	//while (mapNode)
	//{
	//	if (mapNode->name_size())							//->name_size() : �m�[�h���̕����񒷂��擾
	//	{
	//		TRACE("%s\n��", mapNode->name());					//->name() : �m�[�h�����擾
	//	}

	auto num = fileName.find_last_of("/");
	std::string pass = fileName.substr(0, num + 1);		//num��������data/tmx�ɂȂ�

	auto mapNode = tmxDoc_.first_node("map");
	if (mapNode == nullptr)
	{
		return false;
	}
	auto tilesetNode = mapNode->first_node("tileset");

	//auto source = tilesetNode->first_attribute("source");
	auto source = mapNode->first_node("tileset")->first_attribute("source");

	if (source->name_size())
	{
		TRACE("%s:->\n", source->name());
		TRACE("%s:\n", source->value());
		TRACE("%s", pass.c_str());
		//TRACE("%s ", source->value());
	}
	LoadTSX(pass + source->value());
//		if (atr->value_size())
	
	auto getAttrInt = [](rapidxml::xml_node<>* node, std::string str) {
		auto attr = node->first_attribute(str.c_str());
		if (attr == nullptr)
		{
			return 0;
		}
		return atoi(attr->value());
	};
	firstGID_ = getAttrInt(tilesetNode,"firstid");

	layerSize_ = getAttrInt(mapNode, "nextlayerid");

	//layerSize_ = GetAtrInt(mapNode->first_node("tileset"),"nextlayerid") - 1;
	worldArea_ = Vector2D{ getAttrInt(mapNode,"width"),getAttrInt(mapNode,"height") };

	tileSize_ = Vector2D{ getAttrInt(mapNode,"tilewidth"),getAttrInt(mapNode,"tileheight") };
	//	mapNode = mapNode->first_node();			//->fist_node() : ������艺�̃m�[�h���擾
	//	/apNode = mapNode->next_sibling();		//->next_sibling() : ���̓����K�w�̃m�[�h���擾
	//}
	
	SetMap(tmxDoc_);
	return false;
}

const Vector2D& tmxObj::GetWorldArea(void)
{
	return worldArea_;
}

const Vector2D& tmxObj::GetTileSize(void)
{
	return tileSize_;
}

void tmxObj::Draw(void)		//�e�X�g�p
{
	DrawGraph(0, 0, imageObj_[0], true);
}

bool tmxObj::SetMap(rapidxml::xml_document<>& tmxDoc)
{
	auto mapNode = tmxDoc.first_node("map");
	if (mapNode == nullptr)
	{
		TRACE("mapNode=NULL\n");
		return false;
	}

	for (auto layerNode = mapNode->first_node("layer");		//layerNode�K�w�̈�ԍŏ�
		layerNode != nullptr;								//layerNode�̎����Ȃ��Ȃ�܂�
		layerNode = layerNode->next_sibling()				//�����K�w�̎��̃m�[�h���擾
		)
	{

		if (layerNode == nullptr)
		{
			TRACE("layerNode=NULL\n");
			return false;
		}

		auto layerName = layerNode->first_attribute("name");
		if (layerName == nullptr)
		{
			TRACE("layerName=NULL\n");
			return false;
		}

		//���g�����Ă���
		//���ʂ�����
		auto result = mapData_.try_emplace(layerName->value());//�ꏊ�����p��

		//�Q�ƌ^��auto�@���_���Ă���Ȃ��̂ŁA�����I�ɏ���
		auto& mapPair = *result.first;
		mapPair.second.resize(static_cast<size_t>(worldArea_.x)* static_cast<size_t>(worldArea_.y));		//std::vector��int�����o����

		auto dataNode = layerNode->first_node("data");
		if (dataNode == nullptr)
		{
			TRACE("dataNode=NULL\n");
			return false;
		}

		auto csvNode = dataNode->first_node();//�^�O���Ȃ��̂ŁA�m�[�h���͏����Ȃ�
		TRACE("%s", csvNode->value());

		std::stringstream dataStream;
		dataStream << csvNode->value();
		//���ړ��ꂽ���̂ŁA�Q�Ƃ�auto
		for (auto& data : mapPair.second)
		{
			std::string numStr;
			//�g�p���镶����,�i�[��,�������镶��(�J���}�ŋ�؂��ĕ����A�i�[��Ɋi�[����)
			std::getline(dataStream, numStr, ',');

			data = atoi(numStr.c_str())-1;	//�f�[�^���i�[���Ȃ��i�K�Ń}�C�i�X�P����
		}
	}


	return false;
}

void tmxObj::DrawMapData(std::string layer, int x, int y, const Vector2D& camOffset)
{
	//mapData_�ɂ͂P�Ԉȍ~�Ƀf�[�^�������Ă���
	int point = x + y * worldArea_.x;

	if (!mapData_.count(layer))
	{
		TRACE("�w�肵��layer�����݂��Ȃ�\n");
		return;
	}
	if (x < 0 || x > worldArea_.x ||
		y < 0 || y > worldArea_.y)
	{
		TRACE("x�܂���y���͈͊O\n");
		return;
	}
	if (point < 0 || point >= mapData_[layer].size())
	{
		TRACE("point���O�ȉ�\n");
		return;
	}
	if (mapData_[layer][point] < 0)
	{
		return;
	}
	DrawGraph(x * tileSize_.x + camOffset.x, y * tileSize_.y + camOffset.y, imageObj_[mapData_[layer][point]], true);
}

const MapData& tmxObj::GetMapData(void)
{
	return mapData_;
}

const int tmxObj::GetMapData(std::string layer, const Vector2D& pos)
{
	Vector2D grid = pos / tileSize_;

	return GetMapData(layer,grid.x,grid.y);
}

const int tmxObj::GetMapData(std::string layer, int x, int y)
{
	int point = x + y * worldArea_.x;
	if (!mapData_.count(layer))
	{
		TRACE("SetMapData:�w�肵��layer�����݂��Ȃ�\n");
		return -1;
	}
	if (x < 0 || x > worldArea_.x ||
		y < 0 || y > worldArea_.y)
	{
		TRACE("SetMapData:x�܂���y���͈͊O\n");
		return -1;
	}
	if (point < 0 || point >= mapData_[layer].size())
	{
		TRACE("SetMapData:point���O�ȉ�\n");
		return -1;
	}
	return mapData_[layer][point];
}

bool tmxObj::SetMapData(int id, std::string layer, Vector2D pos)
{
	Vector2D grid = pos / tileSize_;
	
	return SetMapData(id, layer, grid.x,grid.y);
}

bool tmxObj::SetMapData(int id, std::string layer, int x, int y)
{
	int point = x + y * worldArea_.x;
	if (!mapData_.count(layer))
	{
		TRACE("SetMapData:�w�肵��layer�����݂��Ȃ�\n");
		return false;
	}
	if (x < 0 || x > worldArea_.x ||
		y < 0 || y > worldArea_.y)
	{
		TRACE("SetMapData:x�܂���y���͈͊O\n");
		return false;
	}
	if (point < 0 || point >= mapData_[layer].size())
	{
		TRACE("SetMapData:point���O�ȉ�\n");
		return false;
	}
	if (mapData_[layer][point] < 0)
	{
		return false;
	}

	mapData_[layer][point] = id;
	return true;
}

Vec2List tmxObj::SearchID(MapID mapID, std::string layer)
{
	Vec2List vec2List;
	if (!mapData_.count(layer))
	{
		return std::move(vec2List);
	}
	int id = static_cast<int>(mapID);
	for (int point = 0; point < mapData_[layer].size(); point++)
	{
		if (mapData_[layer][point] == id)
		{
			vec2List.emplace_back(point % worldArea_.x, point / worldArea_.x);
		}
	}
	return std::move(vec2List);
}

//int tmxObj::GetAtrInt(rapidxml::xml_node<>* node, std::string str)
//{
//
//	//return (attr->next_attribute());
//	return 0;
//}
