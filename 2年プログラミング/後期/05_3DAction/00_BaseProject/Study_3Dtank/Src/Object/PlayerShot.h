#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "Common/Transform.h"
class AnimationController;

class PlayerShot
{
public:
	PlayerShot(const Transform* parent);
	~PlayerShot();
	void Init(void);

	void Create(VECTOR birthPos, VECTOR dir);

	void Update(void);

	// �����`�F�b�N����������
	bool CheckAlive(void);

	void Move(void);
	void Draw(void);
	void Release(void);

	// �����G�t�F�N�g����
	void NonAlive(void);

	// �e�̑��x
	float GetSpeed(void) const;

	// ��������
	float GetTimeAlive(void) const;

	// ��������
	bool IsAlive(void) const;

	// ���W�̎擾
	Transform& GetTransform(void);

	// ���̓����蔻��p�̔��a���擾
	float GetCollisionRadius(void) const;

protected:

	// ���f������̊�{���
	Transform mTransform;

	AnimationController* mExplosion;

	// ���˕���
	VECTOR mDir;

	// ��������
	bool mIsAlive;

	// ��������
	float mStepAlive;

	// �����蔻��̔��a
	float mCollisionRadius;

private:

	// �e�̕W�����x
	static constexpr float DEFAULT_SPEED = 35.0f;

	// �e�̐�������
	static constexpr float DEFAULT_TIME_ALIVE = 5.0f;

};

