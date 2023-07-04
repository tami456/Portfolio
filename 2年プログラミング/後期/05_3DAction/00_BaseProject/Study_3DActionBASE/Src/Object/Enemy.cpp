#include <string>
#include <EffekseerForDxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Common/SpeechBalloon.h"
#include "Planet.h"
#include "Player.h"
#include "Enemy.h"



Enemy::Enemy(Player* player) : mResourceManager(ResourceManager::GetInstance()), mGravityManager(GravityManager::GetInstance())
{
    mAnimationController = nullptr;
	mPlayer = player;
    mState = STATE::NONE;
}

Enemy::~Enemy()
{
}

void Enemy::Init(void)
{
	// ���f���̊�{�ݒ�
	mTransform.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 100.0f, -30.0f, 100.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	mTransform.Update();

	mPlayerRotY = Quaternion();
	mGoalQuaRot = Quaternion();
	mStepRot = 0.0f;

	mJumpPow = AsoUtility::VECTOR_ZERO;
	mGravHitDown = AsoUtility::VECTOR_ZERO;
	mGravHitUp = AsoUtility::VECTOR_ZERO;

	//�ۉe�摜
	mImgShadow = LoadGraph((Application::PATH_IMAGE + "Shadow.png").c_str());

	//�J�v�Z���R���C�_
	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({ 0.0f,110.0f,0.0f });
	mCapsule->SetRelativePosDown({ 0.0f,30.0f,0.0f });
	mCapsule->SetRadius(20.0f);

	mMoveDiff = AsoUtility::VECTOR_ZERO;

	mSlopeDir = AsoUtility::VECTOR_ZERO;
	mSlopeAngleDeg = 0.0f;
	mSlopePow = AsoUtility::VECTOR_ZERO;
	mHitNormal = AsoUtility::VECTOR_ZERO;
	mHitPos = AsoUtility::VECTOR_ZERO;

	mIsAlive = false;

	mHp = 100;

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	//�������
	ChangeState(STATE::PLAY);
}

void Enemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Enemy/";
	mAnimationController = new AnimationController(mTransform.modelId);
	mAnimationController->Add((int)ANIM_TYPE::IDLE, path + "Dancing.mv1", 50.0f);
	mAnimationController->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 50.0f);
	mAnimationController->Add((int)ANIM_TYPE::DEAD, path + "Dead.mv1", 20.0f);

	mAnimationController->Play((int)ANIM_TYPE::RUN);
}

void Enemy::Update(void)
{
	switch (mState)
	{
	case Enemy::STATE::NONE:
		break;
	case Enemy::STATE::PLAY:
		UpdatePlay();
		break;
	case Enemy::STATE::IDLE:
		UpdateIdle();
		break;
	case Enemy::STATE::DEAD:
		break;
	}

	mTransform.Update();
	mAnimationController->Update();
}

bool IsHitSpheres(VECTOR& pos1, float radius1, VECTOR& pos2, float radius2)
{
	bool isHit = false;
	VECTOR diff = VSub(pos1, pos2);
	float radius = radius1 + radius2;

	auto dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	if (dis < (radius * radius))
	{
		isHit = true;
	}

	return isHit;
}

void Enemy::UpdatePlay(void)
{
	

	//�ړ�����(�ړ��ʂ����߂�)
	ProcessMove();

	//�Փ˔���
	Collision();

	//�d�͂ɂ��ړ��ʂ��v�Z
	CalcGravityPow();

	//��]
	mTransform.quaRot = mGravityManager.GetTransform().quaRot;

	//��������������������
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);

	//�����̓v���C���[�ɓ���������A�j���[�V�����ύX
	if(IsHitSpheres(mPlayer->GetTransform()->pos, POS_RADIUS, mTransform.pos, POS_RADIUS))
	{
		ChangeState(STATE::IDLE);
	}
}

void Enemy::UpdateIdle(void)
{
	
	//�����̓v���C���[�ɓ���������A�j���[�V�����ύX
	if (!IsHitSpheres(mPlayer->GetTransform()->pos, POS_RADIUS, mTransform.pos, POS_RADIUS))
	{
		ChangeState(STATE::PLAY);
	}
}

