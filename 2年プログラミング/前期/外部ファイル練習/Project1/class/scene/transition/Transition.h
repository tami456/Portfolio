#pragma once
#include "../BaseScene.h"
class Transition :
	public BaseScene
{
public:
	//new old before after
	//��납�O�ɏ������͎���ɍ��킹��

	Transition(uniquScene beforScene , uniquScene afterScene , int screenSizeX, int screenSizeY);
	~Transition();
private:
	bool Init(void) override;
	uniquScene Update(uniquScene ownScene) override;
	//�h���ł����g��Ȃ�
	virtual bool UpdateTransition(void) = 0;

protected:
	//befor��after��Scene�̓R���X�g���N�^����o��Ə��ł��Ă��܂�
	uniquScene beforScene_;
	uniquScene afterScene_;
};

