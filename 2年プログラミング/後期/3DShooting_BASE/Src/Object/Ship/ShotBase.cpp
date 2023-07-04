#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Common/Quaternion.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../Common/Transform.h"
#include "../Common/SpriteAnimator.h"
#include "ShotBase.h"

ShotBase::ShotBase(const Transform* parent)
{

	// �������n
	mExplosion = new SpriteAnimator(ResourceManager::SRC::SHOT_EXPLOSION, 20.0f, 8.0f);

	mIsAlive = false;

	// �����ϓ�����������ΐ�pGetter��
	mCollisionRadius = 10.0f;

	// ���f������̊�{���
	ResourceManager& rem = ResourceManager::GetInstance();
	mTransform.modelId = rem.LoadModelDuplicate(ResourceManager::SRC::SHOT_MODEL);
	float scale = 0.2f;
	mTransform.scl = { scale, scale, scale };
	mTransform.pos = parent->pos;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
	mTransform.Update();

	mStepAlive = 0.0f;
	mDir = { 0.0f, 0.0f, 0.0f };

}

ShotBase::~ShotBase(void)
{
}

void ShotBase::Create(VECTOR birthPos, VECTOR dir)
{

	// �ė��p�\�Ȃ悤�ɂ���

	// �w������ɒe���΂�
	mDir = dir;

	mTransform.pos = birthPos;
	mTransform.quaRot = Quaternion::LookRotation(mDir);

	// �����n������
	mIsAlive = true;
	mStepAlive = GetTimeAlive();

}

void ShotBase::Update(void)
{

	// ���e���̃G�t�F�N�g
	mExplosion->Update();

	// �����`�F�b�N����������
	if (!CheckAlive())
	{
		return;
	}

	// �ړ�����
	Move();

	// ���f������̊�{���X�V
	mTransform.Update();

}

bool ShotBase::CheckAlive(void)
{

	// ��������
	mStepAlive -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepAlive < 0.0f)
	{
		mIsAlive = false;
	}

	return mIsAlive;

}

void ShotBase::Move(void)
{

	// �ړ�
	VECTOR velocity = VScale(mDir, GetSpeed());
	mTransform.pos = VAdd(mTransform.pos, velocity);

}

void ShotBase::Draw(void)
{

	mExplosion->Draw();

	if (!mIsAlive)
	{
		return;
	}

	if (!CheckCameraViewClip(mTransform.pos))
	{
		MV1DrawModel(mTransform.modelId);
	}

}

void ShotBase::Release(void)
{
	mExplosion->Release();
	delete mExplosion;
}

float ShotBase::GetSpeed(void) const
{
	return DEFAULT_SPEED;
}

float ShotBase::GetTimeAlive(void) const
{
	return DEFAULT_TIME_ALIVE;
}

bool ShotBase::IsAlive(void) const
{
	return mIsAlive;
}

void ShotBase::CreateExplosion(const Transform* transform)
{
	mIsAlive = false;
	// �ՓˑO�̈ʒu�ɂ��āA�ł��邾���\�����d�Ȃ�Ȃ��悤��
	VECTOR pos = VAdd(mTransform.pos, VScale(mTransform.GetBack(), 5.0f));
	mExplosion->Create(pos, transform);
}

VECTOR ShotBase::GetPos(void) const
{
	return mTransform.pos;
}

float ShotBase::GetCollisionRadius(void) const
{
	return mCollisionRadius;
}