void Enemy::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(mTransform.modelId);

	//�ۉe�`��
	DrawShadow();

	//DrawDebug();
}

void Enemy::DrawDebug(void)
{

	int white = 0xffffff;
	int black = 0x000000;
	int red = 0xff0000;
	int green = 0x00ff00;
	int blue = 0x0000ff;
	int yellow = 0xffff00;
	int purpl = 0x800080;

	//�ݒu�Փ˗p�̐���
	DrawLine3D(mGravHitUp, mGravHitDown, 0xff0000);

	//�J�v�Z���R���C�_�̕\��
	mCapsule->Draw();

};

void Enemy::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i, j;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�
	for (auto c : mColliders)
	{
		// �`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��
		ModelHandle = c->mModelId;


		// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(ModelHandle, -1,
			mTransform.pos,
			VAdd(mTransform.pos,
				VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		// ���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// �|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// �|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// �e�|���S����`��
			DrawPolygon3D(Vertex, 1, mImgShadow, TRUE);
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);
}

void Enemy::Release(void)
{
	mAnimationController->Release();
	delete mAnimationController;

	delete mCapsule;
	DeleteGraph(mImgShadow);
}

Transform& Enemy::GetTransform(void)
{
	return mTransform;
}

Capsule* Enemy::GetCapsule(void)
{
	return mCapsule;
}

float Enemy::GetCollisionRadius(void) const
{
	return POS_RADIUS;
}

bool Enemy::IsAlive(void) const
{
	return mIsAlive;
}

void Enemy::SetGoalRotate(float rotRad)
{
	//rad�P�ʂ̃A���O�����Ƃ�
	VECTOR enemyAngle = SceneManager::GetInstance().GetCamera()->GetAngles();

	Quaternion axis =
		Quaternion::AngleAxis(
			rotRad,
			AsoUtility::AXIS_Y);

	//���ݐݒ肳��Ă����]�Ƃ̊p�x�������߂�
	double angleDiff = Quaternion::Angle(axis, mGoalQuaRot);

	//�������l
	if (angleDiff > 0.1f)
	{
		mStepRot = TIME_ROT;
	}

	//���B��������]��ݒ�
	mGoalQuaRot = axis;
}

void Enemy::Rotate(void)
{
	mStepRot -= SceneManager::GetInstance().GetDeltaTime();

	//mPlayerRotY = Quaternion::Slerp(
	//mPlayerRotY,mGoalQuaRot,(TIME_ROT - mStepRot) / TIME_ROT);

	mPlayerRotY = Quaternion::Slerp(
		mPlayerRotY, mGoalQuaRot, 0.5f);
}

void Enemy::ProcessMove(void)
{

	//�ړ��ʂ�0
	mMovePow = AsoUtility::VECTOR_ZERO;

	float rotRad = 0;

	//�ړ����������߂�
	// �R�c���f���Q����R�c���f���P�Ɍ������x�N�g�����Z�o
	dir = VSub(mPlayer->GetTransform()->pos, mTransform.pos);
	dir = VNorm(dir);

	// atan2 ���g�p���Ċp�x���擾
	float Angle = atan2(dir.x, dir.z);

	mSpeed = SPEED_MOVE;

	//��](���B��������]�����߂�)
	SetGoalRotate(Angle);

	//�ړ���������ړ��ʂ��Z�o����
		//�ړ��ʁ@= �����@�~�@�X�s�[�h
	mMoveDir = dir;
	mMovePow = VScale(dir, mSpeed);

	//��](�S�[���ɑ΂��āA���ʕ⊮���s��)
	Rotate();
}

void Enemy::CalcGravityPow(void)
{
	//�d�͕���
	VECTOR dirGravity = mGravityManager.GetDirGravity();

	//�d�͂̋���
	float gravityPow = mGravityManager.GetPower();

	//�d��
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//�㉺�̈ړ��ʂɏd�͂�������
	mJumpPow = VAdd(mJumpPow, gravity);

	float dot = VDot(dirGravity, mJumpPow);
	if (dot >= 0.0f)
	{
		//�d�͕����Ɣ��Ε����łȂ���΁A�W�����v�͂�ł�����
		mJumpPow = gravity;
	}
}

