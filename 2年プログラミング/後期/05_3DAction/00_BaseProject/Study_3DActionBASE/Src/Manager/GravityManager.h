#pragma once
#include "../Object//Common/Transform.h"
#include "../Common/Quaternion.h"
class SceneManager;
class Planet;
class Player;
class Enemy;

class GravityManager
{

public:

	// �d�͕��������S�ɕς��܂ł̎���
	static constexpr float TIME_CHANGE_DIR = 1.0f;

	// ��]��
	static constexpr float DEFAULT_POW_SLERP = 0.08f;

	// ���
	enum class STATE
	{
		IDLE,
		STAGE_CHANGE
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static GravityManager& GetInstance(void);

	void Init(void);

	void Update(void);
	void UpdateIdle(void);
	void UpdateChangeStage(void);

	// �d�͕�����A�������̌v�Z
	void Calculate(void);

	// ���݃v���C���Ă���f��
	Planet* GetActivePlanet(void) const;

	// ���݃v���C���Ă���f����ύX����
	void ChangeActivePlanet(Planet* planet);

	// ����v���C���[���Z�b�g
	void SetPlayer(Player* player);
	void SetEnemy(Enemy* enemy);

	const Transform& GetTransform(void) const;
	
	// �d�͕������擾
	VECTOR GetDirGravity(void) const;

	// �d�͕����̔��Ε������擾
	VECTOR GetDirUpGravity(void) const;

	// �d�͂̋������擾
	float GetPower(void) const;

	// �f���؂�ւ��O�̉�]���
	Quaternion GetPremQuaRot(void) const;

	// �v���C���[�Ƒ����Փ˂����@��
	void SetLastHitNormal(const VECTOR& normal);

	// �v���C���[�Ƒ����Փ˂����ʒu
	void SetLastHitPos(const VECTOR& pos);
	VECTOR GetLastHitPos(void) const;

protected:

	// �ÓI�C���X�^���X
	static GravityManager* mInstance;

	// �v���C���[
	Player* mPlayer;
	Enemy* mEnemy;

	// ���݃v���C���Ă���f��
	Planet* mActivePlanet;

	Transform mTransform;

	// ���
	STATE mState;

	// �d�͕���
	VECTOR mDirGravity;
	VECTOR mPreDirGravity;

	// �d�͕����̔��Ε���
	VECTOR mDirUpGravity;

	// �v���C���[�Ƒ����Փ˂����@��
	VECTOR mLastHitNormal;

	// �v���C���[�Ƒ����Փ˂����ʒu
	VECTOR mLastHitPos;

	// �X�V�X�e�b�v
	float mStep;

	// ��]��
	double mSlerpPow;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	GravityManager(void);
	~GravityManager(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �d�͕������v�Z
	VECTOR CalcDirGravity(void) const;

};

