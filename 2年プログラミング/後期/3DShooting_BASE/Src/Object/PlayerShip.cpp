#include <string>
#include <EffekseerForDxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "ParticleGenerator.h"
#include "./Common/SpriteAnimator.h"
#include "./Common/SpeechBalloon.h"
#include "./Ship/PlayerShot.h"
#include "PlayerShip.h"


PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	//���f������̊�{���
	mTransform.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::PLAYER_SHIP)
	);
	mTransform.scl = { 10.0f,10.0f,10.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion();
	mTransform.pos = { 0.0f,0.0f,0.0f };
	//�ύX������Update
	mTransform.Update();

	//�p�[�e�B�N���G�t�F�N�g
	mParticleGenerator = new ParticleGenerator(mTransform.pos,10.0f);
	mParticleGenerator->Init();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�����o��
	std::string msg = "�ǂ��āI";
	//�V�[���ɂ���ď�Ԃ�ς���
	SceneManager::SCENE_ID sceneId = SceneManager::GetInstance().GetmSceneID();

	if (sceneId == SceneManager::SCENE_ID::EVENT)
	{
		msg = "�|���āI";
	}
	mSpeechBalloon = new SpeechBalloon(
	SpeechBalloon::TYPE::SPEECH,&mTransform);

	mSpeechBalloon->SetText(msg.c_str());
	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({15.0f,25.0f,0.0f});

	//������Ԃ𑖍s��Ԃɂ���
	ChangeState(STATE::RUN);

	//�u�[�X�g�֘A
	mStepDelayBoost = 0.0f;
	mSpeedBoost = 0.0f;
	mEffectBoostPlay = -1;
}

void PlayerShip::InitEffect(void)
{
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION,
		20.0f, 8.0f);

	mExplosion3D = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EXPLOSION).mHandleId;
	mExplosion3DPlay = -1;

	//���˃G�t�F�N�g
	mEffectJet = ResourceManager::GetInstance().Load(ResourceManager::SRC::JET).mHandleId;
	//Effekseer�̃G�t�F�N�g����
	mEffectJetPlayL = PlayEffekseer3DEffect(mEffectJet);
	mEffectJetPlayR = PlayEffekseer3DEffect(mEffectJet);

	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayL, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayR, SCALE, SCALE, SCALE);

	//�u�[�X�g�G�t�F�N�g
	mEffectBoost = ResourceManager::GetInstance().Load(ResourceManager::SRC::BOOST).mHandleId;
	mEffectBoostPlay = PlayEffekseer3DEffect(mEffectBoost);

	float SCALEB = 10.0f;
	SetScalePlayingEffekseer3DEffect(mEffectBoostPlay, SCALEB, SCALEB, SCALEB);

	//�G�t�F�N�g����
	SyncJetEffect();
	SyncBoostEffect();
}

void PlayerShip::Update(void)
{
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		UpdateRun();
		break;
	case PlayerShip::STATE::DESTROY:
		UpdateDestroy();
		break;
	}
}

void PlayerShip::UpdateRun(void)
{
	//��]����
	ProcessTurn();

	//��������
	ProcessBoost();

	//�e�̏���
	ProcessShot();

	VECTOR forward = mTransform.GetForward();
	mTransform.pos = VAdd(mTransform.pos, VScale(forward, SPEED_MOVE + mSpeedBoost));
	mTransform.Update();

	//�v���C���[�̈ʒu�ƃG�t�F�N�g�̈ʒu�𓯊�

	//�v���C���[�̈ʒu�̑O�������ɉ~���o��
	VECTOR forwardPos = VAdd(mTransform.pos, VScale(mTransform.GetForward(), 18.0f));
	mParticleGenerator->SetPos(forwardPos);

	//�v���C���[�̌�����������悤��
	Quaternion pBackRot = mTransform.quaRot;
	pBackRot = pBackRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadD(-90), AsoUtility::AXIS_X));
	mParticleGenerator->SetRot(pBackRot);
	mParticleGenerator->Update();

	SyncJetEffect();

	if (CheckHitKey(KEY_INPUT_B))
	{
		SyncBoostEffect();
	}
}
	

void PlayerShip::UpdateDestroy(void)
{
	//A:���@���j�󂳂ꂽ��J�������~�߂�
	
	//B:�����G�t�F�N�g�����@�̎�O�ɕ`�悷��悤�ɂ���
	VECTOR forwardPos = VAdd(mTransform.pos, VScale(mTransform.GetForward(), 150.0f));

	//�����G�t�F�N�g
	mExplosion->SetPos(forwardPos);
	mExplosion->Update();
}