void Enemy::CalcSlope(void)
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

	//�X�΂ɂ��ړ�
	if (AsoUtility::SqrMagnitude(mJumpPow) == 0.0f)
	{
		float CHK_ANGLE = 120.0f;
		if (mSlopeAngleDeg >= CHK_ANGLE)
		{
			float diff = abs(mSlopeAngleDeg - CHK_ANGLE);
			mSlopePow = VScale(mSlopeDir, diff / 3.0f);
			mMovePow = VAdd(mMovePow, mSlopePow);
		}
	}
}

void Enemy::AddCollider(Collider* collider)
{
	mColliders.emplace_back(collider);
}

void Enemy::ClearCollider(void)
{
	mColliders.clear();
}

void Enemy::Damage(void)
{
	mTransform.pos = VAdd(mTransform.pos, VScale(mTransform.GetBack(), 30.0f));
	mTransform.Update();
	mHp -= 1;
	if (mHp <= 0)
	{
		ChangeState(STATE::DEAD);
	}
}

bool Enemy::IsDead(void)
{
	return mState == STATE::DEAD;
}

void Enemy::ChangeState(STATE state)
{
	mState = state;
	switch (mState)
	{
	case Enemy::STATE::NONE:
		break;
	case Enemy::STATE::PLAY:
		mAnimationController->Play((int)ANIM_TYPE::RUN);
		mIsAlive = true;
		break;
	case Enemy::STATE::IDLE:
		mAnimationController->Play((int)ANIM_TYPE::IDLE);
		mIsAlive = true;
		break;
	case Enemy::STATE::DEAD:
		mAnimationController->Play((int)ANIM_TYPE::DEAD, false);
		mIsAlive = false;
		break;
	}
}

void Enemy::Collision(void)
{
	//���ݍ��W���N�_�Ɉړ�����W�����߂�
	mMovedPos = VAdd(mTransform.pos, mMovePow);

	//�Փˏ���(�J�v�Z��)
	CollisionCapsule();

	//�Փˏ���(�d��)
	CollisionGravity();

	//�t���[���Ԃ̈ړ���
	mMoveDiff = VSub(mMovedPos, mTransform.pos);

	//�ړ�
	mTransform.pos = mMovedPos;
}

void Enemy::CollisionCapsule(void)
{
	//�J�v�Z�����ړ�����
	//�R�s�[�R���X�g���N�^�c
	//�R���X�g���N�^�̂悤�Ȍ`�����Ă�Ŏ������g���Ă�

	Transform trans = Transform(mTransform);
	trans.pos = mMovedPos;
	trans.Update();
	Capsule cap = mCapsule->Copy(&trans);

	//�J�v�Z���ƃX�e�[�W���f���̏Փ˔���
	for (auto c : mColliders)
	{
		auto hits = MV1CollCheck_Capsule(
			c->mModelId, -1,
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
					mMovedPos = VAdd(mMovedPos, VScale(Nhit, pow));

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

void Enemy::CollisionGravity(void)
{
	//�W�����v�ʂ����Z
	mMovedPos = VAdd(mMovedPos, mJumpPow);

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
		auto mJumpDot = VDot(dirGravity, VNorm(mJumpPow));
		if (hit.HitFlag > 0 && mJumpDot > 0.9f)
		{
			//�X�Δ���p�ɏՓˏ�������Ă���
			mHitNormal = hit.Normal;
			mHitPos = hit.HitPosition;

			//�Փ˂��Ă���(�Փ˂������W(���傢��))
			float dis = 2.0f;
			mMovedPos = VAdd(hit.HitPosition,
				VScale(dirUpGravity, dis));

			//��R�͂̑����(���n������d�͂�ł�����)
			mJumpPow = AsoUtility::VECTOR_ZERO;

		}
	}
}
