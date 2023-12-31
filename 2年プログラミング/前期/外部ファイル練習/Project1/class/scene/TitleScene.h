#pragma once
#include "BaseScene.h"


class TitleScene :
    public BaseScene
{
public:
	TitleScene(int screenSizeX, int screenSizeY);
	~TitleScene();
private:
	//publicに置いてしまうと派生で宣言して使えてしまう
	bool Init(void) override;
	uniquScene Update(uniquScene ownScene) override;
	void DrawOwnScreen(void)override;

};

