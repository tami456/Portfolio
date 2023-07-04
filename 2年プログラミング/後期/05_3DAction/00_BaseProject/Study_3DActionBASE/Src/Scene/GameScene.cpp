#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/GravityManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/SpeechBalloon.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/PlayerShot.h"
#include "../Object/Enemy.h"
#include "../Object/Mascot.h"
#include "../Object/Planet.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	mPlayer = nullptr;
	mEnemy = nullptr;
	mMascot = nullptr;
	mSkyDome = nullptr;
	mStage = nullptr;
}

void GameScene::Init(void)
{

	mPlayer = new Player();
	GravityManager::GetInstance().SetPlayer(mPlayer);
	mPlayer->Init();
	
	mEnemy = new Enemy(mPlayer);
	GravityManager::GetInstance().SetEnemy(mEnemy);
	mEnemy->Init();

	mMascot = new Mascot();
	mMascot->Init();

	std::string msg = "";

	auto name = GravityManager::GetInstance().GetActivePlanet()->GetName();

	if (mEnemy->IsDead())
	{
		msg = "���Ɍ��������I";
	}
	else if (!mEnemy->IsDead())
	{
		msg = "�G��|�����I";
	}

	mPlayer->GetSpeechBalloon()->SetText(msg.c_str());

	// �X�J�C�h�[��
	mSkyDome = new SkyDome(mPlayer->GetTransform());
	mSkyDome->Init();

	mStage = new Stage(mPlayer, mEnemy);
	mStage->Init();

	// �X�e�[�W�̐ݒ�
	mStage->ChangeStage(Stage::NAME::MAIN_PLANET);

	SceneManager::GetInstance().GetCamera()->SetTransform(mPlayer->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);


}

void GameScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (mPlayer->IsDead())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	mSkyDome->Update();

	mStage->Update();

	mPlayer->Update();

	mEnemy->Update();

	mMascot->Update();

	

	//�e���X�V
	auto& shots = mPlayer->GetShots();
	for (const auto s : shots)
	{

		s->Update();

		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�
			continue;
		}

		if (mEnemy->IsAlive())
		{
			//�e�ƃ{�X�̏Փ˔���
			
			auto pos = mEnemy->GetTransform().pos;
			if (AsoUtility::IsHitSpheres(
				s->GetTransform().pos, s->GetCollisionRadius(),
				VGet(pos.x,pos.y + 80,pos.z), 100.0f))
			{
				//�Փ˂��Ă�����A�e��j�󂷂�
				s->NonAlive();
				mEnemy->Damage();
				break;
			}
		}
	}
}

void GameScene::Draw(void)
{

	mSkyDome->Draw();

	mStage->Draw();

	mPlayer->Draw();

	mEnemy->Draw();

	mMascot->Draw();

	mPlayer->GetSpeechBalloon()->Draw();

	//�e���X�V
	auto& shots = mPlayer->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}

	// �f�o�b�O�\��
	DrawDebug();

	// �w���v
	DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	DrawFormatString(840, 60, 0x000000, "�_�b�V���F��Shift");
	DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");
	DrawFormatString(840, 100, 0x000000, "�U��    �FE");

}

void GameScene::DrawDebug(void)
{
}

void GameScene::Release(void)
{

	mSkyDome->Release();
	delete mSkyDome;

	mStage->Release();
	delete mStage;

	mPlayer->Release();
	delete mPlayer;
	
	mEnemy->Release();
	delete mEnemy;

	mMascot->Release();
	delete mMascot;
}
