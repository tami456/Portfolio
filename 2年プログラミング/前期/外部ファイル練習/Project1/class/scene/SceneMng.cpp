#include <DxLib.h>
#include "../../_debug/_debug/_DebugConOut.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "BaseScene.h"
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"

//�悭�킩��Ȃ������������Ă����Ԃ��ǂ��Ȃ����珉�������邽��null�������
//�悭�킩��Ȃ����� = �s��l
constexpr int ScreenSizeX = 1024;
constexpr int ScreenSizeY = 768;

void SceneMng::Run(void)
{
	//SysInit�����s���Ă����ꍇ
	if (!SysInit())
	{
		return;
	}
	//���Ƃ���C���X�^���X����
	//unique_ptr�̏ꍇ
	//�ǂ�����d�v
	//scene_.reset(new BaseScene(ScreenSizeX,ScreenSizeY));
	//C++��11�ȍ~�̋@�\
	//�A���h���C�h�J���ł�make_unique���Ȃ��̂ŏ��reset�̕�������
	scene_ = std::make_unique<TitleScene>(ScreenSizeX, ScreenSizeY);

	//�v���O���~���O�͐l�ɗႦ��ꂻ��

	/*SetDrawScreen(DX_SCREEN_BACK);*/
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		
  		Update();
		Draw();
		//SetDrawScreen(DX_SCREEN_BACK);
		//ClsDrawScreen();
		//
		////_dbgAddDraw();
		//ScreenFlip();
	}
}

void SceneMng::Update(void)
{
	_dbgStartDraw();
	//�K�[�h����
	if (!scene_)
	{
		TRACE("�yError�zscene_�̒��g����\n");
		return;
	}
	//*scene_.Update�@�G���[���o��B.�̓A�h���X���m�𑫂����Z
	//*��.���D��x���Ⴂ*scene_ .Update�ł͂Ȃ� * scene_.Update�ɂȂ�
	//�l�����Z�q�Ɠ���
	//uniqu_ptr�̓R�s�[�ł��Ȃ�
	//std::move = ���g�̏��L���𑼂̐l�ɓn�����Ƃ͂ł���
	//���Ӓl���E�Ӓl�ɕϊ�

	//�V���^�b�N�X=���[���A���@�A������
	scene_ = scene_->Update(std::move(scene_));
}

void SceneMng::Draw(void)
{
	if (!scene_)
	{
		TRACE("�yError�zscene_�̒��g����\n");
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	scene_->Draw();
	_dbgDraw();
	ScreenFlip();
}

bool SceneMng::SysInit(void)
{
	SetGraphMode(ScreenSizeX, ScreenSizeY, 32);
	ChangeWindowMode(true);
	SetWindowText("2116020 ���c��");
	
	if (DxLib_Init() == -1)
	{
		//���������s
		return false;
	}
	_dbgSetup(ScreenSizeX, ScreenSizeY, 255);
	return true;
}

SceneMng::SceneMng()
{
}

SceneMng::~SceneMng()
{
}
