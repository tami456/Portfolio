#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Common/Quaternion.h"
#include "../../Utility//AsoUtility.h"
#include "../Common/Transform.h"
#include "../Common/SpriteAnimator.h"
#include "./TurretShot.h"
#include "Turret.h"

Turret::Turret(
	const Transform* transformParent, 
	VECTOR localPos, VECTOR localAddAxis)
{
	//�e
	mTransformParent = transformParent;

	//���΍��W
	mLocalPos = localPos;

	//���Ή�]
	mLocalAddAxis = localAddAxis;

	//�C��
	mTransformStand.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::TURRET_STAND));

	//�C�g
	mTransformGun.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TURRET_GUN));

	//�C��ƖC�g�̂��ꂼ��̈ړ���]
	mLocalAddAxisStand	= VECTOR();
	mLocalAddAxisGun	= VECTOR();

	//�������
	ChangeState(STATE::ATTACK);
}

Turret::~Turret(void)
{
}

void Turret::Init(void)
{
	//���f������̊�{���(�C��)
	mTransformStand.scl = {SCALE,SCALE,SCALE};
	mTransformStand.quaRot = Quaternion();

	//���[�J����]
	mTransformStand.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//�e���o��Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformStand, {0.0f,0.0f,0.0f});

	//���f������̊�{���(�C�g)
	mTransformGun.scl = { SCALE,SCALE,SCALE };
	mTransformGun.quaRot = Quaternion();

	//���[�J����]
	mTransformGun.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//�e���o��Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformGun, { 0.0f,0.0f,0.0f });

	//�ϋv��
	mHp = 2;

	//�C�䃍�[�J���ғ���(rad)
	mAnglePowStand = ANGLE_POW_STAND;

	//�C�g���[�J���ғ���(rad)
	mAnglePowGun = ANGLE_POW_GUN;

	//�����G�t�F�N�g
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION, 120.0f, 8.0f);

	mStepDamaged = -1;


}

void Turret::Update(void)
{
	switch (mState)
	{
	case Turret::STATE::ATTACK:
		UpdateAttack();
		break;
	case Turret::STATE::DESTROY:
		UpdateDestroy();
		break;
	}
}

void Turret::UpdateAttack(void)
{
	auto delta = SceneManager::GetInstance().GetDeltaTime();

	//�C��̉ғ���](rad)
	mLocalAddAxisStand.y += mAnglePowStand * delta;

	//�C��̉��͈�
	if (mLocalAddAxisStand.y < ANGLE_Y_MIN_STAND || mLocalAddAxisStand.y > ANGLE_Y_MAX_STAND)
	{
		mAnglePowStand *= -1.0f;
	}

	//�C���e���f���Ɠ���������
	SyncParent(mTransformStand,mLocalAddAxisStand);

	//�C�g�̉ғ���](rad)
	mLocalAddAxisGun.x += mAnglePowGun * delta;

	//�C�g�̉ғ��͈�
	if (mLocalAddAxisGun.x < ANGLE_Y_MIN_GUN || mLocalAddAxisGun.x > ANGLE_Y_MAX_GUN)
	{
		mAnglePowGun *= -1.0f;
	}

	//�C�䂪��]�������A�C�g����]����K�v������̂Őe�q��]
	Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
	Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
	Quaternion mix = stand.Mult(gun);

	//�C�g��e���f���Ɠ���������
	SyncParent(mTransformGun, mix.ToEuler());

	//�����̓^���b�g�̒e
	//�@�C�g6���e�����˂����
	//�A�C�g�̐悠���肩��e�����˂����(�ʒu����)
	//�B�e��ShotBase���p������(���Ȃ��Ă��ǂ�)�Z
	//�C�N�[���^�C����݂���Z
	//�D���@�Ƃ̓����蔻���݂���
	//�E���@�ƒe���Փ˂�����A�o�g���V�[���̎n�߂���
	
	

	//�e�̐����f�B���C
	mStepDelayShot -= delta;

	//�L�[�`�F�b�N
	if (mStepDelayShot < 0.0f)
	{
		//�f�B���C���Ԃ��Z�b�g
		mStepDelayShot = TIME_DELAY_SHOT;

		VECTOR localPos = Quaternion::PosAxis(mTransformGun.quaRot, { 0.0f,2.0f,3.0f });
		VECTOR birthPos = VAdd(mTransformGun.pos,VScale(localPos,SCALE));

		//�e�𐶐�
		CreateShot(birthPos,mTransformGun.GetForward());
	}

	

	if (mStepDamaged > 0.0f)
	{
		mStepDamaged -= delta;
	}

	//�e�̍X�V
	UpdateShot();
}

