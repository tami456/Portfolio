#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Common/SpriteAnimator.h"
#include "EventShot.h"


EventShot::EventShot(const Transform* parent):ShotBase(parent)
{
}

EventShot::~EventShot(void)
{
}

void EventShot::Update(void)
{

	// ���e���̃G�t�F�N�g
	mExplosion->Update();

	// �����`�F�b�N����������
	if (!IsAlive())
	{
		return;
	}

	// �ړ�����
	Move();

	//�J�����̎���O�`�F�b�N
	VECTOR pos = ConvWorldPosToScreenPos(mTransform.pos);
	

	if (!(pos.z > 0.0f && pos.z < 1.0f))
	//if (CheckCameraViewClip(mTransform.pos))
	{
		//1:(0,100,-498)
		//2:(0,90,-19)
		//�J�����̎���O�ɓ��B������

		//�J�����ɉf��悤�Ɉʒu��i�s�����̌��Ɉړ�������
		VECTOR revDir = VScale(mDir, -1.0f);
		VECTOR velocity = VScale(revDir, 100.0f);
		VECTOR newPos = VAdd(mTransform.pos, velocity);
		mTransform.pos = newPos;
		mTransform.Update();

		//�����G�t�F�N�g�̊J�n
		CreateExplosion(nullptr);

		//�J������h�炷
		SceneManager::GetInstance().GetCamera()->ChangeMode(
			Camera::MODE::SHAKE
		);
	}

	// ���f������̊�{���X�V
	mTransform.Update();

}

float EventShot::GetSpeed(void) const
{
    return 20.0f;
}

float EventShot::GetTimeAlive(void) const
{
    return 10.0f;
}
