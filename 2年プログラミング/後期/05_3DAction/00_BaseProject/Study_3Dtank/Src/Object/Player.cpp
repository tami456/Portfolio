#include <string>
#include <EffekseerForDxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Planet.h"
#include "PlayerShot.h"
#include "Player.h"

Player::Player(void) : mResourceManager(ResourceManager::GetInstance()), mGravityManager(GravityManager::GetInstance())
{
	mTransformWheelL.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_WHEEL));

	mTransformWheelR.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_WHEEL));

	mTransformStand.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_BODY));

	mTransformGun.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_BARREL));
	
	mState = STATE::NONE;
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	// ���f���̊�{�ݒ�
	
	mTransformWheelL.scl = VGet(SCALE, SCALE, SCALE);
	mTransformWheelL.quaRot = Quaternion();
	mTransformWheelL.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//�e���o��Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformWheelL, { 0.0f,0.0f,0.0f });

	mTransformWheelR.scl = VGet(SCALE, SCALE, SCALE);
	mTransformWheelR.quaRot = Quaternion();
	mTransformWheelR.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//�e���o��Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformWheelR, { 0.0f,0.0f,0.0f });

	mTransformStand.scl = VGet(SCALE, SCALE, SCALE);
	mTransformStand.quaRot = Quaternion();
	mTransformStand.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//�e���o��Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformStand, { 0.0f,0.0f,0.0f });

	mTransformGun.scl = VGet(SCALE, SCALE, SCALE);
	mTransformGun.quaRot = Quaternion();
	mTransformGun.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//�e���o��Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformGun, { 0.0f,0.0f,0.0f });

	mTransformWheelL.pos = { -140.0f, -30.0f, 0.0f };
	mTransformWheelL.pos = { 140.0f, -30.0f, 0.0f };
	mTransformStand.pos = { 0.0f, 50.0f, 0.0f };
	mTransformGun.pos = { 0.0f,0.0f, -50.0f };

	mPlayerRotY = Quaternion();
	mGoalQuaRot = Quaternion();
	mStepRot = 0.0f;

	mGravHitDown = AsoUtility::VECTOR_ZERO;
	mGravHitUp = AsoUtility::VECTOR_ZERO;

	//�ۉe�摜
	mImgShadow = LoadGraph((Application::PATH_IMAGE + "Shadow.png").c_str());

	//�J�v�Z���R���C�_
	mCapsule = new Capsule(&mTransformWheelL);
	mCapsule = new Capsule(&mTransformStand);
	mCapsule = new Capsule(&mTransformGun);
	mCapsule->SetRelativePosTop({0.0f,110.0f,0.0f});
	mCapsule->SetRelativePosDown({0.0f,30.0f,0.0f});
	mCapsule->SetRadius(20.0f);

	mMoveDiff = AsoUtility::VECTOR_ZERO;


	mSlopeDir = AsoUtility::VECTOR_ZERO;
	mSlopeAngleDeg = 0.0f;
	mSlopePow = AsoUtility::VECTOR_ZERO;
	mHitNormal = AsoUtility::VECTOR_ZERO;
	mHitPos = AsoUtility::VECTOR_ZERO;

	//�������
	ChangeState(STATE::PLAY);
}


void Player::Update(void)
{

	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	case Player::STATE::DEAD:
		UpdateDead();
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

	
}

void Player::UpdatePlay(void)
{
	//�ړ�����(�ړ��ʂ����߂�)
	ProcessMove();
	
	Attack();

	//�X�΂̌v�Z
	CalcSlope();

	//�d�͂ɂ��ړ��ʂ��v�Z
	CalcGravityPow();

	//�Փ˔���
	Collision();

	auto name = mGravityManager.GetActivePlanet()->GetName();
	auto planetDead = mGravityManager.GetActivePlanet();
	if (mTransformWheelL.pos.y < -1000.0f || (planetDead->InRangeDead(mTransformWheelL.pos) && name != mPreWarpName))
	{
		ChangeState(STATE::DEAD);
	}

}

void Player::UpdateDead(void)
{
	//���ʂ̑����Ԃ̖߂�
	ChangeState(STATE::DEAD);
}

void Player::Draw(void)
{

	// ���f���̕`��
	MV1DrawModel(mTransformWheelL.modelId);
	MV1DrawModel(mTransformWheelR.modelId);
	MV1DrawModel(mTransformStand.modelId);
	MV1DrawModel(mTransformGun.modelId);

	

	// �f�o�b�O�p�`��
	//DrawDebug();

}

void Player::Release(void)
{

	delete mCapsule;
	DeleteGraph(mImgShadow);

	//�e�̊J��
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();
}

