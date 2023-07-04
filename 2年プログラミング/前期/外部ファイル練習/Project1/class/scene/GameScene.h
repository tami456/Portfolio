#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"
#include "TmxObj.h"
#include "Obj/Obj.h"


class GameScene :
    public BaseScene
{
public:
	GameScene(int screenSizeX, int screenSizeY);
	~GameScene();
private:
	bool Init(void) override;
	uniquScene Update(uniquScene ownScene) override;
	void DrawOwnScreen(void)override;
	TmxObj tmxObj_;

	//���̃|�C���^�Ɣ�ׂĂ����x�͕ς��Ȃ�
	//shared_ptr�͎኱�x��
	//unique_ptr�ōςނƂ����unique_ptr��
	ObjList objList_;

	//�f�[�^���h���I�ƒu�������Ƃ�std::vector
	//std::list���g�������b�g�F�v�f�������Ă��锠�A���̔���R�Â��ĘA�g������
	//�v�f��ς������Ƃ�move���g��Ȃ��čς�(�R�Â���؂�΂悢)
	//�f�����b�g�F�����_���A�N�Z�X���ł��Ȃ��O���炩��납�炩�r������A�N�Z�X�ł��Ȃ�
	//�Ώ��@�FPlayer����ɗ���悤�Ƀ\�[�g����΂悢
};