void PlayerShip::Draw(void)
{
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		//���@
		MV1DrawModel(mTransform.modelId);
		//�p�[�e�B�N��
		mParticleGenerator->Draw();
		break;
	case PlayerShip::STATE::DESTROY:
		//�����G�t�F�N�g
		mExplosion->Draw();
		break;
	}
	
	//���@�̓����蔻��
	//DrawSphere3D(mTransform.pos, COLLISION_RADIUS, 5, 0xffffff, 0xffffff,false);

}

void PlayerShip::Release(void)
{
	mParticleGenerator->Release();
	delete mParticleGenerator;

	mExplosion->Release();
	delete mExplosion;

	if (mExplosion3DPlay != -1)
	{
		StopEffekseer3DEffect(mExplosion3DPlay);
	}

	StopEffekseer3DEffect(mEffectJetPlayL);
	StopEffekseer3DEffect(mEffectJetPlayR);

	StopEffekseer3DEffect(mEffectBoostPlay);

	mSpeechBalloon->Release();
	delete mSpeechBalloon;

	//�e�̊J��
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();
}

void PlayerShip::ProcessTurn(void)
{
	//�E����
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		Turn(SPEED_ROT_Y,AsoUtility::AXIS_Y);
	}

	//������
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_Y, AsoUtility::AXIS_Y);
	}

	//�����
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_X, AsoUtility::AXIS_X);
	}

	//������
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_X, AsoUtility::AXIS_X);
	}

}

void PlayerShip::ProcessBoost(void)
{
	mStepDelayBoost += SceneManager::GetInstance().GetDeltaTime();
	if (CheckHitKey(KEY_INPUT_B) && mStepDelayBoost > TIME_DELAY_BOOST)
	{
		//�f�B���C(�N�[���^�C��)����
		mStepDelayBoost = 0.0f;

		//��������
		mSpeedBoost = SPEED_BOOST;

		//�G�t�F�N�g�Đ�
		mEffectBoostPlay = PlayEffekseer3DEffect(mEffectBoost);

		//�傫��
		float SCALE = 10.0f;
		SetScalePlayingEffekseer3DEffect(mEffectBoostPlay, SCALE, SCALE, SCALE);

		//�G�t�F�N�g����
		SyncBoostEffect();

	}
	if (mStepDelayBoost < TIME_DELAY_BOOST && mEffectBoostPlay > 0)
	{
		//�G�t�F�N�g����
		SyncBoostEffect();
	}
	//mStepDelayBoost 0.0 = �G�t�F�N�g������_��	= �G�t�F�N�g��		= ������������
	//mStepDelayBoost 3.0 = �G�t�F�N�g������_��	= �G�t�F�N�g��		= ������������
	//mStepDelayBoost 3.1 = �G�t�F�N�g���Ă悢		= �G�t�F�N�g���I��	= �����Ӗ��Ȃ�


	//��������
	if (mSpeedBoost > 0.0f)
	{
		//���X�Ɍ���
		mSpeedBoost -= (SUB_SPEED_BOOST * SceneManager::GetInstance().GetDeltaTime());
		if (mSpeedBoost < 0.0f)
		{
			//�ʏ푖�s����������Ȃ��悤�[���ɂ��Ă���
			mSpeedBoost = 0.0f;
		}

		SyncBoostEffect();

	}

}

void PlayerShip::ProcessShot(void)
{
	//�e�̐����f�B���C
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}

	//�L�[�`�F�b�N
	if (CheckHitKey(KEY_INPUT_N) && mStepDelayShot <= 0.0f)
	{
		//�f�B���C���Ԃ��Z�b�g
		mStepDelayShot = TIME_DELAY_SHOT;

		//�e�𐶐�
		CreateShot();
	}
}

void PlayerShip::Turn(double deg, VECTOR axis)
{

	////�f�O���[�p�����W�A���p�ɕϊ�
	//float rad = AsoUtility::Deg2RadF(deg);

	////�w�莲(axis)�ɑ΂����]��(rad)����������](Quaternion)�����
	//Quaternion axisQ = Quaternion::AngleAxis(rad, axis);

	////���@�̉�]�ɁA���ō쐬������]��������(��������)
	//mTransform.quaRot = mTransform.quaRot.Mult(axisQ);

	mTransform.quaRot = mTransform.quaRot.Mult(VScale(axis,deg));
}

const Transform& PlayerShip::GetTransform(void) const
{
	return mTransform;
}

void PlayerShip::Destroy(void)
{

	ChangeState(STATE::DESTROY);

}

