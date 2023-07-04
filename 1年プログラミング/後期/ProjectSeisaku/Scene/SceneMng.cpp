#include <memory>
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "../_debug/_DebugDispOut.h"
#include "../_debug/_DebugConOut.h"

//SceneMng* SceneMng::s_Instance = nullptr;		//�|�C���^
constexpr int ScreenSizeX = 640;
constexpr int ScreenSizeY = 480;

SceneMng::SceneMng()
{
	initFlag_ = SysInit();
}

SceneMng::~SceneMng()
{
}

void SceneMng::Run(void)
{
	if (!initFlag_)		//���s������A�������Ȃ�
	{
		return;
	}
	std::unique_ptr<BaseScene> scene = std::make_unique<TitleScene>();




	//reset()�̂݁����g���k�ɂ���	

	//�h�b�g�ŏo�Ă���get��unique_ptr�N���X�̃����o�[�֐��A�����o�[�ϐ�
	//->�ł͔��ŁABaseScene�̊֐����g����

	//BaseScene* scene = new TitleScene();

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();
		//scene.get()->Update();
		//(*scene).Update();	//���̂ɃA�N�Z�X �܂���->

		scene = scene->Update(std::move(scene));	//BaseScene��������A������V�[����Title�Ȃ̂ŁATitle��DrawOwn���Ă΂��

		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		scene->Draw();
		//_dbgAddDraw();
		ScreenFlip();
	}

}

Vector2D SceneMng::GetScreenSize(void)
{
	return {ScreenSizeX,ScreenSizeY};
}

bool SceneMng::SysInit(void)
{
	SetWindowText("2116020_���c��");
	SetGraphMode(ScreenSizeX, ScreenSizeY, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)				//DxLib�̏�����
	{
		//���������s
		return false;
	}
	SetWindowIconID(333);
	_dbgSetup(ScreenSizeX, ScreenSizeY, 255);
	return true;
}