void Player::Attack(void)
{
	//�C��̉ғ���](rad)

	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		mLocalAddAxisStand.y += AsoUtility::Deg2RadF(1.0f);

	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		mLocalAddAxisStand.y -= AsoUtility::Deg2RadF(1.0f);
	}

	//�C���e���f���Ɠ���������
	SyncParent(mTransformStand, mLocalAddAxisStand);

	//�C�g�̉ғ���](rad)
	if (CheckHitKey(KEY_INPUT_UP))
	{
		mLocalAddAxisGun.x += AsoUtility::Deg2RadF(1.0f);
		if (mLocalAddAxisGun.x > ANGLE_Y_MAX_GUN)
		{
			mLocalAddAxisGun.x = ANGLE_Y_MAX_GUN;
		}

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		mLocalAddAxisGun.x -= AsoUtility::Deg2RadF(1.0f);
		if (mLocalAddAxisGun.x < ANGLE_Y_MIN_GUN)
		{
			mLocalAddAxisGun.x = ANGLE_Y_MIN_GUN;
		}
	}

	//�C�䂪��]�������A�C�g����]����K�v������̂Őe�q��]
	Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
	Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
	Quaternion mix = stand.Mult(gun);

	//�C�g��e���f���Ɠ���������
	SyncParent(mTransformGun, mix.ToEuler());
	SyncParent(mTransformWheelL, mix.ToEuler());
	SyncParent(mTransformWheelR, mix.ToEuler());
}

Transform* Player::GetTransform(void)
{
	return &mTransformWheelL;
}

Capsule* Player::GetCapsule(void)
{
	return mCapsule;
}

void Player::ProcessMove(void)
{

	//�ړ��ʂ�0
	mMovePow = AsoUtility::VECTOR_ZERO;

	//X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = mTransformStand.quaRot;

	//�ړ�����
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	if (CheckHitKey(KEY_INPUT_W))
	{
		//�ړ����������߂�
		dir = cameraRot.GetForward();
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		//�ړ����������߂�
		dir = cameraRot.GetBack();
	}

	//�ړ��L�[�����͂���Ă�����	  &&  �W�����v���[�V��������������
	if (!AsoUtility::EqualsVZero(dir))
	{

		mSpeed = SPEED_MOVE;

		//�ړ���������ړ��ʂ��Z�o����
		//�ړ��ʁ@= �����@�~�@�X�s�[�h
		mMoveDir = dir;
		mMovePow = VScale(dir, mSpeed);

		if (CheckHitKey(KEY_INPUT_E))
		{
			ProcessShot();
		}
	}
}

void Player::ProcessShot(void)
{
	//�e�̐����f�B���C
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}

	//�L�[�`�F�b�N
	if (CheckHitKey(KEY_INPUT_E) && mStepDelayShot <= 0.0f)
	{
		//�f�B���C���Ԃ��Z�b�g
		mStepDelayShot = TIME_DELAY_SHOT;

		//�e�𐶐�
		CreateShot();
	}
}

void Player::CalcSlope(void)
{
	//�d�͂̔��Ε���
	VECTOR gravityUp = mGravityManager.GetDirUpGravity();

	//�d�͂̔��Ε�������n�ʂ̖@�������Ɍ�������]��
	Quaternion up2NorQua = Quaternion::FromToRotation(gravityUp, mHitNormal);

	//�擾������]�̎��Ɗp�x���擾����
	float angle = 0.0f;
	VECTOR axis;
	up2NorQua.ToAngleAxis(&angle, &axis);

	//90�x�𑫂��āA�X�΃x�N�g���ւ̉�]���擾����
	Quaternion slopeQua = Quaternion::AngleAxis(
		angle + AsoUtility::Deg2RadF(90.0f), axis);

	//�n�ʂ̖@���������X�Ε����ɉ�]������
	mSlopeDir = slopeQua.PosAxis(gravityUp);

	//�X�΂̊p�x
	mSlopeAngleDeg = static_cast<float>(
		AsoUtility::AngleDeg(gravityUp, mSlopeDir));
}

void Player::CalcGravityPow(void)
{

	//�d�͕���
	VECTOR dirGravity = mGravityManager.GetDirGravity();

	//�d�͂̋���
	float gravityPow = mGravityManager.GetPower();

	//�d��
	VECTOR gravity = VScale(dirGravity,gravityPow);

	//�㉺�̈ړ��ʂɏd�͂�������
	mJumpPow = VAdd(mJumpPow, gravity);

	float dot = VDot(dirGravity, mJumpPow);
	if (dot >= 0.0f)
	{
		//�d�͕����Ɣ��Ε����łȂ���΁A�W�����v�͂�ł�����
		mJumpPow = gravity;
	}

}

void Player::AddCollider(Collider* collider)
{
	mColliders.emplace_back(collider);

}

void Player::ClearCollider(void)
{
	mColliders.clear();
}

bool Player::IsPlay(void)
{
	return mState == STATE::PLAY;
}



bool Player::IsDead(void)
{
	return mState == STATE::DEAD;
}

std::vector<PlayerShot*>& Player::GetShots(void)
{
	return mShots;
}

