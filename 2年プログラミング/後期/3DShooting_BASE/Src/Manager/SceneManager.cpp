#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/EventScene.h"
#include "../Scene/BattleScene.h"
#include "../Utility/Measure.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::mInstance = nullptr;

void SceneManager::CreateInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new SceneManager();
	}
	mInstance->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *mInstance;
}

void SceneManager::Init(void)
{

	mSceneID = SCENE_ID::TITLE;
	mWaitSceneID = SCENE_ID::NONE;

	mFader = new Fader();
	mFader->Init();

	mCamera = new Camera();
	mCamera->Init();

	/*mScene = new TitleScene();
	mScene->Init();*/

	mScene = new BattleScene();
	mScene->Init();

	mIsSceneChanging = false;

	// �f���^�^�C��
	mPreTime = std::chrono::system_clock::now();

	// 3D�p�̐ݒ�
	Init3D();

}

void SceneManager::Init3D(void)
{

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	//�|���S���̗�����\�����邩�ǂ���
	SetUseBackCulling(true);

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	//SetCameraNearFar(0.0f, 50.0f);

	// ���C�g�̐ݒ�
	//ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });
	SetUseLighting(false);

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);
	//SetBackgroundColor(0, 100, 100);

	// �J�����ݒ�
	// ������Ԃł͎��_�̈ʒu���A 
	// x = 320.0f, y = 240.0f, z = (��ʂ̃T�C�Y�ɂ���ĕω�)
	// �����_�̈ʒu�́A
	// x = 320.0f, y = 240.0f, z = 1.0f�A
	// �J�����̏������
	// x = 0.0f, y = 1.0f, z = 0.0f
	// �܂��ʂ̂w�x���ʏ�̒��S�ɋ��Ăy���̃v���X����������悤�ȃJ�����ɂȂ��Ă��܂��B
	//SetCameraPositionAndAngle(
	//	{ 0.0f, 200, -500.0f },
	//	30.0f * (DX_PI_F / 180.0f), 0.0f, 0.0f
	//);

	//�t�H�O�̐ݒ�(�J�������牓���ʒu�ɖ���������)
	SetFogEnable(true);
	SetFogColor(5,5,5);
	SetFogStartEnd(3000.0f, 15000.0f);




}

void SceneManager::Update(void)
{

	if (mScene == nullptr)
	{
		return;
	}

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	mDeltaTime = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - mPreTime).count() / 1000000000.0);
	mPreTime = nowTime;

	mFader->Update();
	if (mIsSceneChanging)
	{
		Fade();
	}
	else
	{
		mScene->Update();
	}

	// �J�����X�V�X�e�b�v
	mCamera->Update();

}

void SceneManager::Draw(void)
{
	
	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	mCamera->SetBeforeDraw();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	// �`��
	mScene->Draw();

	// ��Ƀ|�X�g�G�t�F�N�g�p
	mCamera->Draw();

	// �v���t�@�N�^�����O
	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		break;
	case SCENE_ID::GAME:
		// UI�`��
		mCamera->DrawUI();
		break;
	}

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
	
	mFader->Draw();

}

void SceneManager::Release(void)
{

	mScene->Release();
	delete mScene;

	delete mFader;

	mCamera->Release();
	delete mCamera;

}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{

	mWaitSceneID = nextId;

	if (isFading)
	{
		mFader->SetFade(Fader::STATE::FADE_OUT);
		mIsSceneChanging = true;
	}
	else
	{
		DoChangeScene();
	}

}

SceneManager::SCENE_ID SceneManager::GetmSceneID(void)
{
	return mSceneID;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return mDeltaTime;
}

Camera* SceneManager::GetCamera(void) const
{
	return mCamera;
}

SceneManager::SceneManager(void)
{

	mSceneID = SCENE_ID::NONE;
	mWaitSceneID = SCENE_ID::NONE;

	mScene = nullptr;
	mFader = nullptr;

	mIsSceneChanging = false;

	// �f���^�^�C��
	mDeltaTime = 1.0f / 60.0f;

	mCamera = nullptr;

}

SceneManager::~SceneManager(void)
{
	delete mInstance;
}

void SceneManager::ResetDeltaTime(void)
{
	mDeltaTime = 0.016f;
	mPreTime = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{

	// ���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	mScene->Release();
	delete mScene;

	mSceneID = mWaitSceneID;

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		mScene = new TitleScene();
		break;
	case SCENE_ID::GAME:
		mScene = new GameScene();
		break;
	case SCENE_ID::EVENT:
		mScene = new EventScene();
		break;
	case SCENE_ID::BATTLE:
		mScene = new BattleScene();
		break;
	default:
		break;
	}

	mScene->Init();

	// �v�����Z�b�g
	Measure::GetInstance().Reset();

	ResetDeltaTime();

	mWaitSceneID = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = mFader->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		if (mFader->IsEnd())
		{
			mFader->SetFade(Fader::STATE::NONE);
			mIsSceneChanging = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		if (mFader->IsEnd())
		{
			DoChangeScene();
			mFader->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


