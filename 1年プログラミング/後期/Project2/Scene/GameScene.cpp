#include <dxlib.h>
#include "GameScene.h"
#include "ResultScene.h"
#include "../Actor/Actor.h"
#include "../Actor/Player.h"
#include "../Actor/Enemy.h"
#include "../Actor/Bomb.h"
#include "../Input/Keyboard.h"
#include "../Input/Gamepad.h"
#include "../common/Frame.h"
#include "MapID.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

bool GameScene::Init(void)
{
	next_ = false;
	SetDrawScreen(screenID_);
	tmxObj_ = std::make_shared<tmxObj>();		//���̂����A�V�K�I�u�W�F�N�g�����
	//tmxObj_->LoadTSX("data/tmx/map.tsx");
	tmxObj_->LoadTMX("data/tmx/map.tmx");

	fireMap_ = std::make_shared<FireMap>(tmxObj_->GetWorldArea(), tmxObj_->GetTileSize());

	auto create = [&](MapID mapID,ActorType actorType) 
	{
		auto PosList = tmxObj_->SearchID(mapID, "block");
		for (auto pos:PosList)
		{
			if (CreateActor(actorType, pos))
			{
				tmxObj_->SetMapData(-1, "block", pos.x, pos.y);
			}
		}
	};

	viewArea_ = { 640,480 };

	create(MapID::Player, ActorType::Player);
	create(MapID::Enemy1, ActorType::Enemy1);
	create(MapID::Enemy2, ActorType::Enemy2);
	//CreateActor<Bomb>(ActorType::Bomb, { 10,10 }, true);
	//�T�����APlayer������������J������ReConnect���ĂсAPlayer���J�����̃^�[�Q�b�g�ɂ���
	for (auto actor : actorList_)
	{
		if (actor->type_ == ActorType::Player)
		{
			camera_.ReConnect(actor);
			camera_.Init(tmxObj_->GetWorldArea() * tmxObj_->GetTileSize());
			break;
		}
	}
	int gameSizeY = tmxObj_->GetWorldArea().y * tmxObj_->GetTileSize().y;
	drawOffset_.y = (viewArea_.y - gameSizeY) / 2;
	//�C�ӂ�id��map�������
	//for (int y = 0; y < tmxObj_->GetWorldArea().y; y++)
	//{
	//	for (int x = 0; x < tmxObj_->GetWorldArea().x; x++)
	//	{
	//		tmxObj_->SetMapData(-1, "block",x,y);			
	//	}
	//}
	TRACE("PLAYER��Dead�ŃV�[���J��\n");
	return true;
}

uniqueScene GameScene::Update(uniqueScene ownScene)
{
	controller_->Update(*ownScene.get());
	for (auto& actor : actorList_)		//���̂��Q�Ƃō��
	{
		actor->Update(*this);
		if (actor->IsBomb().first)
		{
			CreateActor<Bomb>(ActorType::Bomb, actor->IsBomb().second,false);
		}
	}

	camera_.Update();
	//�����o�[�֐�
	//remove_if
	//�R���e�i�̓��e������


	/*auto itr = std::remove_if(actorList_.begin(),actorList_.end(),
		[](const sharedActor& actor) {
			return !actor->IsAlive();
		});
	actorList_.erase(itr, actorList_.end());*/

	actorList_.remove_if(
		[&](const sharedActor& actor) {
			if (!actor->IsAlive() && 
				actor->GetActorType() == ActorType::Player)
			{
				next_ = true;
			}
			return !actor->IsAlive();
		});
	
	fireMap_->Update(tmxObj_);
	
	_dbgDrawBox(220, 0, 400, 16, 0, true);
	_dbgDrawFormatString(220, 0, 0xffffff, "ActorList�̓o�^��:%d", actorList_.size());
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawOwnScreen();
	
	//auto contData = controller_->GetInputData();
	//if (contData[NowFlame][static_cast<int>(InputID::Btn3)] && !contData[OldFlame][static_cast<int>(InputID::Btn3)])
	//{
	//	//ownScene.reset(new GameScene());
	//	//return std::move(ownScene);

	//	//�Q�[���V�[���������Ă��郆�j�[�N�|�C���^���Ԃ�B
	//	//TitleScene���j�����ꂽ�̂ŁA~TitleScene���Ă΂��B
	//	return std::make_unique<ResultScene>();
	//}
	if (next_)
	{
		return std::make_unique<ResultScene>();
	}
	_dbgDrawBox(110, 0, 210, 18, 0, true);
	_dbgDrawFormatString(110,0,0xffffff,"X:%d,Y:%d",camera_.GetPos().x, camera_.GetPos().y);

	return std::move(ownScene);

}

void GameScene::DrawOwnScreen(void)
{
	//���炷�͈�-�J�����̍��W
	auto offset = (viewArea_ / 2) - camera_.GetPos();
	

	SetDrawScreen(screenID_);
	const Vector2D& worldArea = tmxObj_->GetWorldArea();

	//�}�b�v�̕`��
	for (auto layerID : tmxObj_->GetMapData())
	{
		for (int y = 0; y < worldArea.y; y++)
		{
			for (int x = 0; x < worldArea.x; x++)
			{
				tmxObj_->DrawMapData(layerID.first, x, y, offset);
			}
		}
	}
	//Actor�֘A�̕`��
	
	//Z�\�[�g
	actorList_.sort([](sharedActor actor1,sharedActor actor2) {
		return actor1->GetFootPos().y < actor2->GetFootPos().y;
		});

	
	//tmxObj_->Draw();
	for (auto& actor : actorList_)		//���̂��Q�Ƃō��
	{
		actor->Draw(offset);
	}

	//���̕`��
	for (int y = 0; y < worldArea.y; y++)
	{
		for (int x = 0; x < worldArea.x; x++)
		{
			fireMap_->Draw(tmxObj_,x, y, offset,drawOffset_);
		}
	}
	
}

bool GameScene::CreateActor(ActorType type, Vector2D pos)
{
	switch (type)
	{
	case ActorType::Player:
		CreateActor<Player>(type,pos,false);
		break;
	case ActorType::Enemy1:
	case ActorType::Enemy2:
		CreateActor<Enemy>(type, pos,false);	
		break;
	case ActorType::Bomb:
		CreateActor<Bomb>(type, pos, false);
		break;
	default:
		TRACE("type�ُ�");
		return false;
		//break;
	}
		//auto actor = (--actorList_.end());
		//(*actor)->LoadAnime();
		//(*actor)->animeCtl_.reConnect(&(*actor)->pos_);
	
	return true;
}
