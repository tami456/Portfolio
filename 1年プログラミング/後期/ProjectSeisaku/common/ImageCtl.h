#pragma once

#include <string>		//��������������Ƃ��ł���N���X
#include <dxlib.h>
#include <vector>
#include "Vector2D.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

using ImageVec = std::vector<int>;

struct DrawObj			//�`��p�̃I�u�W�F�N�g
{
	ImageVec imageObj;		//�摜���i�[
	Vector2D pos;
	void Draw(void);
};

struct ImageLoader		//�摜�ǂݍ��ݗp�̃N���X
{
	ImageVec execute(std::string imageName)//LoadGraph��
	{
		ImageVec imageObj;
		imageObj.resize(1);
		imageObj[0] = LoadGraph(imageName.c_str());
		if (imageObj[0] == -1)
		{
			TRACE("�摜�̓ǂݍ��ݎ��s");
		}
		return std::move(imageObj);
	}
	ImageVec execute(std::string imageName,Vector2D divCnt,Vector2D divSize)//LoadDivGraph��
	{
		ImageVec imageObj;
		imageObj.resize(static_cast<size_t>(divCnt.x) * static_cast<size_t>(divCnt.y));
		
		auto flag = LoadDivGraph(("data/image/"+imageName).c_str(),
			divCnt.x * divCnt.y,divCnt.x,divCnt.y,
			divSize.x, divSize.y, &imageObj[0]);

		//imageObj.data() &imageObj[0]
		if (flag == -1)
		{
			TRACE("�摜�̓ǂݍ��ݎ��s");
		}

		return std::move(imageObj);
	}
};

