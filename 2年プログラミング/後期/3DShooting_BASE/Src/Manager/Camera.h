#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
class Transform;

class Camera
{

public:

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �J�����ʒu�ƒ����_�Ƃ̑��΍��W
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -100.0f, 500.0f };

	//�V�F�C�N����
	static constexpr float TIME_SHAKE = 2.0f;

	//�V�F�C�N��
	//static constexpr float WIDTH_SHAKE = 3.0f;
	static constexpr float WIDTH_SHAKE = 5.0f;
	
	//�V�F�C�N�X�s�[�h
	//static constexpr float SPEED_SHAKE = 30.0f;
	static constexpr float SPEED_SHAKE = 15.0f;

	//�J�������[�h
	enum class MODE
	{
		NONE,
		FREE,			//�t���[
		FIXED_POINT,	//��_�J����
		FOLLOW,			//�Ǐ]�J����
		FOLLOW_SPRING,	//�o�l�t���Ǐ]�J����
		SHAKE,			//�J�����V�F�C�N(�J�����h�炵)
		LOCK,
	};

	// �R���X�g���N�^
	Camera(void);

	// �f�X�g���N�^
	~Camera(void);

	// ������
	void Init(void);

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �X�V
	void Update(void);

	// �`��O�̃J�����ݒ�
	void SetBeforeDraw(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);
	void SetBeforeDrawShake(void);
	void SetBeforeDrawLock(void);
	
	// �`��
	void Draw(void);
	void DrawUI(void);

	void Release(void);

	// �J�������W
	VECTOR GetPos(void) const;

	// �����_���W
	VECTOR GetTargetPos(void) const;

	// �J��������
	VECTOR GetDir(void) const;

	//�J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	//�Ǐ]�Ώۂ̐ݒ�
	void SetFollowTarget(const Transform* target);
private:

	//�Ǐ]�Ώ�
	const Transform* mTarget;

	//�J�������[�h
	MODE mMode;

	// �J�����̈ʒu
	VECTOR mPos;

	// �J�����p�x
	Quaternion mQuaRot;

	// �����_
	VECTOR mTargetPos;

	// �J�����̏����
	VECTOR mCameraUp;

	//���x
	VECTOR mVelocity;

	//�J�����V�F�C�N�p
	//���Ԍv���p
	float mStepShake;
	//�J�����V�F�C�N�O�̏����ʒu
	VECTOR mDefaultPos;
	//�J������h�炷����
	VECTOR mShakeDir;
};

