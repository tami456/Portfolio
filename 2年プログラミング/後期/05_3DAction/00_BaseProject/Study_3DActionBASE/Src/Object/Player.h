#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"
class ResourceManager;
class GravityManager;
class AnimationController;
class Collider;
class Capsule;
class PlayerShot;
class SpeechBalloon;


class Player
{
public:

	//�ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;
	static constexpr float SPEED_FLY = 30.0f;

	//�e�̐�������
	static constexpr float ALIVE_SHOT = 10.0f;

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//�W�����v��
	static constexpr float POW_JUMP = 35.0f;

	//�W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	//���G�t�F�N�g�̔����Ԋu
	static constexpr float TIME_FOOT_SMOKE = 0.3f;

	//�O�ՃG�t�F�N�g�̉�]
	static constexpr float TIME_ORBIT = 0.3f;

	//�e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
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
		KICK
	};

	Player(void);
	~Player(void);

	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);
	void UpdateWarpReserve(void);
	void UpdateWarpMove(void);
	void UpdateDead(void);

	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);

	void Release(void);

	Transform* GetTransform(void);
	Capsule* GetCapsule(void);

	//����
	void ProcessMove(void);
	void ProcessJump(void);
	void ProcessShot(void);

	//��](�U����������p�x�����߂�)
	void SetGoalRotate(float rotRad);

	//��](�S�[���ɑ΂��āA���ʕ⊮���s��)
	void Rotate(void);

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

	//���[�v�̊J�n
	void StartWarp(
		float time, 
		const Quaternion& goalRot,
		const VECTOR& goalPos);

	//�O�ՃG�t�F�N�g�̒�~
	void StopOrbitEffect(void);

	//�v���C���[�̏�Ԕ��f
	bool IsPlay(void);
	bool IsWarpMove(void);
	bool IsDead(void);

	//�e�̎擾
	std::vector<PlayerShot*>& GetShots(void);

	//�����o���N���X�̎擾
	SpeechBalloon* GetSpeechBalloon(void) const;



private:

	ResourceManager& mResourceManager;
	GravityManager& mGravityManager;

	Transform mTransform;

	std::string msg;

	// �A�j���[�V����
	AnimationController* mAnimationController;

	//�X�s�[�`�o���[��
	SpeechBalloon* mSpeechBalloon;

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

	//�W�����v����
	bool mIsJump;

	//�W�����v�̓��͎�t����
	float mStepJump;

	//�Փ˔���ɗp������R���C�_
	Capsule* mCapsule;
	std::vector<Collider*> mColliders;

	//�ݒu�Փ˂Ŏg�p����������W
	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	//�ۉe
	int mImgShadow;

	//�����̃G�t�F�N�g
	int mEffectSmoke;
	float mStepFootSmoke;
	int mPlayEffectSmoke;


	//�t���[���Ԃ̈ړ��l
	VECTOR mMoveDiff;
	
	//���[�v�p
	float mTimeWarp;
	float mStepWarp;

	//���[�v�����������̏��
	Quaternion mWarpQua;
	VECTOR mWarpReservePos;

	//���[�v�����������̃v���C���[���
	Quaternion mReserveStartQua;
	VECTOR mReserveStartPos;

	//���[�v�O�̘f����
	Stage::NAME mPreWarpName;

	//�O�ՃG�t�F�N�g
	int mEffectWarpOrbit;

	int mPlayOrbitEffectL;
	int mPlayOrbitEffectR;

	//�v���C���[�̎�(�t���[���ԍ�)
	int mFrameLeftHand;
	int mFrameRightHand;

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

	//���n���[�V�����̏I������
	bool IsEndLanding(void);

	//�����G�t�F�N�g
	void EffectFootSmoke(void);

	//�O�ՃG�t�F�N�g
	void EffectWarpOrbit(void);
	void SyncWarpOrbitPos(void);

	//���@�̒e�𔭎�
	void CreateShot(void);
};

