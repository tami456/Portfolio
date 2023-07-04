#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "Player.h"
#include "Common/Capsule.h"
#include "WarpStar.h"

WarpStar::WarpStar(Player* player, Transform transform)
{
	mPlayer = player;
	mTransform = transform;

	mState = STATE::NONE;
}

void WarpStar::Init(void)
{

	// ���f���̊�{���
	mTransform.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::WARP_STAR)
	);

	mTransform.Update();

	//Z�̖���]�̏�Ԃ�ێ����Ă���
	VECTOR angle = mTransform.quaRot.ToEuler();
	mWarpQua = Quaternion::Euler(angle.x, angle.y, 0.0f);

	//���[�v�����p�̈ړ����W
	mWarpReservePos = 
		VAdd(mTransform.pos,mWarpQua.
			PosAxis(WARP_RELATIVE_POS));
	
	//�G�t�F�N�g
	mEffectRotParticle = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::WARP_STAR_ROT_EFF).mHandleId;

	mStepEffect = TERM_EFFECT;

	ChangeState(STATE::IDLE);

	
}

void WarpStar::Update(void)
{

	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		UpdateIdle();
		break;
	case WarpStar::STATE::RESERVE:
		UpdateReserve();
		break;
	case WarpStar::STATE::MOVE:
		UpdateMove();
		break;
	}

}

//����֐����K�p
bool IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
	// ���̓��m�̏Փ˔���
	bool hit = false;

	// ���݂��̔��a�̍��v
	float radius = radius1 + radius2;

	// ���W�̍����炨�݂��̋��������
	VECTOR diff = VSub(pos1, pos2);

	// �O�����̒藝�Ŕ�r(SqrMagnitude�Ɠ���) a^2 + b^2 = c^2
	float distance = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	if (distance < (radius * radius))
	{
		hit = true;
	}

	return hit;
}

void WarpStar::UpdateIdle(void)
{

	//Z��]
	RotateZ(SPEED_ROT_IDLE);

	//�G�t�F�N�g�̔���
	PlayEffectRotParticle();

	//�v���C���[(�_)�ƃX�^�[(����)�̋��̂̓����蔻��
	
	//�X�^�[�̍��W
	VECTOR sPos = mTransform.pos;

	//�v���C���[�̍��W
	VECTOR pPos = mPlayer->GetCapsule()->GetCenter();
	float radius = mPlayer->GetCapsule()->GetRadius();

	//���̂̔��a
	//RADIUS

	if (IsHitSpheres(sPos, RADIUS, pPos, radius))
	{
		ChangeState(STATE::RESERVE);
	}

}

void WarpStar::UpdateReserve(void)
{

	//Z��]
	RotateZ(SPEED_ROT_RESERVE);

	//�G�t�F�N�g�̔���
	PlayEffectRotParticle();

	if (mPlayer->IsWarpMove())
	{
		ChangeState(STATE::MOVE);
	}

}

void WarpStar::UpdateMove(void)
{
	if(mPlayer->IsPlay())
	{
		ChangeState(STATE::IDLE);
	}
}

void WarpStar::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void WarpStar::Release(void)
{
}

Transform* WarpStar::GetTransform(void)
{
	return &mTransform;
}

void WarpStar::ChangeState(STATE state)
{
	//��ԕύX
	mState = state;

	//��ԑJ�ڂ�����Ƃ��̏���������

	mState = state;
	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		break;
	case WarpStar::STATE::RESERVE:
		//�v���C���[�����[�v�����i�K�Ɉڍs������
		mPlayer->StartWarp(
			TIME_WARP_RESERVE, mWarpQua, mWarpReservePos);
		break;
	case WarpStar::STATE::MOVE:
		break;
	}

}

void WarpStar::RotateZ(float speed)
{
	//���̃��f����Z��]������
	
	mTransform.quaRot = 
		mTransform.quaRot.Mult(VScale(AsoUtility::AXIS_Z,AsoUtility::Deg2RadF(speed)));
	//speed�F�x(�f�O���[)

	mTransform.Update();

}

void WarpStar::PlayEffectRotParticle(void)
{

	mStepEffect -= SceneManager::GetInstance().GetDeltaTime();

	if (mStepEffect < 0.0f)
	{
		mStepEffect = TERM_EFFECT;

		int mPlayEffectRotParticle;

		mPlayEffectRotParticle = PlayEffekseer3DEffect(mEffectRotParticle);

		float Scale = 5.0f;

		SetScalePlayingEffekseer3DEffect(mPlayEffectRotParticle, Scale, Scale, Scale);
		VECTOR StarEffect;
		StarEffect =
			VAdd(mTransform.pos, mTransform.quaRot.
				PosAxis(EFFECT_RELATIVE_POS));

		SetPosPlayingEffekseer3DEffect(
			mPlayEffectRotParticle,
			StarEffect.x, StarEffect.y, StarEffect.z
		);
	}
}
