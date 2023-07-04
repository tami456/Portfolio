#pragma once
#include <list>
#include<memory>
#include "Actor.h"
#include "../Input/Controller.h"
#include "../common/Vector2D.h"
#include "../common/ImageCtl.h"



class Player:
	public Actor
{
public:
	Player(ActorType type, Vector2D pos);
	~Player();

private:
	void Init(void);
	void Update(BaseScene& scene)override;
	std::list<bool (Player::*)(const ActorList& actList,std::weak_ptr<tmxObj>)> moveFuncList_;
	bool CheckMove(Vector2D& pos,Vector2D&& offset,std::weak_ptr<tmxObj> tmxObj);	//�E�Ɖ��͕��⍂��������̂ł߂肱��
	//�E�Ӓl�Q�Ɓ����̏�ō���ď�������́�����
	//���Ӓl�Q�Ɓ��ϐ���錾���āA���̒��ɑ�����遨��

	//bool (Player::*moveFunc_)(void);
	bool MoveLeft(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool MoveRight(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool MoveUp(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool MoveDown(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool Idle(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool SetBomb(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool Dead(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool CheckHit(const ActorList& actList);
	ActorType CheckSideActor(const ActorList& actList,Vector2D offset);
	std::unique_ptr<Controller> cont_;		//Updata���Ăׂ΁A�L�[�����擾�ł���
};

