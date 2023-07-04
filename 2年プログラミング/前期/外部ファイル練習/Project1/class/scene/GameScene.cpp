#include "TitleScene.h"
#include "GameScene.h"
#include <DxLib.h>
#include "TmxObj.h"
#include "Obj/Player.h"
#include "Obj/Hadouken.h"
#include "Obj/SonicBoom.h"
#include "Obj/StateMng.h"
#include "transition/CrossOver.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "../../_debug/_debug/_DebugConOut.h"


//�f�t�H���g�R���X�g���N�^���Ȃ�����G���[���o��
//�R���X�g���N�^�̌��BaseScene���������Ƃɂ���ăG���[�͖����Ȃ�
GameScene::GameScene(int screenSizeX, int screenSizeY):BaseScene(screenSizeX,screenSizeY)
{
	TRACE("GameScene�̐���\n");
	Init();
	DrawOwnScreen();
	
}

GameScene::~GameScene()
{
	TRACE("GameScene�̔j��\n");
	
}

bool GameScene::Init(void)
{
	StateMng::Create();
	lpStateMng.Load("./data/Player.xml", "Player");
	lpStateMng.Load("./data/Hadouken.xml","Hadouken");
	lpStateMng.Load("./data/SonicBoom.xml", "SonicBoom");
	tmxObj_.LoadTMX("./tmx/map.tmx");
	//���݂���v�f�̌��ɒǉ����Ă���
	//�C�e���[�^�[�F�R���e�i�ɃA�N�Z�X����|�C���^�݂����Ȃ��
	//�s��������
	
	//obj.h��tmxObj_�̎Q�Ƃ���
	objList_.emplace_back(std::make_unique<Player>(tmxObj_,objList_));
	
	
	return true;
}

uniquScene GameScene::Update(uniquScene ownScene)
{
	//�������o���Ă���̂��킩��Ȃ��ꍇauto���g����������������
	/*std::list<int> intList;
	auto num = intList.begin();*/
	//��뉉�Z�͏����x��
	//��뉉�Z��5�s��������Ă���
	//��ׂđO���Z��2�s�ōς�ł���
	//�C�e���[�^�[���g���Ƃ��͋C��t������������
	//�C�e���[�^�[�ɑ΂���><�͎g���Ȃ�
	for (auto obj = objList_.begin(); obj != objList_.end(); ++obj)
	{
		//()��t���ėD��x���グ��
		(*obj)->Update();
	}

	//�����ƍ폜���܂Ƃ߂Ă���Ă����
	//�����_�ł���
	//����ł��邩�ǂ����̊m�F
	//�����Ă���̂��m�F�������ꍇ��isAlive�ł�����
	objList_.remove_if([](UniqueObj& obj) {return obj->isDeth(); });

	DrawOwnScreen();
	//if (CheckHitKey(KEY_INPUT_SPACE))
	//{
	//	//������ӂ̐�����TitleScene�ɏ����Ă���
	//	return std::make_unique<TitleScene>(screenSize_.x, screenSize_.y);
	//}
	
	return ownScene;
}

void GameScene::DrawOwnScreen(void)
{

	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto worldArea_ = tmxObj_.GetWorldArea();
	auto tileSize_ = tmxObj_.GetTileSize();
	
	
	for (const auto& layer: tmxObj_.GetMapData())
	{
		for (int y = 0; y < worldArea_.y; y++)
		{
			for (int x = 0; x < worldArea_.x; x++)
			{

				//auto mapData = tmxObj_.GetMapData().at("block")[x + y * worldArea_.x];
				//const�������Ȃ���const���O��Ă��܂�

				//0���Ԃ��Ă�����A�N�Z�X���Ȃ�
				//1��������A�N�Z�X����
				if (x + y * worldArea_.x < layer.second.size())
				{
					//gid�͂����܂�id�Ȃ̂ŉ摜�ł͂Ȃ�
					auto gid = layer.second[x + y * worldArea_.x];
					if (gid >= 0)
					{
						//TMX�̕��ŕԂ�l�������������������
						DrawGraph(
							x * tileSize_.x,
							y * tileSize_.y,
							lpImageMng.GetID(tmxObj_.GetMapKey())[gid],
							true);
					}
				}
				
			}
		}
	}
	//���̏ꏊ���ꏏ�ɕς��Ȃ��Ƃ����Ȃ�����Q�Ɠn��
	for (auto& obj: objList_)
	{
		obj->Draw();
	}
}
