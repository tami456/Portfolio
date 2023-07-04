#pragma once
#include <string>
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
class GetAtr 
{
public:
	//�ŏ���()�͂ǂ̃I�y���[�^�[���I�[�o�[���C�h���邩
	//2�Ԗڂ�()�̓I�[�o�[���C�h�����֐��Ăяo����()�ɑ΂��Ă̈���
	bool operator()(rapidxml::xml_node<>* node,std::string atrName, int& num)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		num = atoi(atr->value());
		return true;
	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, unsigned int& str)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		str = atoi(atr->value());
		return true;
	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, double& num)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		num = atof(atr->value());
		return true;
	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, std::string& str)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		str = atr->value();
		return true;
	}

};
