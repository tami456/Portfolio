#include <DxLib.h>
#include "BaseScene.h"

//�R���X�g�̌��ɏ����Ə������̗\�񂪂ł���
//�K�v�Ȃ���΃R���X�g���N�^���f�X�g���N�^������Ȃ�
BaseScene::BaseScene(int screenSizeX, int screenSizeY):screenSize_(screenSizeX,screenSizeY)
{

	//GetDrawScreenSize(&screenSize_.x, &screenSize_.y);
	screenID_ = MakeScreen(screenSize_.x, screenSize_.y, true);
}

BaseScene::~BaseScene()
{

}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, true);
}
