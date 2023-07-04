#pragma once
#include "../Common/Transform.h"
class EventShot;
class Turret;
class SpriteAnimator;
class Camera;

class BossShip
{

public:
	static constexpr float SPEED_MOVE = 4.0f;

	//���e��̃C�x���g�ҋ@����
	static constexpr float TIME_EVENT = 4.0f;

	static constexpr float SHOT_MOVE = 2.0f;

	//�t�F�[�h����
	static constexpr float TIME_FADE = 8.0f;

	//�t�F�[�h�̊J�n�E�I�����̐F
	static constexpr COLOR_F FADE_FROM = {1.0f,1.0f,1.0f,1.0f};
	static constexpr COLOR_F FADE_TO = {0.8f,0.1f,0.1f,0.0f};

	//�ȉ~�̔��a
	static constexpr float EX_RADIUS_X = 800.0f;
	static constexpr float EX_RADIUS_Y = 800.0f;
	static constexpr float EX_RADIUS_Z = 2000.0f;

	//�����Ԋu
	static constexpr float TIME_EXPLOSION = 0.2f;

	//���
	enum class STATE
	{
		NONE,
		EVENT,
		BATTLE,
		DESTROY,
		END
	};

	// �R���X�g���N�^
	BossShip(void);

	// �f�X�g���N�^
	~BossShip(void);
	void Init(void);
	void Update(void);
	void UpdateTurret(void);
	void Draw(void);
	void DrawTurret(void);
	void Release(void);

	//3D����̊�b����ǂݎ���p�Ŏ擾
	const Transform& GetTransform(void) const;

	//��������
	bool IsAlive(void) const;

	//�^���b�g
	const std::vector<Turret*>& GetTurrets(void) const;

	//�{�X�̔j�󔻒�
	bool IsDestroy(void) const;

	//�{�X���o�I������
	bool IsEnd(void) const;
private:

	//�C�x���g�p�̒e����
	EventShot* mEventShot;

	Camera* mCamera;

	//3D���f���̊�{���
	Transform mTransform;

	//�^���b�g
	std::vector<Turret*> mTurrets;

	//���
	STATE mState;

	//���Ԍv��
	float mStepEvent;
	float mStepFade;
	float mStepExplosion;

	//�^���b�g�̑S�Ńt���O
	bool mIsAllDestroyTurrets;

	//�j�󎞂̔����G�t�F�N�g
	std::vector<SpriteAnimator*> mExplosions;


	//��ԑJ��
	void ChangeState(STATE state);

	//�^���b�g�̍쐬
	void MakeTurret(VECTOR localPos,VECTOR localAddAxis);

	//�{�X�j�󎞂̉��o(����)�쐬
	void CreateRandomExplosion(void);

	//�����G�t�F�N�g��Z�\�[�g
	void ZSortExplosion(void);
};