bool PlayerShip::IsDestroy(void)
{
	return mState == STATE::DESTROY;
}

SpeechBalloon* PlayerShip::GetSpeechBalloon(void) const
{
	return mSpeechBalloon;
}

std::vector<PlayerShot*>& PlayerShip::GetShots(void)
{
	return mShots;
}

void PlayerShip::ChangeState(STATE state)
{
	//��ԕω�(�J��)���̏�������
	mState = state;
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		break;
	case PlayerShip::STATE::DESTROY:
	{
		//�����G�t�F�N�g�̐���(�J�n)
		//mExplosion->Create(mTransform.pos, nullptr);
		//Effekseer�̃G�t�F�N�g����
		mExplosion3DPlay = PlayEffekseer3DEffect(mExplosion3D);
		VECTOR pos = mTransform.pos;
		pos = VAdd(pos, VScale(mTransform.GetForward(), 100.0f));
		//�ʒu
		SetPosPlayingEffekseer3DEffect(mExplosion3DPlay, pos.x, pos.y, pos.z);
		//�傫��
		SetScalePlayingEffekseer3DEffect(mExplosion3DPlay, 10.0f,10.0f,10.0f);
		//��]
		SetRotationPlayingEffekseer3DEffect(mExplosion3DPlay, 0.0f,0.0f,0.0f);
	}
		break;
	}
}

void PlayerShip::SyncJetEffect(void)
{

	VECTOR pos;

	const VECTOR angles = Quaternion::ToEuler(mTransform.quaRot);
	
#pragma region �����˃G�t�F�N�g
	//���΍��W����]������
	VECTOR localLPos = { -5.0f,0.0f,-15.0f };
	//���΍��W����]������
	localLPos = mTransform.quaRot.PosAxis(localLPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform.pos, localLPos);
	//�G�t�F�N�g�̈ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayL, pos.x, pos.y, pos.z);
#pragma endregion


#pragma region �E���˃G�t�F�N�g

	//���΍��W����]������
	VECTOR localRPos = { 5.0f,0.0f,-15.0f };
	//���΍��W����]������
	localRPos = mTransform.quaRot.PosAxis(localRPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform.pos, localRPos);
	//�G�t�F�N�g�̈ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayR, pos.x, pos.y, pos.z);
#pragma endregion

	//��]
	SetRotationPlayingEffekseer3DEffect(mEffectJetPlayL, angles.x, angles.y, angles.z);
	SetRotationPlayingEffekseer3DEffect(mEffectJetPlayR, angles.x, angles.y, angles.z);

}

void PlayerShip::SyncBoostEffect(void)
{

#pragma region ��]����
	//�N�H�[�^�j�I������I�C���[�p�ɕϊ�
	const VECTOR angles = Quaternion::ToEuler(mTransform.quaRot);

	//��]
	SetRotationPlayingEffekseer3DEffect(mEffectBoostPlay, angles.x, angles.y, angles.z);
#pragma endregion

#pragma region �ʒu����
	VECTOR pos;

	//���΍��W����]������
	VECTOR localBPos = { 0.0f,0.0f,100.0f };
	//���΍��W����]������
	localBPos = mTransform.quaRot.PosAxis(localBPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform.pos, localBPos);
	//�G�t�F�N�g�̈ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEffectBoostPlay, pos.x, pos.y, pos.z);
#pragma endregion

}

void PlayerShip::CreateShot(void)
{
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�������
			//�C���X�^���X���g���܂킷
			s->Create(mTransform.pos,mTransform.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//�C���X�^���X��V�������
		auto newShot = new PlayerShot(&mTransform);
		newShot->Create(mTransform.pos, mTransform.GetForward());

		//�ϒ��z��ɒǉ�
		mShots.emplace_back(newShot);
	}

}

//����
	//const VECTOR angles = mTransform.quaRot.ToEuler();
	//����������W�����
	//VECTOR localPos = VScale(mTransform.GetBack(), 15.0f);
	//pos = VAdd(mTransform.pos, localPos);
	//VECTOR localLeftPos = VScale(mTransform.GetBack(), 5.0f);
	//localLeftPos = VAdd(localLeftPos,VScale(mTransform.GetLeft(), 5.0f));
	//pos = VAdd(mTransform.pos, localLeftPos);
	////�ʒu
	//SetPosPlayingEffekseer3DEffect(mEffectJetPlayL, -pos.x, pos.y, pos.z);
	//SetPosPlayingEffekseer3DEffect(mEffectJetPlayR, pos.x, pos.y, pos.z);