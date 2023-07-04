#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"
class ResourceManager;
class GravityManager;
class AnimationController;
class Collider;
class Capsule;
class Player;

class Enemy
{
public:
	//�ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	static constexpr float POS_RADIUS = 20.0f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		IDLE,
		WARP_RESERVE,
		WARP_MOVE,
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
		DEAD
	};

	Enemy(Player* player);
	~Enemy();

	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);
	void UpdateIdle(void);

	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);


	void Release(void);

	Transform& GetTransform(void);
	Capsule* GetCapsule(void);
	float GetCollisionRadius(void) const;

	//��������
	bool IsAlive(void) const;

	//��](�U����������p�x�����߂�)
	void SetGoalRotate(float rotRad);

	//��](�S�[���ɑ΂��āA���ʕ⊮���s��)
	void Rotate(void);

	//����
	void ProcessMove(void);

	//�d�͂̌v�Z
	void CalcGravityPow(void);

	//�X�΂̌v�Z
	void CalcSlope(void);

	//�Փ˔���ɗp������R���C�_
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	//�G�Ƀ_���[�W�����
	void Damage(void);

	bool IsDead(void);

private:
	ResourceManager& mResourceManager;
	GravityManager& mGravityManager;

	Transform mTransform;

	// �A�j���[�V����
	AnimationController* mAnimationController;

	// ���
	STATE mState;

	//�v���C���[�̏��
	Player* mPlayer;

	//�ړ��X�s�[�h
	float mSpeed;

	//�ړ�����
	VECTOR mMoveDir;

	//�ړ���
	VECTOR mMovePow;

	//�ړ���̍��W
	VECTOR mMovedPos;

	VECTOR dir;

	//�ڒn�_
	VECTOR mHitPos;

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

	//�X�΂̕���
	VECTOR mSlopeDir;

	//�X�Ίp
	float mSlopeAngleDeg;

	//�X�΂̈ړ���
	VECTOR mSlopePow;

	//���A�v���C���[���ݒu���Ă���|���S���̖@��
	VECTOR mHitNormal;

	//�t���[���Ԃ̈ړ��l
	VECTOR mMoveDiff;

	//��������
	bool mIsAlive;

	int mHp;

	// ��ԑJ��
	void ChangeState(STATE state);

	//�Փ˔���p�̊֐�
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);
};

