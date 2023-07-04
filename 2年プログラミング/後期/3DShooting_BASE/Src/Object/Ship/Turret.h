#pragma once

#include <vector>
#include "ShotBase.h" 
#include "../Common/Transform.h"

class TurretShot;
class SpriteAnimator;

class Turret
{
public:

	//�Փ˔���(���̂̔���)
	static constexpr float COLLISION_RADIUS = 200.0f;

	//��_���[�W�G�t�F�N�g
	static constexpr float TIME_DAMAGED_EFFECT = 2.0f;

	//�C��̃��[�J����]�ғ��p
	static constexpr float ANGLE_Y_MIN_STAND = -30 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_STAND = 30 * (DX_PI_F / 180.0f);

	//�C��̃��[�J����]��
	static constexpr float ANGLE_POW_STAND = 0.2f;

	//�C�g�̃��[�J����]�ғ��p
	static constexpr float ANGLE_Y_MIN_GUN = -10 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_GUN = 20 * (DX_PI_F / 180.0f);

	//�C�g�̃��[�J����]��
	static constexpr float ANGLE_POW_GUN = 0.2f;

	// �^���b�g�̑傫��
	static constexpr float SCALE = 80.0f;

	//�e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;
	// ���
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY
	};

	// �R���X�g���N�^
	Turret(
		const Transform* transformParent,
		VECTOR localPos,
		VECTOR localAddAxis);

	// �f�X�g���N�^
	~Turret(void);

	void Init(void);

	void Update(void);
	void UpdateAttack(void);
	void UpdateDestroy(void);
	void UpdateShot(void);
	

	void Draw(void);
	void DrawAttack(void);
	void DrawDestroy(void);
	void DrawShot(void);

	void Release(void);

	// �{�X���(�e���f��)�Ɖ�]�ƈʒu�̓���
	void SyncParent(Transform& transform,VECTOR addAxis);

	// �e�̎擾
	std::vector<TurretShot*>& GetShots(void);

	//��������
	bool IsAlive(void) const;

	//���W�̎擾
	VECTOR GetPos(void) const;

	//�^���b�g�Ƀ_���[�W��^���鏈��
	void Damage(void);

private:

	ShotBase* mShotBase;

	//�e
	std::vector<TurretShot*> mShots;

	// �{�X�̊�{���
	const Transform* mTransformParent;

	//���g�̔����G�t�F�N�g��
	SpriteAnimator* mExplosion;


	// ���f������̊�{���(�C��)
	Transform mTransformStand;

	// ���f������̊�{���(�C�g)
	Transform mTransformGun;

	// ���
	STATE mState;

	// ��͂���̑��΍��W
	VECTOR mLocalPos;

	// �ǉ��̃��[�J����]
	VECTOR mLocalAddAxis;

	// �X�ɒǉ��̖C�g���[�J����]
	VECTOR mLocalAddAxisStand;

	// �X�ɒǉ��̖C�g���[�J����]
	VECTOR mLocalAddAxisGun;

	// �ϋv��
	int mHp;

	//�C��̃��[�J���ғ���(deg)
	float mAnglePowStand;

	//�C�g�̃��[�J���ғ���(deg)
	float mAnglePowGun;

	//�e�̔��ˊԊu
	float mStepDelayShot;

	//��_���[�W�G�t�F�N�g�̎��Ԍv���p
	float mStepDamaged;

	// ��ԑJ��
	void ChangeState(STATE state);

	void CreateShot(VECTOR birthPos, VECTOR dir);
	
};

