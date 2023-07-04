#include <DxLib.h>
#include <memory>
#include "TitleScene.h"
#include "GameScene.h"
#include "transition/Transition.h"
#include "transition/CrossOver.h"
#include "Input/InputID.h"
#include "../ImageMng.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "../../_debug/_debug/_DebugConOut.h"

TitleScene::TitleScene(int screenSizeX, int screenSizeY):BaseScene(screenSizeX,screenSizeY)
{
	Init();
	//�N���X"BaseScene"�̊���̃R���X�g���N�^�[�͑��݂��܂���c�ÖٓI�ɌĂ΂�Ȃ�
	//�����̎d���c�R���X�g���N�^�̕����̂�����ɁF�������ČĂяo���c�����I�ƌĂ�
	//����c���炩���ߌ��߂�ꂽ����
	//�f�t�H���g�R���X�g���N�^���Ȃ�����G���[���o��
	//�f�t�H���g�R���X�g���N�^�̖�ځc�C���X�^���X���ɏ���ɌĂ΂��
	TRACE("TitleScene�̐���\n");
	DrawOwnScreen();
}

TitleScene::~TitleScene()
{
	TRACE("TitleScene�̔j��\n");
	
}

bool TitleScene::Init(void)
{
	lpImageMng.GetID( "image/map.png","TitleBG");
	return true;
}

uniquScene TitleScene::Update(uniquScene ownScene)
{
	DrawOwnScreen();
	if (CheckHitKey(KEY_INPUT_SPACE) || GetJoypadInputState(PAD_INPUT_3))
	{
		//TitleScene���������̂�GameScene�Ƃ��Ċm�肷��
		//ownScene = TitleScene
		//ownScene = GameScene�ɂ���
		
		return std::make_unique<CrossOver>(std::move(ownScene),
			std::make_unique<GameScene>(screenSize_.x, screenSize_.y),
			screenSize_.x, screenSize_.y);
	}
	//�������g��Ԃ�
	//SceneMng��scene_�������Ă���
	
	//���[�J���ϐ��c�֐��𔲂���Ə��ł���
	//GameScene��return��������ł���
	//�j���^�C�~���O���ӎ����Ȃ��Ă悭�Ȃ�
	return ownScene;
}

void TitleScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0,lpImageMng.GetID("TitleBG")[0], true);
}
