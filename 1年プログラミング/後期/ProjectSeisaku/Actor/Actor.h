#pragma once
#include <memory>
#include <list>
#include "../common/Vector2D.h"
#include "ActorType.h"
#include "State.h"
#include "../common/AnimeCtl.h"

class BaseScene;
class Actor;

using sharedActor = std::shared_ptr<Actor>;
using ActorList = std::list<sharedActor>;

class Actor
{
public:
	
	Actor(ActorType type, Vector2D pos);
	virtual ~Actor();
	virtual void Update(BaseScene& scene) = 0;
	void Draw(void);
	void LoadAnime(void);
	const Vector2D& GetPos(void) { return pos_; }
	const Vector2D GetSize(void) { return size_; }
	const Vector2D GetFootPos(void) { return animeCtl_.GetFootPos(); }
	void SetCount(int count) { count_ = count; }
	int GetCount() { return count_; }
	bool IsAlive(void) { return alive_; }
	void SetActorType(ActorType type) { type_ = type; }
	ActorType GetActorType(void) { return type_; }
	void SetStateType(State state) { state_ = state; }
	State GetStateType(void) { return state_; }
	int GetScore() { return score_; }
private:
	
protected:
	Vector2D pos_;		//�A�N�^�[�̍��W
	Vector2D size_;
	int speed_;			//�A�N�^�[�̃X�s�[�h
	bool alive_;		//�A�N�^�[�̐����t���O �n�ʂɗ�����A�܂��͉�ʊO�ɓ���������false
	int count_;			//�A�N�^�[�̐�������
	int score_;			//�A�N�^�[�����X�R�A
	ActorType type_;
	State state_;
	AnimeCtl animeCtl_;

	friend class TitleScene;
	friend class GameScene;
};

