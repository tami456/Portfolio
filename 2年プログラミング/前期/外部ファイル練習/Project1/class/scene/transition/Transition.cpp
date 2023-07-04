#include "Transition.h"
#include "../../../_debug/_debug/_DebugDispOut.h"
#include "../../../_debug/_debug/_DebugConOut.h"

Transition::Transition(uniquScene beforScene, uniquScene afterScene, int screenSizeX, int screenSizeY):BaseScene(screenSizeX,screenSizeY)
{
	TRACE("Transition�̐���\n");
	//SceneMng��Update�ɐ���
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);

}

Transition::~Transition()
{
	
}

bool Transition::Init(void)
{
	return true;
}

uniquScene Transition::Update(uniquScene ownScene)
{
	if (UpdateTransition())
	{
		//�����������std::move
		//�I������̂ŃA�t�^�[�V�[��
		return std::move(afterScene_);
	}
	//ownScene�ɓ����Ă�����̂͏��ł���
	//���A���beforScene�����ł���
	return ownScene;
}


