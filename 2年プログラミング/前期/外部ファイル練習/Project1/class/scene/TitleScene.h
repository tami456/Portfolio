#pragma once
#include "BaseScene.h"


class TitleScene :
    public BaseScene
{
public:
	TitleScene(int screenSizeX, int screenSizeY);
	~TitleScene();
private:
	//public�ɒu���Ă��܂��Ɣh���Ő錾���Ďg���Ă��܂�
	bool Init(void) override;
	uniquScene Update(uniquScene ownScene) override;
	void DrawOwnScreen(void)override;

};

