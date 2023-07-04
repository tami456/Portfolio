#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Stage.h"
#include "../Object/SpaceDome.h"
#include "../Object/PlayerShip.h"
#include "../Object/RockManager.h"
#include "../Object/Common/SpeechBalloon.h"
#include "../Object/Ship/PlayerShot.h"
#include "../Object/Ship/TurretShot.h"
#include "../Object/Ship/Turret.h"
#include "GameScene.h"

GameScene::GameScene(void)
{

}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	

	mPlayerShip = new PlayerShip();
	mPlayerShip->Init();

	mSpaceDome = new SpaceDome(&mPlayerShip->GetTransform());
	mSpaceDome->Init();
	
	


	mStage = new Stage();
	mStage->Init();

	//�J�������[�h(�t���[�J����)
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	//�J�������[�h(�Ǐ]�J����)
	SceneManager::GetInstance().GetCamera()->SetFollowTarget(&mPlayerShip->GetTransform());
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);

	mRockManager = new RockManager(&mPlayerShip->GetTransform());
	mRockManager->Init();

	mStepShipdestroy = 0.0f;

}

void GameScene::Update(void)
{
	// �V�[���J��
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}
	
	mStage->Update();
	mSpaceDome->Update();
	mPlayerShip->Update();
	mRockManager->Update();

	if (mPlayerShip->IsDestroy())
	{
		//���@���j�󂳂�Ă���
		mStepShipdestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipdestroy > TIME_RESTART)
		{
			//�V�[���J��
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
		}
	}
	else
	{
		//���@�ƃ_���W�����̏Փ˔���
		auto info = MV1CollCheck_Sphere(
			mStage->GetModelDungeon(), -1,
			mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);

		//if����=���Ȃ���������
		if (info.HitNum > 0)
		{
			
			//�Փ˂��Ă���̂Ŏ��@��j��
			mPlayerShip->Destroy();

			
		}

		//�����蔻��p�̌��ʏ��̒��œ��I�Ƀ������m�ۂ��Ă���悤�Ȃ̂ŁA
		//�g���I�������A�K���������̔j���E��n�������邱��
		MV1CollResultPolyDimTerminate(info);
	}

	//���K�F�{�X��͂̎���5000�̋����ɓ�������A�C�x���g�V�[���ɑJ�ڂ���悤��
	//PG���쐬���Ă�������
	
	if (AsoUtility::SqrMagnitude(mStage->GetPosCargoShip(), mPlayerShip->GetTransform().pos) < 5000 * 5000)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}

	//�e���X�V
	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Update();
	}

	

}

void GameScene::Draw(void)
{
	//�w�i�E�s�����Ȃ���
	mSpaceDome->Draw();
	
	//�I�u�W�F�N�g(�����E��������)
	mStage->Draw();
	mRockManager->Draw();
	mPlayerShip->Draw();

	//�e���X�V
	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}

	//�X�N���[���G�t�F�N�g
	
	//UI(2D)�֘A
	mPlayerShip->GetSpeechBalloon()->Draw();

	//�X�N���[���G�t�F�N�g(UI����)
}

void GameScene::Release(void)
{
	mStage->Release();
	delete mStage;

	mSpaceDome->Release();
	delete mSpaceDome;
	
	mPlayerShip->Release();
	delete mPlayerShip;
	
	mRockManager->Release();
	delete mRockManager;

}
