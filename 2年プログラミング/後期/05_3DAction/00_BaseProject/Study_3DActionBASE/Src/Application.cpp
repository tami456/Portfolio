#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Manager/GravityManager.h"
#include "Application.h"

Application* Application::mInstance = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";

void Application::CreateInstance(void)
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	mInstance->Init();
}

Application& Application::GetInstance(void)
{
	return *mInstance;
}

void Application::Init(void)
{

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("3DAction");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	mIsInitFail = false;
	if (DxLib_Init() == -1)
	{
		TRACE("DxLib �̏��������s");
		mIsInitFail = true;
		return;
	}

	//Effekseer�̏�����
	InitEffekseer();

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// �f�o�b�O�@�\������
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	// ���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	auto& intpuManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		_dbgStartDraw();

		intpuManager.Update();
		sceneManager.Update();

		sceneManager.Draw();

		_dbgAddDraw();

		ScreenFlip();

	}

}

void Application::Release(void)
{
	//Effekseer���I������
	Effkseer_End();


	// DxLib�I��
	if (DxLib_End() == -1)
	{
		mIsReleaseFail = true;
	}

}

bool Application::IsInitFail(void) const
{
	return mIsInitFail;
}

bool Application::IsReleaseFail(void) const
{
	return mIsReleaseFail;
}

Application::Application(void)
{
	mIsInitFail = false;
	mIsReleaseFail = false;
}

Application::~Application(void)
{
	delete mInstance;
}

void Application::InitEffekseer(void)
{
	//��ʂɕ\���\�ȍő�p�[�e�B�N����
	if (Effekseer_Init(8000) == -1)
	{
		//���ꂪ��������DxLib���I��点�Ȃ��Ƃ����Ȃ�
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(false);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}
