#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "../../Object/Common/Transform.h"
class SpriteAnimator;

class ShotBase
{

public:

	// �R���X�g���N�^�EInit�ECreate�̋�ʂ���₱�����̂ŁA
	// Init����
	ShotBase(const Transform* parent);
	virtual ~ShotBase(void);

	/// <summary>
	/// �e�̐���
	/// </summary>
	/// <param name="birthPos">�������W</param>
	/// <param name="dir">�i�s����</param>
	virtual void Create(VECTOR birthPos, VECTOR dir);

	virtual void Update(void);

	// �����`�F�b�N����������
	virtual bool CheckAlive(void);

	virtual void Move(void);
	virtual void Draw(void);
	virtual void Release(void);

	// �����G�t�F�N�g����
	virtual void CreateExplosion(const Transform* transform);

	// �e�̑��x
	virtual float GetSpeed(void) const;

	// ��������
	virtual float GetTimeAlive(void) const;

	// ��������
	bool IsAlive(void) const;

	// ���W�̎擾
	VECTOR GetPos(void) const;

	// ���̓����蔻��p�̔��a���擾
	float GetCollisionRadius(void) const;

protected:

	// �e�̔����G�t�F�N�g
	SpriteAnimator* mExplosion;

	// ���f������̊�{���
	Transform mTransform;

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
