#pragma once
#include <memory>
#include "../common/Vector2D.h"

class Controller;
class BaseScene;		//�N���X�錾�@����o��\��̃N���X��錾����
//std::unique_ptr<BaseScene>��uniqueScene�Ƃ������O�Ŏg��
using uniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	bool Init(void);
	virtual uniqueScene Update(uniqueScene ownScene) = 0;
	virtual void DrawOwnScreen(void) = 0;
	virtual void Draw(void);				//�X�N���[���ɕ`�悷�邾���Ȃ̂ŁA���������ł���
	//�g�����₷���悤��virtual������
private:
	Vector2D screenSize_;
protected:
	Vector2D drawOffset_;
	int screenID_;
	std::unique_ptr<Controller> controller_;
};

