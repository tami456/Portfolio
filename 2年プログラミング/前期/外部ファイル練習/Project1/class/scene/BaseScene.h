#pragma once
#include <memory>
#include "../Vector2D.h"
#include "../../class/ImageMng.h"

//�N���X�錾
//�G�N�X�^�[���c�ォ��o�Ă����Ƃ����錾
class BaseScene;
using uniquScene = std::unique_ptr<BaseScene>;
 
class BaseScene
{
public:
	BaseScene(int screenSizeX,int screenSizeY);
	virtual ~BaseScene();
	virtual bool Init(void) = 0;
	//ownScene = �������guniquScene
	virtual uniquScene Update(uniquScene ownScene) = 0;

	//�ŗL�̕����Ƃ�������Ȃ������ŕ�����
	virtual void Draw(void);

private:
protected:
	//����̃R���X�g���N�^�ɗv��Ȃ�
	virtual void DrawOwnScreen(void) = 0;
	int screenID_;
	//const��t����Ɛ��l���ς���Ȃ��Ȃ�
	const Vector2D screenSize_;
	Vector2D drawOffset_;
};

