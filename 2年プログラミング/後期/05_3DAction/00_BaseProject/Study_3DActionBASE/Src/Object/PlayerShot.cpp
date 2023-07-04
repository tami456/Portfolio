#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Quaternion.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "Common/Transform.h"
#include "Common/AnimationController.h"
#include "PlayerShot.h"

PlayerShot::PlayerShot(const Transform* parent)
{
    mIsAlive = false;

    mCollisionRadius = 10.0f;

	// ���f������̊�{���
	ResourceManager& rem = ResourceManager::GetInstance();
	mTransform.modelId = rem.LoadModelDuplicate(ResourceManager::SRC::SHOT);
	float scale = 0.2f;
	mTransform.scl = { scale, scale, scale };
	mTransform.pos = parent->pos;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
	mTransform.Update();

	mStepAlive = 0.0f;
	mDir = { 0.0f, 0.0f, 0.0f };
}

PlayerShot::~PlayerShot()
{
}

void PlayerShot::Init(void)
{
}

void PlayerShot::Create(VECTOR birthPos, VECTOR dir)
{
	mDir = dir;

	mTransform.pos = birthPos;
	mTransform.quaRot = Quaternion::LookRotation(mDir);

	//�����n������
	mIsAlive = true;
	mStepAlive = GetTimeAlive();
}

void PlayerShot::Update(void)
{

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

bool PlayerShot::CheckAlive(void)
{
	// ��������
	mStepAlive -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepAlive < 0.0f)
	{
		mIsAlive = false;
	}

	return mIsAlive;
}

void PlayerShot::Move(void)
{
	// �ړ�
	VECTOR velocity = VScale(mDir, GetSpeed());
	mTransform.pos = VAdd(mTransform.pos, velocity);
}

void PlayerShot::Draw(void)
{
	if (mIsAlive)
	{
		MV1DrawModel(mTransform.modelId);
	}
}

void PlayerShot::Release(void)
{
}

void PlayerShot::NonAlive(void)
{
	mIsAlive = false;
}

float PlayerShot::GetSpeed(void) const
{
	return DEFAULT_SPEED;
}

float PlayerShot::GetTimeAlive(void) const
{
    return DEFAULT_TIME_ALIVE;
}

bool PlayerShot::IsAlive(void) const
{
    return mIsAlive;
}

Transform& PlayerShot::GetTransform(void)
{
    return mTransform;
}

float PlayerShot::GetCollisionRadius(void) const
{
    return mCollisionRadius;
}
