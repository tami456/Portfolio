#include <chrono>
#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "../Common/Fader.h"
#include "../Manager/GravityManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/GameOverScene.h"
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

	// �d�͊Ǘ�������
	GravityManager::CreateInstance();

	mFader = new Fader();
	mFader->Init();

	mCamera = new Camera();
	mCamera->Init();

	mIsSceneChanging = false;

	// �f���^�^�C��
	mPreTime = std::chrono::system_clock::now();

	// 3D�p�̐ݒ�
	Init3D();

	// �����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 139, 139);

	// �t�H�O�ݒ�
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

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

	// �d�͕����̌v�Z
	GravityManager::GetInstance().Update();

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

	//Effekseer�̍X�V����
	UpdateEffekseer3D();

	// �`��
	mScene->Draw();

	//Effekseer�̕`�揈��
	DrawEffekseer3D();

	// ��Ƀ|�X�g�G�t�F�N�g�p
	mCamera->Draw();

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

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// �t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	// �J�ڐ�V�[���������o�ϐ��ɕێ�
	mWaitSceneID = nextId;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
	mFader->SetFade(Fader::STATE::FADE_OUT);
	mIsSceneChanging = true;

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

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// ���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	// �V�[����ύX����
	mSceneID = sceneId;

	// ���݂̃V�[�������
	if (mScene != nullptr)
	{
		mScene->Release();
		delete mScene;
	}

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		mScene = new TitleScene();
		break;
	case SCENE_ID::GAME:
		mScene = new GameScene();
		break;
	case SCENE_ID::GAMEOVER:
		mScene = new GameOverScene();
		break;
	}

	mScene->Init();

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
			DoChangeScene(mWaitSceneID);
			mFader->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


