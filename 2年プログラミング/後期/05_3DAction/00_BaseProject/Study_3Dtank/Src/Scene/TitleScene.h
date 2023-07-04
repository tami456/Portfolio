#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �摜
	int mImgTitle;
	int mImgPush;

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