void Player::SyncParent(Transform& transform, VECTOR addAxis)
{
	//�{�X��͂�3D�����擾
	Quaternion parentRot = mTransformStand.quaRot;

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
	VECTOR localPos = Quaternion::PosAxis(parentRot, mLocalPos);
	//�����܂ŏK��������
	////���΍��W���΍��W�ɕϊ�
	//transform.pos = VAdd(
	//mTransformParent->pos, localPos);

	//���f���̑傫�����A�X�P�[���������΍��W���΍��W�ɕϊ�
	transform.pos = VAdd(
		mTransformStand.pos, VScale(localPos, SCALE));

	//���f������̊�{���X�V
	transform.Update();

}


void Player::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

}

void Player::Collision(void)
{
	
	//���ݍ��W���N�_�Ɉړ�����W�����߂�
	mMovedPos = VAdd(mTransformStand.pos, mMovePow);

	//�Փˏ���(�J�v�Z��)
	CollisionCapsule();

	//�Փˏ���(�d��)
	CollisionGravity();

	//�t���[���Ԃ̈ړ���
	mMoveDiff = VSub(mMovedPos, mTransformStand.pos);

	//�ړ�
	mTransformStand.pos = mMovedPos;
}

void Player::CollisionCapsule(void)
{
	//�J�v�Z�����ړ�����
	//�R�s�[�R���X�g���N�^�c
	//�R���X�g���N�^�̂悤�Ȍ`�����Ă�Ŏ������g���Ă�
	
	Transform trans = Transform(mTransformStand);
	trans.pos = mMovedPos;
	trans.Update();
	Capsule cap = mCapsule->Copy(&trans);

	//�J�v�Z���ƃX�e�[�W���f���̏Փ˔���
	for (auto c : mColliders)
	{
		auto hits = MV1CollCheck_Capsule(
			c->mModelId,-1,
			cap.GetPosTop(),
			cap.GetPosDown(),
			cap.GetRadius()
			);
		
		//�Փˏ���

		for (int i = 0; i < hits.HitNum; i++)
		{
			//���̏Փˏ����擾����
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//�������ׂ����炷���߂ɁA1�|���S����
				//�J�v�Z���̏Փ˔���ɂ���
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(),
					cap.GetPosDown(),
					cap.GetRadius(),
					hit.Position[0],
					hit.Position[1],
					hit.Position[2]
				);

				if (pHit)
				{
					//�Փ˂��Ă���
					float pow = 1.0f;				//������Ƃ���������
					auto Nhit = hit.Normal;			//�Փ˂����|���S���̖@��

					//mMovedPos���Փ˂����|���S���̖@���ɁA
					//������Ƃ����ړ�������
					mMovedPos = VAdd(mMovedPos,VScale(Nhit, pow));

					//�J�v�Z�����ړ�������
					trans.pos = mMovedPos;
					trans.Update();
					cap = mCapsule->Copy(&trans);

					continue;
				}

				//�Փ˂��Ă��Ȃ��̂Ń��[�v�I��
				break;
			}
		}
		//���o�����|���S���Ƃ̏Փˏ�����n��
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::CollisionGravity(void)
{

	//�d�͂̕���
	VECTOR dirGravity = mGravityManager.GetDirGravity();

	//�d�͂̔��Ε���
	VECTOR dirUpGravity = mGravityManager.GetDirUpGravity();

	//�d�͂̋���
	float gravityPow = mGravityManager.GetPower();

	//���� mMovedPos
	
	float checkPow = 10.0f;

	//�����̂�����Ə�̍��W
	mGravHitUp = VAdd(mMovedPos, VScale(dirUpGravity, gravityPow));
	mGravHitUp = VAdd(mGravHitUp, VScale(dirUpGravity, checkPow * 2.0f));

	//�����̂�����Ɖ��̍��W
	mGravHitDown = VAdd(mMovedPos, VScale(dirGravity, checkPow));
	
	for (auto c : mColliders)
	{
		//�n��(�X�e�[�W���f��)�Ƃ̏Փ�
		//�����̏ꍇ�͏Փ˂͈̔͂�1�|���S�������ɂȂ�
		auto hit = MV1CollCheck_Line(c->mModelId, -1, mGravHitUp, mGravHitDown);
		if (hit.HitFlag > 0)
		{
			//�X�Δ���p�ɏՓˏ�������Ă���
			mHitNormal = hit.Normal;
			mHitPos = hit.HitPosition;

			//�Փ˂��Ă���(�Փ˂������W(���傢��))
			float dis = 2.0f;
			mMovedPos = VAdd(hit.HitPosition,
				VScale(dirUpGravity, dis));
		}
	}
}

void Player::CreateShot(void)
{
	bool isCreate = false;

	VECTOR pos = mTransformGun.pos;

	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�������
			//�C���X�^���X���g���܂킷
			s->Create(
				VGet(pos.x - 10.0f,pos.y,pos.z),
				mTransformGun.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//�C���X�^���X��V�������
		auto newShot = new PlayerShot(&mTransformGun);
		newShot->Create(
			VGet(pos.x - 10.0f, pos.y, pos.z),
			mTransformGun.GetForward());

		//�ϒ��z��ɒǉ�
		mShots.emplace_back(newShot);
	}
}
