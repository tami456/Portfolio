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
#include "PlayerShot.h"
#include "Player.h"

Player::Player(void) : mResourceManager(ResourceManager::GetInstance()), mGravityManager(GravityManager::GetInstance())
{

	mAnimationController = nullptr;
	mState = STATE::NONE;
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	// ���f���̊�{�ݒ�
	mTransform.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 0.0f, -30.0f, 0.0f };
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

	mIsJump = false;
	mStepJump = 0.0f;

	//�J�v�Z���R���C�_
	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({0.0f,110.0f,0.0f});
	mCapsule->SetRelativePosDown({0.0f,30.0f,0.0f});
	mCapsule->SetRadius(20.0f);

	//�����̃G�t�F�N�g
	mEffectSmoke = mResourceManager.Load(
		ResourceManager::SRC::FOOT_SMOKE).mHandleId;
	
	mStepFootSmoke = TIME_FOOT_SMOKE;
	mMoveDiff = AsoUtility::VECTOR_ZERO;
	mPlayEffectSmoke = -1;

	//���[�v�G�t�F�N�g
	mEffectWarpOrbit = mResourceManager.Load(
		ResourceManager::SRC::WARP_ORBIT).mHandleId;

	//���f���t���[��(��)���f���t���[����T��
	mFrameLeftHand = MV1SearchFrame(mTransform.modelId,"mixamorig:LeftHand");
	mFrameRightHand = MV1SearchFrame(mTransform.modelId, "mixamorig:RightHand");


	mSlopeDir = AsoUtility::VECTOR_ZERO;
	mSlopeAngleDeg = 0.0f;
	mSlopePow = AsoUtility::VECTOR_ZERO;
	mHitNormal = AsoUtility::VECTOR_ZERO;
	mHitPos = AsoUtility::VECTOR_ZERO;


	mSpeechBalloon = new SpeechBalloon(
		SpeechBalloon::TYPE::SPEECH, &mTransform);

	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({ 15.0f,180.0f,0.0f });

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	//�������
	ChangeState(STATE::PLAY);
}

void Player::InitAnimation(void)
{

	std::string path = Application::PATH_MODEL + "Player/";
	mAnimationController = new AnimationController(mTransform.modelId);
	mAnimationController->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	mAnimationController->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::KICK, path + "magic.mv1", 90.0f);

	mAnimationController->Play((int)ANIM_TYPE::IDLE);

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
	case Player::STATE::WARP_RESERVE:
		UpdateWarpReserve();
		break;
	case Player::STATE::WARP_MOVE:
		UpdateWarpMove();
		break;
	case Player::STATE::DEAD:
		UpdateDead();
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

	mTransform.Update();
	mAnimationController->Update();
	
}

void Player::UpdatePlay(void)
{
	//�ړ�����(�ړ��ʂ����߂�)
	ProcessMove();
	
	//�W�����v����
	ProcessJump();

	//�X�΂̌v�Z
	CalcSlope();

	//�d�͂ɂ��ړ��ʂ��v�Z
	CalcGravityPow();

	//�Փ˔���
	Collision();

	//��]
	mTransform.quaRot = mGravityManager.GetTransform().quaRot;

	//��������������������
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);

	//�����̃G�t�F�N�g����
	EffectFootSmoke();
	auto name = mGravityManager.GetActivePlanet()->GetName();
	auto planetDead = mGravityManager.GetActivePlanet();
	if (mTransform.pos.y < -1000.0f || (planetDead->InRangeDead(mTransform.pos) && name != mPreWarpName))
	{
		ChangeState(STATE::DEAD);
	}

}

