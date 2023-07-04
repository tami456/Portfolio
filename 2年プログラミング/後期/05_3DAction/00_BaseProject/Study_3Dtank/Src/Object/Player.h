#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"

class ResourceManager;
class GravityManager;
class Collider;
class Capsule;
class PlayerShot;


class Player
{
public:

	//�ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;

	//�e�̐�������
	static constexpr float ALIVE_SHOT = 10.0f;

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//���G�t�F�N�g�̔����Ԋu
	static constexpr float TIME_FOOT_SMOKE = 0.3f;

	//�e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	// �^���b�g�̑傫��
	static constexpr float SCALE = 0.3f;

	//�C��̃��[�J����]�ғ��p
	static constexpr float ANGLE_Y_MIN_STAND = -30 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_STAND = 30 * (DX_PI_F / 180.0f);

	static constexpr float ANGLE_Y_MIN_GUN = -10 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_GUN = 20 * (DX_PI_F / 180.0f);

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		VICTORY,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		KICK
	};

	Player(void);
	~Player(void);

	void Init(void);
	void Update(void);
	void UpdatePlay(void);
	void UpdateDead(void);

	void Draw(void);

	void Release(void);

	void Attack(void);

	Transform* GetTransform(void);
	Capsule* GetCapsule(void);

	//����
	void ProcessMove(void);
	void ProcessShot(void);

	//�X�΂̌v�Z
	void CalcSlope(void);

	//�d�͂̌v�Z
	void CalcGravityPow(void);

	//�Փ˔���ɗp������R���C�_
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	//�l�n�����ƃR�s�[���Ȃ��Ƃ����Ȃ����玞�Ԃ�������
	//�Q�Ɠn�����Ƃ����ɂ���Ƌ����Ă���邾��
	//const�t���Ȃ���Q�Ɠn�������邱�Ƃň��S�Ƀf�[�^��n����

	//�v���C���[�̏�Ԕ��f
	bool IsPlay(void);
	bool IsDead(void);

	//�e�̎擾
	std::vector<PlayerShot*>& GetShots(void);

	// �{�X���(�e���f��)�Ɖ�]�ƈʒu�̓���
	void SyncParent(Transform& transform, VECTOR addAxis);

private:

	// �{�X�̊�{���
	const Transform* mTransformParent;

	ResourceManager& mResourceManager;
	GravityManager& mGravityManager;

	// ���f������̊�{���(�z�C�[��)
	Transform mTransformWheelL;
	Transform mTransformWheelR;

	// ���f������̊�{���(�C��)
	Transform mTransformStand;

	// ���f������̊�{���(�C�g)
	Transform mTransformGun;

	//�C��̃��[�J���ғ���(deg)
	float mAnglePowStand;

	//�C�g�̃��[�J���ғ���(deg)
	float mAnglePowGun;

	// �ǉ��̃��[�J����]
	VECTOR mLocalAddAxis;

	// �X�ɒǉ��̖C�䃍�[�J����]
	VECTOR mLocalAddAxisStand;

	// �X�ɒǉ��̖C�g���[�J����]
	VECTOR mLocalAddAxisGun;

	// ��͂���̑��΍��W
	VECTOR mLocalPos;


	// ���
	STATE mState;

	//�ړ��X�s�[�h
	float mSpeed;

	//�ړ�����
	VECTOR mMoveDir;

	//�ړ���
	VECTOR mMovePow;

	//�ړ���̍��W
	VECTOR mMovedPos;

	//��](���݂̃v���C���[�̌���)
	Quaternion mPlayerRotY;

	//��](���B����������=�S�[��)
	Quaternion mGoalQuaRot;

	//��]�̎��Ԍv���p
	float mStepRot;

	//�W�����v��
	VECTOR mJumpPow;

	//�Փ˔���ɗp������R���C�_
	Capsule* mCapsule;
	std::vector<Collider*> mColliders;

	//�ݒu�Փ˂Ŏg�p����������W
	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	//�ۉe
	int mImgShadow;

	//�t���[���Ԃ̈ړ��l
	VECTOR mMoveDiff;
	

	//���[�v�O�̘f����
	Stage::NAME mPreWarpName;


	//�X�΂̕���
	VECTOR mSlopeDir;

	//�X�Ίp
	float mSlopeAngleDeg;

	//�X�΂̈ړ���
	VECTOR mSlopePow;

	//���A�v���C���[���ݒu���Ă���|���S���̖@��
	VECTOR mHitNormal;

	//�ڒn�_
	VECTOR mHitPos;

	//�e
	std::vector<PlayerShot*> mShots;

	//�e�̔��ˊԊu
	float mStepDelayShot;

	// ��ԑJ��
	void ChangeState(STATE state);

	//�Փ˔���p�̊֐�
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);

	//���@�̒e�𔭎�
	void CreateShot(void);
};