void Turret::UpdateDestroy(void)
{

	//�e�̍X�V
	UpdateShot();

	//�����G�t�F�N�g�̍X�V
	mExplosion->Update();

}

void Turret::UpdateShot(void)
{
	//�e�̍X�V
	for (const auto s : mShots)
	{
		s->Update();
	}
}

void Turret::Draw(void)
{
	switch (mState)
	{
	case Turret::STATE::ATTACK:
		DrawAttack();
		break;
	case Turret::STATE::DESTROY:
		DrawDestroy();
		break;
	}
}

void Turret::DrawAttack(void)
{
	if (mStepDamaged > 0.0f)
	{
		MV1SetMaterialDifColor(mTransformStand.modelId, 
			0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
		MV1SetMaterialDifColor(mTransformGun.modelId, 
			0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
	}
	else
	{
		MV1SetMaterialDifColor(mTransformStand.modelId, 
			0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
		MV1SetMaterialDifColor(mTransformGun.modelId, 
			0, GetColorF(0.2f, 0.4f, 0.3f, 1.0f));
	}

	MV1DrawModel(mTransformStand.modelId);
	MV1DrawModel(mTransformGun.modelId);

	//�e�̕`��
	DrawShot();
}

void Turret::DrawDestroy(void)
{
	//�e�̕`��
	DrawShot();

	//�����G�t�F�N�g
	mExplosion->Draw();
}

void Turret::DrawShot(void)
{
	//�e�̕`��
	for (const auto s : mShots)
	{
		s->Draw();
	}
}

void Turret::Release(void)
{
	//�e�̊J��
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();

	mExplosion->Release();
	delete mExplosion;
}

void Turret::SyncParent(Transform& transform, VECTOR addAxis)
{

	//�{�X��͂�3D�����擾
	Quaternion parentRot = mTransformParent->quaRot;

	//�C��E�C�g�̃��[�J����]
	//Unity�@Z��X��Y = Y * X * Z
	Quaternion localRot;
	Quaternion axis;

	//Y
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	//X
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//Z
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);


	//�����ғ����̉�]������ɉ�����
	//------------------------------------------
	//Y
	axis = Quaternion::AngleAxis(
		addAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	//X
	axis = Quaternion::AngleAxis(
		addAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//Z
	axis = Quaternion::AngleAxis(
		addAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);
	//------------------------------------------

	//�e�̉�]�ɉ����āA�C��E�C�g����]������
	transform.quaRot = parentRot.Mult(localRot);

	//�e�̉�]���ɍ��킹���ʒu���v�Z
	VECTOR localPos = Quaternion::PosAxis(parentRot,mLocalPos);
	//�����܂ŏK��������
	////���΍��W���΍��W�ɕϊ�
	//transform.pos = VAdd(
	//mTransformParent->pos, localPos);

	//���f���̑傫�����A�X�P�[���������΍��W���΍��W�ɕϊ�
	transform.pos = VAdd(
		mTransformParent->pos, VScale(localPos, SCALE));

	//���f������̊�{���X�V
	transform.Update();

}

std::vector<TurretShot*>& Turret::GetShots(void)
{
	return mShots;
}

bool Turret::IsAlive(void) const
{
	return mState == STATE::ATTACK;
}

VECTOR Turret::GetPos(void) const
{
	return mTransformStand.pos;
}

void Turret::Damage(void)
{

	mStepDamaged = TIME_DAMAGED_EFFECT;
	mHp -= 1;
	if (mHp <= 0)
	{
		ChangeState(STATE::DESTROY);
	}
		
}

void Turret::ChangeState(STATE state)
{
	//��Ԃ��X�V����
	mState = state;

	//��ԑJ�ڎ��̏�����
	switch (mState)
	{
	case Turret::STATE::ATTACK:
		break;
	case Turret::STATE::DESTROY:
	{
		//�����G�t�F�N�g
		mExplosion->Create(mTransformGun.pos, nullptr);
	}
		break;
	}
}

void Turret::CreateShot(VECTOR birthPos,VECTOR dir)
{
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�������
			//�C���X�^���X���g���܂킷
			s->Create(birthPos, dir);
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//�C���X�^���X��V�������
		auto newShot = new TurretShot(&mTransformGun);
		newShot->Create(birthPos, dir);

		//�ϒ��z��ɒǉ�
		mShots.emplace_back(newShot);
	}
}