void Player::UpdateWarpReserve(void)
{
	mStepWarp -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepWarp < 0.0f)
	{
		//���B������
		mTransform.quaRot = mWarpQua;
		mTransform.pos = mWarpReservePos;
		ChangeState(STATE::WARP_MOVE);

		SyncWarpOrbitPos();
	}
	else
	{
		//���X�Ƀv���C���[���������W�Ɉړ��E��]������

		//�X�^�[�g��]�FmReserveStartQua;
		//�X�^�[�g���W�FmReserveStartPos;
		
		//�S�[����]�FmWarpQua;
		//�S�[�����W�FmWarpReservePos;

		//�ړ��O�ƈړ���̍��W�Ɖ�]�̔䗦��n��
		//���W�͐��`�⊮
		//t�̒l���A0.0f��������mReserveStartPos
		//t�̒l���A0.5f�������灪���̒���
		//t�̒l���A1.0f��������mWarpReservePos

		float t;
		t = 1.0f - (mStepWarp/mTimeWarp);
		mTransform.pos = AsoUtility::Lerp(mReserveStartPos, mWarpReservePos, t);
		

		//��]�͋��ʕ⊮
		mTransform.quaRot = Quaternion::Slerp(mReserveStartQua, mWarpQua, t);
	}
}

void Player::UpdateWarpMove(void)
{
	
	//�L�����N�^�[���ړ������鏈��
	//�X�^�[�̕����A�ړ�����������(��])�͂��łɌ��܂��Ă���̂�
	//���̕����Ƀv���C���[���ړ�������
	mTransform.pos = VAdd(mTransform.pos,VScale(mWarpQua.GetForward(), SPEED_FLY));
	mTransform.Update();

	auto name = mGravityManager.GetActivePlanet()->GetName();
	if (name != mPreWarpName)
	{
		//�G�t�F�N�g��~
		StopOrbitEffect();

		//�����A�j���[�V����
		mAnimationController->Play(
			static_cast<int>(ANIM_TYPE::JUMP),
			true, 13.0f, 25.0f);

		mAnimationController->SetEndLoop(23.0f, 25.0f, 5.0f);

		//���ʂ̑����Ԃ̖߂�
		ChangeState(STATE::PLAY);

	}
	
	//�G�t�F�N�g�̈ʒu����
	SyncWarpOrbitPos();
}

void Player::UpdateDead(void)
{
	//���ʂ̑����Ԃ̖߂�
	ChangeState(STATE::DEAD);
}

void Player::Draw(void)
{

	// ���f���̕`��
	MV1DrawModel(mTransform.modelId);

	//�ۉe�`��
	DrawShadow();

	// �f�o�b�O�p�`��
	//DrawDebug();

}

