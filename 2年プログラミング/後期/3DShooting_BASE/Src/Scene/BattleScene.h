#pragma once
#include "SceneBase.h"
class SpaceDome;
class BossShip;
class RockManager;
class PlayerShip;
class Turret;


class BattleScene : public SceneBase
{
public:
	//���@�j�󎞋�̃��X�^�[�g����
	static constexpr float TIME_RESTART = 2.0f;

	//�{�X�j�󂩂�̃��X�^�[�g����
	static constexpr float TIME_RESTART_BOSS_DESTROY = 5.0f;

	// �R���X�g���N�^
	BattleScene(void);

	// �f�X�g���N�^
	~BattleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//�w�i��
	RockManager* mRockManager;
	
	//�X�J�C�h�[��
	SpaceDome* mSpaceDome;

	//�{�X�V�b�v
	BossShip* mBossShip;

	//���@
	PlayerShip* mPlayerShip;

	Turret* mTurret;

	//���@�̔j�󉉏o����
	float mStepShipDestroy;

	//�Q�[���N���A�摜
	int mImgEndLogo;

	//�{�X�j���̉��o����
	float mStepBossDestroy;
};

