#pragma once
#include <memory>
#include <dxlib.h>
#include "../common/Vector2D.h"

class Actor;

class Camera
{
public:
	Camera();
	~Camera();
	bool Init(const Vector2D& worldSize);
	void Update(void);
	bool ReConnect(std::weak_ptr<Actor> actor);
	const Vector2D& GetPos(void) const { return pos_; }	//関数名 const…中で変数を変更することができない。constで呼べる
	
private:
	std::weak_ptr<Actor> target_;
	Vector2D pos_;
	RECT moveArea_;
};

