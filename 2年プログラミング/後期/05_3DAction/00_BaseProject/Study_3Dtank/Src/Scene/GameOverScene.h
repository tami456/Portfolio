#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;


class GameOverScene:public SceneBase
{
public:

	// �R���X�g���N�^
	GameOverScene(void);

	// �f�X�g���N�^
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �摜
	int mImgBG;
	int mImgPush;
	int mImgGameOver;

	// �w�i
	SkyDome* mSkyDome;

	// �f��
	Transform mPlanet;
	Transform mMovePlanet;

	// �L�����N�^�[
	Transform mChara;

	// �A�j���[�V����
	AnimationController* mAnimationController;
};

