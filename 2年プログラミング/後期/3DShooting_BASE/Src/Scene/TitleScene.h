#pragma once
#include "SceneBase.h"
class SpaceDome;
class ParticleGenerator;
class TextScroll;

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
	void DrawLogo(void);
	void Release(void) override;

private:

	// �^�C�g�����S
	int mImgTitleLogo;

	//�X�J�C�h�[��
	SpaceDome* mSpaceDome;

	//�p�[�e�B�N���G�t�F�N�g
	ParticleGenerator* mParticleGenerator;

	TextScroll* mTextScroll;
};

