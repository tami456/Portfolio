#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/RockManager.h"
#include "../Object/SpaceDome.h"
#include "../Object/Ship/BossShip.h"
#include "../Object/PlayerShip.h"
#include "../Object/Ship/Turret.h"
#include "../Object/Common/SpeechBalloon.h"
#include "../Object/Ship/PlayerShot.h"
#include "../Object/Ship/TurretShot.h"
#include "../Utility/AsoUtility.h"
#include "BattleScene.h"

BattleScene::BattleScene(void)
{
}

BattleScene::~BattleScene(void)
{
}

void BattleScene::Init(void)
{
	mBossShip = new BossShip();
	mBossShip->Init();

	mPlayerShip = new PlayerShip();
	mPlayerShip->Init();

	const auto& trans = mPlayerShip->GetTransform();

	//�w�i��
	mRockManager = new RockManager(&trans);
	mRockManager->Init();

	//�w�i(�X�J�C�h�[��)
	mSpaceDome = new SpaceDome(&trans);
	mSpaceDome->Init();

	//�J�������[�h��Ǐ]�^�ɂ���
	auto& ins = SceneManager::GetInstance();
	ins.GetCamera()->SetFollowTarget(&trans);
	ins.GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);

	//���@�̔j�󉉏o���Ԃ̏�����
	mStepShipDestroy = 0.0f;

	//�Q�[���N���A��̃��S
	mImgEndLogo = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::END_LOGO).mHandleId;

	//�{�X�̔j�󉉏o���Ԃ̏�����
	mStepBossDestroy = TIME_RESTART_BOSS_DESTROY;


}

void BattleScene::Update(void)
{
	mBossShip->Update();
	mPlayerShip->Update();
	mRockManager->Update();
	mSpaceDome->Update();
	
	//���@�ƃ{�X�Ƃ̓����蔻��
	if (mPlayerShip->IsDestroy())
	{
		//���@���j�󂳂�Ă���
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy > TIME_RESTART)
		{
			//�V�[���J��
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::BATTLE, true);
		}
	}
	else
	{
		//�{�X�̐�������
		if (mBossShip->IsAlive())
		{
			//���@�ƃ_���W�����̏Փ˔���
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
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

			//�{�X�̒e
			auto turrets = mBossShip->GetTurrets();
			for (const auto t : turrets)
				{
				auto ts = t->GetShots();
				for (auto s : ts)
				{
					if (!s->IsAlive())
					{
						continue;
					}

					if (AsoUtility::IsHitSpheres(
						mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS,
						s->GetPos(), s->GetCollisionRadius()))
					{
						//�Փ˂��Ă���̂Ŏ��@��j��
						mPlayerShip->Destroy();
					}
				}
				
			}
		}

	}
	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{

		s->Update();

		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�
			continue;
		}

		if (mBossShip->IsAlive())
		{
			//�e�ƃ{�X�̏Փ˔���
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				s->GetPos() , s->GetCollisionRadius());

			if (info.HitNum > 0)
			{
				//�Փ˂��Ă�����A�e��j�󂷂�
				s->CreateExplosion(&mBossShip->GetTransform());
				continue;
			}

			//�����蔻��p�̌��ʏ��̒��œ��I�Ƀ������m�ۂ��Ă���悤�Ȃ̂ŁA
			//�g���I�������A�K���������̔j���E��n�������邱��
			MV1CollResultPolyDimTerminate(info);
		}

		//���@�̒e�ƃ^���b�g�̏Փ˔���(���̂Ƌ���)
		auto turrets = mBossShip->GetTurrets();
		for (auto t : turrets)
		{
			if (!t->IsAlive())
			{
				continue;
			}

			//���@�̒e�ƃ^���b�g�̏Փ˔���
			/*auto info = MV1CollCheck_Sphere(
				t->GetPos(), t->COLLISION_RADIUS,
				s->GetPos(), s->GetCollisionRadius());*/

			if (AsoUtility::IsHitSpheres(
				s->GetPos(), s->GetCollisionRadius(),
				t->GetPos(), Turret::COLLISION_RADIUS))
			{
				//�^���b�g�Ƀ_���[�W
				t->Damage();

				//�e���ђʂ����Ȃ�
				s->CreateExplosion(nullptr);
				break;
			}


		}

	}

	//�{�X�̔j�󔻒�
	if (mBossShip->IsEnd())
	{
		mStepBossDestroy -= SceneManager::GetInstance().GetDeltaTime();
		if (mStepBossDestroy < 0.0)
		{
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::TITLE, true
			);
		}
	}
	if (CheckHitKey(KEY_INPUT_L))
	{
		SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	}
}

void BattleScene::Draw(void)
{
	//�s����(�w�i)
	mRockManager->Draw();
	mSpaceDome->Draw();

	//�s����(�I�u�W�F�N�g)
	mBossShip->Draw();
	mPlayerShip->Draw();

	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}

	mPlayerShip->GetSpeechBalloon()->Draw();	

	if (mBossShip->IsDestroy() || mBossShip->IsEnd())
	{
		//�N���A���S
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0, mImgEndLogo, true);
	}
}

void BattleScene::Release(void)
{
	mBossShip->Release();
	delete mBossShip;
	mPlayerShip->Release();
	delete mPlayerShip;
	mRockManager->Release();
	delete mRockManager;
	mSpaceDome->Release();
	delete mSpaceDome;
}
