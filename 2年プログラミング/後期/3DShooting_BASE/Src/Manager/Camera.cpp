#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "SceneManager.h"
#include "Camera.h"

Camera::Camera(void)
{

	mPos = VECTOR();
	mQuaRot = Quaternion();
	mTargetPos = VECTOR();
	mCameraUp = VECTOR();

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	ChangeMode(MODE::FIXED_POINT);
	

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	mPos = DEFAULT_CAMERA_POS;
	mTargetPos = VAdd(mPos, RELATIVE_TARGET_POS);
	mCameraUp = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A���̌X������Ԃ��X�������Ƃ���
	// mQuaRot�͉�]�v�Z�p�ŁA
	// �����܂Ŏ��ƂȂ�̂́A�J�������W�ƒ����_�Ƃ���
	mQuaRot = Quaternion();

}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mMode)
	{
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSpring();
		break;
	case Camera::MODE::SHAKE:
		SetBeforeDrawShake();
		break;
	case Camera::MODE::LOCK:
		SetBeforeDrawLock();
		break;
	}

	// �J�����̐ݒ�
	SetCameraPositionAndTargetAndUpVec(
		mPos,
		mTargetPos,
		mCameraUp
	);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
	
}

void Camera::SetBeforeDrawFree(void)
{
	//�J��������ł���悤�Ɏ���

	//��]����
	VECTOR axis = AsoUtility::VECTOR_ZERO;
	float rev = 1.0f;
	if (CheckHitKey(KEY_INPUT_UP)) { axis = AsoUtility::AXIS_X; rev = -1.0f;}
	if (CheckHitKey(KEY_INPUT_DOWN)) { axis = AsoUtility::AXIS_X; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { axis = AsoUtility::AXIS_Y; rev = -1.0f;}
	if (CheckHitKey(KEY_INPUT_RIGHT)) { axis = AsoUtility::AXIS_Y; }

	//�J�������W�𒆐S�Ƃ��āA�����_����]������
	if (!AsoUtility::EqualsVZero(axis))
	{
		//����̉�]�ʂ�����
		Quaternion axisQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f * rev), axis);
		mQuaRot = mQuaRot.Mult(axisQ);

		//�ʒu���ɉ�]���𔽉f������
		VECTOR localPos = mQuaRot.PosAxis(RELATIVE_TARGET_POS);

		//�����_�X�V
		mTargetPos = VAdd(mPos, localPos);

		//�J�����̏�����X�V
		mCameraUp = mQuaRot.GetUp();
			
	}




	//�ړ�����
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	if (CheckHitKey(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		float speed = 20.0f;
		//VTransform�̃N�H�[�^�j�I���o�[�W����

		//lpos = q * lpos * q(-1)

		//Quaternion tmp;
		//tmp = tmp.Mult(mQuaRot);
		////�~�X��w,x,y,z�̏��ԂɂȂ��Ă���
		//tmp = tmp.Mult(Quaternion(0.0f,moveDir.x, moveDir.y, moveDir.z));
		//tmp = tmp.Mult(mQuaRot.Inverse());

		//�ړ��������J���������ɍ��킹�ĉ�]
		VECTOR rotDir = mQuaRot.PosAxis(moveDir);

		//�ړ��������������Ɉړ��ʂ�������
		VECTOR movePow = VScale(rotDir, speed);

		//�J�����ʒu�ƒ����_�𐧌�
		mPos = VAdd(mPos, movePow);
		mTargetPos = VAdd(mTargetPos, movePow);

	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//��_�J�����Ȃ̂ŉ������Ȃ�
}

void Camera::SetBeforeDrawFollow(void)
{
	//�Ǐ]�Ώۂ̏��
	VECTOR targetPos = mTarget->pos;
	MATRIX targetMat = mTarget->matRot;
	Quaternion targetRot = mTarget->quaRot;

	//mTargetPos = VAdd(targetPos, VTransform({ 0, 0, 400 }, targetMat));
	//mPos = VAdd(targetPos, VTransform({ 0, 100, -200 }, targetMat));


	////MATRIX
	////�J�������W
	//VECTOR relativeCPos = VTransform({ 0.0f,25.0f,-80.0f }, targetMat);

	////�J�����ʒu�̍X�V
	//mPos = VAdd(mPos,relativeCPos);

	////�����_(���@�̑O)
	//VECTOR relativeTPos = VTransform(RELATIVE_TARGET_POS, targetMat);
	//mTargetPos = VAdd(mPos, relativeTPos);

	////�J�����̏����
	//mCameraUp = VTransform({ 0.0f,1.0f,0.0f }, targetMat);


	//QUATERNION
	//�J�������W
	VECTOR relativeCPos = targetRot.PosAxis({ 0.0f,25.0f,-80.0f });

	

	//�J�����ʒu�̍X�V
	mPos = VAdd(targetPos, relativeCPos);

	//�����_(���@�̑O)
	VECTOR relativeTPos = targetRot.PosAxis(RELATIVE_TARGET_POS);
	mTargetPos = VAdd(mPos, relativeTPos);

	//�J�����̏����
	mCameraUp = targetRot.PosAxis({ 0.0f,1.0f,0.0f });

	mCameraUp = mTarget->GetUp();
}

void Camera::SetBeforeDrawFollowSpring(void)
{
	//���@�̏��
	VECTOR shipPos = mTarget->pos;
	MATRIX shipMat = mTarget->matRot;
	Quaternion shipRot = mTarget->quaRot;

	//�J�����̍��W
	VECTOR relativeCPos = shipRot.PosAxis({ 0.0f,40.0f,150.0f });

	//�o�l�萔
	float POW_SPRING = 24.0f;
	float dampening = 2.0f * sqrtf(POW_SPRING);

	float delta = 1.0f / 60.0f;

	VECTOR idealPos = VAdd(shipPos, relativeCPos);

	VECTOR diff = VSub(mPos, idealPos);

	VECTOR force = VScale(diff, -POW_SPRING);

	force = VSub(force, VScale(mVelocity, dampening));

	mVelocity = VAdd(mVelocity, VScale(force, delta));

	//�J�����ʒu�̍X�V
	mPos = VAdd(mPos, VScale(mVelocity, delta));


	//�����_
	VECTOR localTPos = shipRot.PosAxis(RELATIVE_TARGET_POS);
	mTargetPos = VAdd(mPos, localTPos);

	//�J�����̏����
	mCameraUp = mTarget->GetUp();
}

void Camera::SetBeforeDrawShake(void)
{
	//���Ԍo�ߏ���
	mStepShake -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepShake < 0.0f)
	{
		//�I������
		mPos = mDefaultPos;
		//�Œ�J�����ɖ߂�
		ChangeMode(MODE::FIXED_POINT);
		return;
	}

	//�J�����h�炵����
	//�g�̗� = �g�̍��� * sin(���� * ���x)
	float movePow = WIDTH_SHAKE * sinf(mStepShake * SPEED_SHAKE);
	VECTOR velocity = VScale(mShakeDir, movePow);
	VECTOR newPos = VAdd(mDefaultPos, velocity);
	mPos = newPos;

	//�J�����h�炵����(������)

	//0 or 1 -> -1 or 1

	//���������
	//int cnt = static_cast<int>(mStepShake * SPEED_SHAKE);
	////0 or 1
	//int shake = cnt % 2;
	////0 or 2
	//shake *= 2;
	////-1 or 1
	//shake -= 1;

	//float mMovePow = WIDTH_SHAKE * shake;
	//VECTOR mVelocity = VScale(mShakeDir, mMovePow);
	//VECTOR mNewPos = VAdd(mDefaultPos, mVelocity);
	//mPos = mNewPos;

}

void Camera::SetBeforeDrawLock(void)
{
}


void Camera::Draw()
{
}

void Camera::DrawUI(void)
{

	// �������
	int x = 200;
	int y = 10;
	int addY = 40;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�y����z");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�@�@�@����F��������");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�@�@�@�����F�a");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�@�@�@���e�F�m");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�V�[���J�ځF�r��������");
	
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return mPos;
}

VECTOR Camera::GetTargetPos(void) const
{
	return mTargetPos;
}

VECTOR Camera::GetDir(void) const
{
	return VNorm(VSub(mTargetPos, mPos));
}

void Camera::ChangeMode(MODE mode)
{

	SetDefault();

	mMode = mode;
	switch (mMode)
	{
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::SHAKE:
		//�J�����h�炵�O�̏�����
		mStepShake = TIME_SHAKE;
		//�΂�45�x
		mShakeDir = VNorm({0.7f,0.7f,0.0f});
		mDefaultPos = mPos;
		break;
	}
}

void Camera::SetFollowTarget(const Transform* target)
{
	mTarget = target;
}
