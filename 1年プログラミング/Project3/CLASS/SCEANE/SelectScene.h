#pragma once
#include "SCN_ID.h"
#include "../CharaImage.h"
#include "../COMMON/Vector2.h"
#include "Player.h"
class SelectScene
{
public:
	SelectScene();
	~SelectScene();
	bool Init(void);
	bool Release(void);
	SCN_ID Update(char* keyData, char* keyDataOld);
	void Draw(void);
	void SelectDraw(void);
	CharID GetSelPlayer(int no);

private:
	CharImage charImage_[static_cast<int>(CharID::MAX)];
	int selectImage;		//�L�����̑I�����S
	int bgImage;			//�w�i
	Vector2 screenSize_;	//��ʂ̉𑜓x
	Player player_[PLAYER_MAX];
	int gameScreenID_;
	int screenID_;

};