void Player::DrawShadow(void)
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
			if (HitRes->Position[0].y >mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
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



void Player::DrawDebug(void)
{

	int white = 0xffffff;
	int black = 0x000000;
	int red = 0xff0000;
	int green = 0x00ff00;
	int blue = 0x0000ff;
	int yellow = 0xffff00;
	int purpl = 0x800080;

	VECTOR v;

	// �L������{���
	//-------------------------------------------------------
	// �L�������W
	v = mTransform.pos;
	DrawFormatString(20, 60, black, "�L�������W �F (%0.2f, %0.2f, %0.2f)",
		v.x, v.y, v.z
	);

	v = mTransform.pos;
	DrawFormatString(20, 60, black, "�X�Ίp �F %0.2f",
		mSlopeAngleDeg
	);
	//-------------------------------------------------------

	//�ݒu�Փ˗p�̐���
	DrawLine3D(mGravHitUp,mGravHitDown,0xff0000);

	//�J�v�Z���R���C�_�̕\��
	mCapsule->Draw();

};
void Player::Release(void)
{
	mAnimationController->Release();
	delete mAnimationController;

	delete mCapsule;
	DeleteGraph(mImgShadow);

	//�e�̊J��
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();

	mSpeechBalloon->Release();
	delete mSpeechBalloon;
}

Transform* Player::GetTransform(void)
{
	return &mTransform;
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
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	//�ړ�����
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	float rotRad = 0;

	if (CheckHitKey(KEY_INPUT_W))
	{
		//�ړ����������߂�
		dir = cameraRot.GetForward();
		rotRad = AsoUtility::Deg2RadF(0.0f);
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		//�ړ����������߂�
		dir = cameraRot.GetBack();
		rotRad = AsoUtility::Deg2RadF(180.0f);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		//�ړ����������߂�
		dir = cameraRot.GetRight();
		rotRad = AsoUtility::Deg2RadF(90.0f);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		//�ړ����������߂�
		dir = cameraRot.GetLeft();
		rotRad = AsoUtility::Deg2RadF(-90.0f);
	}

	//�ړ��L�[�����͂���Ă�����	  &&  �W�����v���[�V��������������
	if (!AsoUtility::EqualsVZero(dir) && (mIsJump || IsEndLanding()))
	{
		//��](���B��������]�����߂�)
		SetGoalRotate(rotRad);

		ANIM_TYPE type = ANIM_TYPE::RUN;
		mSpeed = SPEED_MOVE;
		if (CheckHitKey(KEY_INPUT_LSHIFT))
		{
			mSpeed = SPEED_RUN;
			type = ANIM_TYPE::FAST_RUN;
		}
		//�ړ���������ړ��ʂ��Z�o����
		//�ړ��ʁ@= �����@�~�@�X�s�[�h
		mMoveDir = dir;
		mMovePow = VScale(dir, mSpeed);
		
		//�J�����̕���������
		//mTransform.quaRot = cameraRot.Mult(Quaternion::AngleAxis(rotRad,AsoUtility::AXIS_Y));
		if (!mIsJump && IsEndLanding())
		{
			//�A�j���[�V�����Đ�(���ꂼ��Ŏw��o���Ȃ�����int�^�ɂ���)
			mAnimationController->Play(static_cast<int>(type));
		}

	}
	else
	{
		if (!mIsJump && IsEndLanding())
		{

			if (CheckHitKey(KEY_INPUT_E))
			{
				
				mAnimationController->Play(static_cast<int>(ANIM_TYPE::KICK),13.0f,20.0f,false);
				ProcessShot();
			}
			else
			{
				//�A�j���[�V�����Đ�
				mAnimationController->Play(static_cast<int>(ANIM_TYPE::IDLE));
			}
			
		}

		

	}

	

	//��](�S�[���ɑ΂��āA���ʕ⊮���s��)
	Rotate();
}

void Player::ProcessJump(void)
{

	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);

	//�W�����v�󂯕t��
	if (isHit && (mIsJump || IsEndLanding()))
	{
		if (!mIsJump)
		{
			//mAnimationController->Play(static_cast<int>(ANIM_TYPE::JUMP));
			//mAnimationController->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
			
			////�؂���A�j���[�V����
			//mAnimationController->Play(
			// static_cast<int>(ANIM_TYPE::JUMP), false,
			//	13.0f, 24.0f);

			//������蕂�V�����[�V���������
			mAnimationController->Play(
				static_cast<int>(ANIM_TYPE::JUMP), true,
				13.0f, 24.0f);
			mAnimationController->SetEndLoop(23.0f,25.0f,5.0f);

		}

		mIsJump = true;

		//�W�����v�̓��͎�t���Ԃ����炷
		mStepJump += SceneManager::GetInstance().GetDeltaTime();
		if (mStepJump < TIME_JUMP_IN)
		{
			mJumpPow = VScale(mGravityManager.GetDirUpGravity(),POW_JUMP);
		}
	}

	

	//�{�^���𗣂�����W�����v�͂ɉ��Z���Ȃ�
	if (!isHit)
	{
		mStepJump = TIME_JUMP_IN;
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

void Player::SetGoalRotate(float rotRad)
{
	//rad�P�ʂ̃A���O�����Ƃ�
	VECTOR cameraAngles = 
		SceneManager::GetInstance().GetCamera()->GetAngles();

	Quaternion axis = 
		Quaternion::AngleAxis(
			cameraAngles.y + rotRad,
			AsoUtility::AXIS_Y);

	//���ݐݒ肳��Ă����]�Ƃ̊p�x�������߂�
	double angleDiff = Quaternion::Angle(axis,mGoalQuaRot);

	//�������l
	if (angleDiff > 0.1f)
	{
		mStepRot = TIME_ROT;
	}

	//���B��������]��ݒ�
	mGoalQuaRot = axis;

}

void Player::Rotate(void)
{

	mStepRot -= SceneManager::GetInstance().GetDeltaTime();

	//mPlayerRotY = Quaternion::Slerp(
	//mPlayerRotY,mGoalQuaRot,(TIME_ROT - mStepRot) / TIME_ROT);

	mPlayerRotY = Quaternion::Slerp(
		mPlayerRotY, mGoalQuaRot, 0.2f);

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

void Player::StartWarp(
	float time, const Quaternion& goalRot, const VECTOR& goalPos)
{

	mTimeWarp = time;
	mStepWarp = time;
	//�i�s����
	mWarpQua = goalRot;
	mWarpReservePos = goalPos;
	mPreWarpName = mGravityManager.GetActivePlanet()->GetName();

	//���[�v�̏����i�K�ɓ���
	ChangeState(STATE::WARP_RESERVE);
}

void Player::StopOrbitEffect(void)
{
	//�G�t�F�N�g��~
	StopEffekseer3DEffect(mPlayOrbitEffectL);
	StopEffekseer3DEffect(mPlayOrbitEffectR);
}

bool Player::IsPlay(void)
{
	return mState == STATE::PLAY;
}

bool Player::IsWarpMove(void)
{
	return mState == STATE::WARP_MOVE;
}

bool Player::IsDead(void)
{
	return mState == STATE::DEAD;
}

std::vector<PlayerShot*>& Player::GetShots(void)
{
	return mShots;
}

SpeechBalloon* Player::GetSpeechBalloon(void) const
{
	return mSpeechBalloon;
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
	case Player::STATE::WARP_RESERVE:
		
		mJumpPow = AsoUtility::VECTOR_ZERO;

		//�ړ��O�ƈړ���̍��W�Ɖ�]�̔䗦��n��
		//���[�v�����J�n���̃v���C���[���
		mReserveStartQua = mTransform.quaRot;
		mReserveStartPos = mTransform.pos;

		//�|�[�Y�A�j���[�V����
		mAnimationController->Play(
			static_cast<int>(Player::ANIM_TYPE::WARP_PAUSE));

		break;
	case Player::STATE::WARP_MOVE:

		//�L�����N�^�[�����ʂ������Ă�����
		mPlayerRotY = Quaternion();

		//������������
		mGoalQuaRot = Quaternion();

		//��ԃA�j���[�V����
		mAnimationController->Play(
			static_cast<int>(Player::ANIM_TYPE::FLY));

		EffectWarpOrbit();

		break;
	case Player::STATE::DEAD:
		//��ԃA�j���[�V����
		mAnimationController->Play(
			static_cast<int>(Player::ANIM_TYPE::FLY));
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

void Player::CollisionCapsule(void)
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

			if (mIsJump)
			{
				//���n���[�V����
				mAnimationController->Play(
					static_cast<int>(ANIM_TYPE::JUMP), false,
					29.0f, 45.0f, false, true
				);
			}

			//�W�����v�I��
			mIsJump = false;
			mStepJump = 0.0f;
		}
	}
}

bool Player::IsEndLanding(void)
{
	//���n���[�V�������I����Ă���
	bool ret = true;

	if (mAnimationController->GetPlayType() 
		!= static_cast<int>(ANIM_TYPE::JUMP))
	{
		//�W�����v���[�V��������Ȃ�(���n���[�V�������I����Ă���)

		return ret;
	}

	if (mAnimationController->IsEnd())
	{
		//�W�����v���[�V�������I�����Ă���
		return ret;
	}

	return false;
}

void Player::EffectFootSmoke(void)
{
	//�~���F�v���C���[�̑������牌��0.3�b���Ƃɕ`�悵���A
	//		�v���C���[�������Ă��鉉�o���s���B
	//		���v���C���[�������Ă��Ȃ��Ƃ��́A����V���ɔ��������Ȃ�
	// 
	//�t���[���Ԃ̈ړ��ʁFmMoveDiff
	//0.3�b����			�FTIME_FOOT_SMOKE
	//�G�t�F�N�g��HID	�FmEffectSmoke

	//�G�t�F�N�g���Đ�����֐�
	//PlayEffekseer3DEffect

	//�G�t�F�N�g�̑傫����ݒ肷��֐�
	//SetScalePlayingEffekseer3DEffect

	//�G�t�F�N�g�̈ʒu��ݒ肷��֐�
	//SetPosPlayingEffekseer3DEffect

	mMoveDiff.y = 0.0f;
	//�ړ��ʂ�float��
	float len = AsoUtility::SqrMagnitudeF(mMoveDiff);

	mStepFootSmoke -= SceneManager::GetInstance().GetDeltaTime();

	if (len != 0.0f)
	{
		if (mStepFootSmoke <= 0.0f)
		{
			mStepFootSmoke = TIME_FOOT_SMOKE;
			mPlayEffectSmoke = PlayEffekseer3DEffect(mEffectSmoke);

			float Scale = 10.0f;

			SetScalePlayingEffekseer3DEffect(mPlayEffectSmoke, Scale, Scale, Scale);

			SetPosPlayingEffekseer3DEffect(
				mPlayEffectSmoke,
				mTransform.pos.x, mTransform.pos.y, mTransform.pos.z
			);
		}
	}

	
}

void Player::EffectWarpOrbit(void)
{

	//�@�O�ՃG�t�F�N�g�̐���
	mPlayOrbitEffectL = PlayEffekseer3DEffect(mEffectWarpOrbit);
	mPlayOrbitEffectR = PlayEffekseer3DEffect(mEffectWarpOrbit);

	//�A�傫���̐ݒ�
	float scale = 10.0f;
	SetScalePlayingEffekseer3DEffect(mPlayOrbitEffectL, scale, scale, scale);
	SetScalePlayingEffekseer3DEffect(mPlayOrbitEffectR, scale, scale, scale);

	//�B�ʒu�̐ݒ�(�E��A����)
	SyncWarpOrbitPos();

	//�C�G�t�F�N�g�̉�]
	VECTOR euler = mTransform.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(
		mPlayOrbitEffectL, euler.x, euler.y, euler.z
	);

	SetRotationPlayingEffekseer3DEffect(
		mPlayOrbitEffectR, euler.x, euler.y, euler.z
	);
}

void Player::SyncWarpOrbitPos(void)
{
	//mFrameLeftHand
	//mFrameRightHand

	VECTOR pos;

	//����̍��W(�Ώۃ��f���̃t���[���ԍ��̈ʒu���擾)
	pos = MV1GetFramePosition(mTransform.modelId, mFrameLeftHand);

	SetPosPlayingEffekseer3DEffect(
		mPlayOrbitEffectL,
		pos.x, pos.y, pos.z
	);

	//����̍��W(�Ώۃ��f���̃t���[���ԍ��̈ʒu���擾)
	pos = MV1GetFramePosition(mTransform.modelId, mFrameRightHand);

	SetPosPlayingEffekseer3DEffect(
		mPlayOrbitEffectR,
		pos.x, pos.y, pos.z
	);
}

void Player::CreateShot(void)
{
	bool isCreate = false;

	VECTOR pos;

	//����̍��W(�Ώۃ��f���̃t���[���ԍ��̈ʒu���擾)
	pos = MV1GetFramePosition(mTransform.modelId, mFrameLeftHand);

	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�������
			//�C���X�^���X���g���܂킷
			s->Create(
				VGet(pos.x - 10.0f,pos.y,pos.z),
				mTransform.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//�C���X�^���X��V�������
		auto newShot = new PlayerShot(&mTransform);
		newShot->Create(
			VGet(pos.x - 10.0f, pos.y, pos.z),
			mTransform.GetForward());

		//�ϒ��z��ɒǉ�
		mShots.emplace_back(newShot);
	}
}
