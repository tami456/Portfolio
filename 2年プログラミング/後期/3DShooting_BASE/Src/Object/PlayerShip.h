#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"
class ParticleGenerator;
class SpriteAnimator;
class SpeechBalloon;
class PlayerShot;

class PlayerShip
{
public:

	//���
	enum class STATE
	{
		NONE,
		RUN,		//�ʏ�(���s)���
		DESTROY		//�j����
	};


	static constexpr float SPEED_MOVE = 10.0f;

	//���@�̉�]��
	static constexpr float SPEED_ROT_Y = DX_PI_F/180.0f * 1.0f;
	static constexpr float SPEED_ROT_X = DX_PI_F/180.0f * 1.0f;

	//�Փ˔���p�̋��̔��a
	static constexpr float COLLISION_RADIUS = 20.0f;

	//�Փ˔���p�̋��̔��a(�e)
	static constexpr float SHOT_COLLISION_RADIUS = 10.0f;

	//�u�[�X�g�̊Ԋu
	static constexpr float TIME_DELAY_BOOST = 3.0f;

	//�u�[�X�g���̉����X�s�[�h
	static constexpr float SPEED_BOOST = 6.0f;

	//�u�[�X�g�̌���
	static constexpr float SUB_SPEED_BOOST = 2.0f;
	
	//�e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	// �R���X�g���N�^
	PlayerShip(void);

	// �f�X�g���N�^
	~PlayerShip(void);

	void Init(void);
	void InitEffect(void);
	void Update(void);
	void UpdateRun(void);
	void UpdateDestroy(void);
	void Draw(void);
	void Release(void);

	//����
	void ProcessTurn(void);
	void ProcessBoost(void);
	void ProcessShot(void);

	void Turn(double deg,VECTOR axis);


	const Transform& GetTransform(void) const;

	//���@�j��
	void Destroy(void);

	//���@���j�󂳂�Ă��邩�m�F
	bool IsDestroy(void);

	//�����o���N���X�̎擾
	SpeechBalloon* GetSpeechBalloon(void) const;
 
	//�e�̎擾
	std::vector<PlayerShot*>& GetShots(void);
private:

	//���
	STATE mState;

	//�p�[�e�B�N���G�t�F�N�g
	ParticleGenerator* mParticleGenerator;

	//���f����3D����
	Transform mTransform;

	//�����G�t�F�N�g
	SpriteAnimator* mExplosion;

	//�����o��
	SpeechBalloon* mSpeechBalloon;


	//�e
	std::vector<PlayerShot*> mShots;

	//�e�̔��ˊԊu
	float mStepDelayShot;

	//�����G�t�F�N�g(Effekseer)
	int mExplosion3D;
	int mExplosion3DPlay;

	//���˃G�t�F�N�g
	int mEffectJet;
	int mEffectJetPlayL;
	int mEffectJetPlayR;

	//�u�[�X�g�G�t�F�N�g
	float mStepDelayBoost;
	float mSpeedBoost;
	int mEffectBoost;
	int mEffectBoostPlay;

	void ChangeState(STATE state);

	//�G�t�F�N�g����
	void SyncJetEffect(void);

	//�u�[�X�g�G�t�F�N�g����
	void SyncBoostEffect(void);

	//���@�̒e�𔭎�
	void CreateShot(void);
};

