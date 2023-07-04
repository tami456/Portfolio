#include <dxlib.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "../Input/Keyboard.h"
#include "../Input/Gamepad.h"
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
	titleObj.imageObj = ImageLoader().execute("data/image/title.png");
	titleObj.pos = (lpSceneMng.GetScreenSize() - Vector2D{640,480})/2;
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
		return std::make_unique<GameScene>();
	}
	//�^�C�g���V�[���������Ă��郆�j�[�N�|�C���^���A��
	return std::move(ownScene);
}

void TitleScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	//DrawString(0, 16, "TitleScene��Draw()�Ăяo��", 0xffffff);
	titleObj.Draw();
	if ((blinkCount++ / 30) % 2 == 0)	//�O���Z�㉉�Z�ɍ��͂Ȃ�
	{
		int length = GetDrawStringWidth("�o�k�d�`�r�d  'C'�@�j�d�x", 33);
		DrawString((640 - length) / 2, 480 - 150,
			"�o�k�d�`�r�d  'C'�@�j�d�x",
			0xff00ff);
	}
}

