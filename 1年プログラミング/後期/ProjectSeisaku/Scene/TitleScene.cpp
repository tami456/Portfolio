#include <dxlib.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "../Input/Keyboard.h"
#include "../common/Frame.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init(void)
{
	sH = LoadSoundMem("data/bgm/Comical_Cats.mp3");
	volume = 100;
	sH2 = LoadSoundMem("data/bgm/������ ���ʉ� �V�X�e��48.mp3");
	vol2 = 120;
	titleObj.imageObj = ImageLoader().execute("data/image/title.png");
	ChangeVolumeSoundMem(vol2, sH2);
	ChangeVolumeSoundMem(volume, sH);
	PlaySoundMem(sH, DX_PLAYTYPE_LOOP);
	
	return true;
}

uniqueScene TitleScene::Update(uniqueScene ownScene)
{
	controller_->Update(*ownScene.get());
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	//DrawString(0, 0, "TitleScene��Update()�Ăяo��", 0xffffff);

	DrawOwnScreen();
	auto contData = controller_->GetInputData();
	if (contData[NowFlame][static_cast<int>(InputID::Btn3)] && !contData[OldFlame][static_cast<int>(InputID::Btn3)])
	{
		//ownScene.reset(new GameScene());
		//return std::move(ownScene);

		//�Q�[���V�[���������Ă��郆�j�[�N�|�C���^���Ԃ�B
		//TitleScene���j�����ꂽ�̂ŁA~TitleScene���Ă΂��B
		StopSoundMem(sH);
		PlaySoundMem(sH2, DX_PLAYTYPE_NORMAL);
		return std::make_unique<GameScene>();
	}
	//�^�C�g���V�[���������Ă��郆�j�[�N�|�C���^���A��
	return std::move(ownScene);
}

void TitleScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	DrawString(0, 0, "TitleScene��Draw()�Ăяo��", 0xffffff);
	titleObj.Draw();
	if ((blinkCount++ / 30) % 2 == 0)	//�O���Z�㉉�Z�ɍ��͂Ȃ�
	{
		int length = GetDrawStringWidth("�o�k�d�`�r�d  'C'�@�j�d�x", 33);
		DrawString((640 - length) / 2, 480 - 150,
			"�o�k�d�`�r�d  'C'�@�j�d�x",
			0xff00ff);
	}
}

