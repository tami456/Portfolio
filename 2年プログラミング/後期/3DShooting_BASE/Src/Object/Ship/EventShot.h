#pragma once
#include "ShotBase.h" 
class Transform;
class EventShot : public ShotBase
{
public:
	//�R���X�g���N�^
	EventShot(const Transform* parent);

	//�f�X�g���N�^
	~EventShot(void);

	//�X�V
	void Update(void) override;

	//�e�̑��x
	float GetSpeed(void) const override;

	// ��������
	float GetTimeAlive(void) const override;


};

