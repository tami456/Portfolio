#pragma once
#include "SceneBase.h"
class SpaceDome;
class BossShip;

class EventScene : public SceneBase
{

public:

	// �R���X�g���N�^
	EventScene(void);

	// �f�X�g���N�^
	~EventScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//�X�J�C�h�[��
	SpaceDome* mSpaceDome;

	//�{�X�V�b�v
	BossShip* mBossShip;
};

