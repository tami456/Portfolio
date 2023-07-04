#pragma once
#include <vector>
#include "SceneBase.h"
class Stage;
class SpaceDome;
class PlayerShip;
class Turret;
class RockManager;
class Rock;

class GameScene : public SceneBase
{

public:
	//���@�j�󎞋�̃��X�^�[�g����
	static constexpr float TIME_RESTART = 2.0f;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	Stage* mStage;
	SpaceDome* mSpaceDome;
	PlayerShip* mPlayerShip;
	Turret* mTurret;
	//�w�i�̊�
	RockManager* mRockManager;

	//���@�̔j�󉉏o����
	float mStepShipdestroy;
};
