#include <stdlib.h>
#include <time.h>
#include <DxLib.h>
#include "main.h"

#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"


SCN_ID scnID;		//���쒆�̃V�[��ID
int blinkCount;
int spaceKeyFlag;		//���݃t���[���̃X�y�[�X�L�[�̏��
int spaceKeyFlagOld;	//�P�t���[���O�̃X�y�[�X�L�[�̏��

//�^�C�g���֘A
int titleLogoImage;
int titleLogoPosX;
int titleLogoPosY;


//�Q�[���I�[�o�[�֘A
int gameOverLogoImage;
int gameOverLogoPosX;
int gameOverLogoPosY;

int gameScreen;		//�Q�[���G���A�p�X�N���[���n���h��
int scrCount[SCR_MAX];



//[]����ƈꎟ���z��ƌ��� //�񎟌��z��
//���@�֘A
int playerImage[DIR_MAX][ANIM_MAX];
int playerPosX;
int playerPosY;
int playerSpeed;
int playerAnimCount;
int playerLife;
bool playerFlag;	
DIR_ID playerDir;

//���@�̒e�֘A
int pShotImage;
int pShotPosX;
int pShotPosY;
int pShotSpeed;
bool pShotFlag;

//�G�֘A
int enemyImage[ENEMY_TYPE_MAX][DIR_MAX][ANIM_MAX];
int enemyPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];
int enemyPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];
int enemySpeed;
int enemyAnimCount;
DIR_ID enemyDir[ENEMY_COUNT_Y][ENEMY_COUNT_X];
bool enemyFlag[ENEMY_COUNT_Y][ENEMY_COUNT_X];

int ufoPosX;
int ufoPosY;
int ufoSpeed;
int ufoDir;
bool ufoFlag;

MOVE_LR enemyMoveFlagX;								//�G�̍��E�ړ��Ǘ��p
MOVE_MODE enemyMoveMode;
int enemyMoveLineX;			//�G�̍��E�ړ��Ǘ��p
int enemyMoveLineY;			//�G�̉��ړ��Ǘ��p
int enemyMoveCount;			//�G�̈ړ��p�J�E���^�[

//�G�̒e�֘A
int eShotImage;							//�G�̒e�̉摜ID
int eShotPosX[ENEMY_COUNT_X];			//�G�̒e��X���W
int eShotPosY[ENEMY_COUNT_X];			//�G�̒e��Y���W
int eShotSpeed;							//�G�̒e�̈ړ����x
bool eShotFlag[ENEMY_COUNT_X];			//�G�̒e�̏��

int effectImage;
int effectPosX;
int effectPosY;
int effectCount;

unsigned int frame;

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	if (SysInit() == false)
	{
		//�������Ɏ��s���Ă�����v���O�������I������
		TRACE("�V�X�e���n�������Ɏ��s\n");
		return -1;
	}
	if (Init() == false)
	{
		//�������Ɏ��s���Ă�����v���O�������I������
		TRACE("�������Ɏ��s\n");
		return -1;
	}

	//�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();
		GetKeyState();

		switch (scnID)
		{
		case SCN_ID_TITLE:
			TitleScene();
			break;
		case SCN_ID_GAME:
			GameScene();
			break;
		case SCN_ID_GAME_OVER:
			GameOverScene();
			break;
		default:
			//���Ȃ��͂�
			TRACE("�V�[��ID�ُ�\n");
			break;
		}
		//if (scnID == SCN_ID_TITLE)
		//{
		//	TitleScene();
		//}
		//else if(scnID == SCN_ID_GAME)
		//{
		//	GameScene();
		//}
		//else if(scnID == SCN_ID_GAME_OVER)
		//{
		//	GameOverScene();
		//}
		//else
		//{
		//	//���Ȃ��͂�
		//	TRACE("�V�[��ID�ُ�\n");
		//}
	}

	DxLib_End();
	return 0;
}

void TitleScene(void)
{
	if (spaceKeyFlag == 1 && spaceKeyFlagOld == 0)
	//if (spaceKeyFlag && ~spaceKeyFlagOld)
	//if (spaceKeyFlag && !spaceKeyFlagOld)
	//if (spaceKeyFlag & ~spaceKeyFlagOld)
	//if (spaceKeyFlag & (spaceKeyFlag ~ spaceKeyFlagOld)
	{
		scnID = SCN_ID_GAME;
	}
	TitleDraw();
}

void GameScene(void)
{
	PlayerReProc();
	EnemyCtr();
	PlayerCtr();
	ShotCtr();
	CheckHitObj();
	GameDraw();
	frame++;
}

void GameOverScene(void)
{
	
	if (spaceKeyFlag == 1 && spaceKeyFlagOld == 0)
	{
		scnID = SCN_ID_TITLE;
		Init();
	}
	GameOverDraw();
}

void EnemyCtr(void)
{
	if (effectCount > 0)
	{
		effectCount--;
	}
	enemyAnimCount++;
	if (frame <= 60 * 3)
	{
		return;
	}
	enemyMoveCount++;
	if ((enemyMoveCount%4) != 0) //4�̔{���̎�������������
	{
		return;
	}

	if (enemyMoveMode == MOVE_MODE_LR)
	{
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			if (enemyMoveFlagX == MOVE_RIGHT)
			{
				enemyPosX[y][(ENEMY_COUNT_X - 1) - enemyMoveLineX] += enemySpeed;
				enemyDir[y][(ENEMY_COUNT_X - 1) - enemyMoveLineX] = DIR_RIGHT;
			}
			else
			{
				enemyPosX[y][enemyMoveLineX] -= enemySpeed;
				enemyDir[y][enemyMoveLineX] = DIR_LEFT;
			}
			
		}
		enemyMoveLineX++;
		if (enemyMoveLineX >= ENEMY_COUNT_X)
		{
			enemyMoveLineX = 0;

			for (int y = 0; y < ENEMY_COUNT_Y; y++)
			{
				for (int x = 0; x < ENEMY_COUNT_X; x++)
				{
					if (enemyFlag[y][x])
					{
						if (enemyMoveFlagX == MOVE_RIGHT)
						{
							
							if (enemyPosX[y][x] >= GAME_SCREEN_SIZE_X - CHIP_SIZE_X)
							{
								enemyMoveFlagX = MOVE_LEFT;
								enemyMoveMode = MOVE_MODE_DOWN;
							}
						}
						if (enemyMoveFlagX == MOVE_LEFT)
						{
							
							if (enemyPosX[y][x] <= 0)
							{
								enemyMoveFlagX = MOVE_RIGHT;
								enemyMoveMode = MOVE_MODE_DOWN;
							}
						}
					}
				}
			}
		}
		
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (!eShotFlag[x])
			{
				//�m���Œe�������ǂ���
				if (rand() % 300 == 0)
				{
					//�����Ă���G��T��
					for (int y = ENEMY_COUNT_Y - 1; y >= 0; y--)
					{
						
						if (enemyFlag[y][x])
						{
							eShotPosX[x] = enemyPosX[y][x] + (CHIP_SIZE_X / 2 - ESHOT_SIZE_X / 2);
							eShotPosY[x] = enemyPosY[y][x] + CHIP_SIZE_X / 3;
							enemyDir[y][x] = DIR_DOWN;
							eShotFlag[x] = true;
							break;
						}
					}
				}
			}
		}
		
	}
	else if(enemyMoveMode == MOVE_MODE_DOWN)
	{
		
		//for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				enemyPosY[(ENEMY_COUNT_Y - 1) - enemyMoveLineY][x] += (CHIP_SIZE_Y + (CHIP_SIZE_Y / 2));
				enemyDir[(ENEMY_COUNT_Y - 1) - enemyMoveLineY][x] = DIR_DOWN;
			}
		}
		enemyMoveLineY++;
		if (enemyMoveLineY >= ENEMY_COUNT_Y)
		{
			enemyMoveLineY = 0;
			enemyMoveMode = MOVE_MODE_LR;
		}
	}
	else
	{
		TRACE("enemyMoveMode�̒l�ُ�%d\n", enemyMoveMode);
	}

	ufoPosX += ufoSpeed;
	if (ufoFlag)
	{
		if (rand() % 300 == 0)
		{
			if (ufoPosX > GAME_SCREEN_SIZE_X)
			{
				ufoPosX = 0 - CHIP_SIZE_X;
			}
		}
	}
	
}


//int playerMoveCount = (GAME_SCREEN_SIZE_X - CHIP_SIZE_X) / 2;

void PlayerCtr(void)
{
	/*playerMoveCount += playerSpeed;
	playerPosX = abs((playerMoveCount % (GAME_SCREEN_SIZE_X - CHIP_SIZE_X) * 2) - (GAME_SCREEN_SIZE_X - CHIP_SIZE_X));
	TRACE("%d\n", playerPosX);*/

	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		
		if (playerPosX - playerSpeed >= 0)
		{
			playerDir = DIR_LEFT;
			playerPosX -= playerSpeed;
			
		}
		else
		{
			playerPosX = 0;
		}
	}

	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		
		if (playerPosX + playerSpeed < GAME_SCREEN_SIZE_X - CHIP_SIZE_X)
		{
			playerDir = DIR_RIGHT;
			playerPosX += playerSpeed;
		}
		else
		{
			playerPosX = GAME_SCREEN_SIZE_X - CHIP_SIZE_X;
		}
	}
	if (frame >= 60 * 3)
	{
		if (CheckHitKey(KEY_INPUT_Z))
		{
			if (!pShotFlag)
			{
				pShotFlag = true;
				pShotPosX = playerPosX + (CHIP_SIZE_X - PSHOT_SIZE_X) / 2;
				pShotPosY = playerPosY;
				playerDir = DIR_UP;
			}
		}
	}
	playerAnimCount++;
}

void ShotCtr(void)
{
	if (pShotFlag)
	{
		pShotPosY += pShotSpeed;
		if (pShotPosY < -PSHOT_SIZE_Y)
		{
			pShotFlag = false;
		}
	}
	
	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotFlag[x])
		{
			if (eShotFlag[x])
			{
				eShotPosY[x] += eShotSpeed;
			}
			if (eShotPosY[x] > GAME_SCREEN_SIZE_Y)
			{
				eShotFlag[x] = false;
			}
		}
	}
	
}

void CheckHitObj(void)
{
	//enemyPosX[0][0];	//�G�̍�
	//enemyPosX[0][0];	//�G�̉E
	//enemyPosY[0][0];	//�G�̏�
	//pShotPosX;		//�e�̍�
	//pShotPosY;		//�e�̏�
	//enemyFlag[0][0];

	if (pShotFlag)
	{
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				if (enemyFlag[y][x])
				{
					if (pShotPosX < enemyPosX[y][x] + CHIP_SIZE_X &&	//�e�̍��@�G�̉E
						enemyPosX[y][x] < pShotPosX + PSHOT_SIZE_X &&	//�G�̍��@�e�̉E
						pShotPosY < enemyPosY[y][x] + CHIP_SIZE_Y &&	//�e�̏�@�G�̉�
						enemyPosY[y][x] < pShotPosY + PSHOT_SIZE_Y		//�G�̏�@�e�̉�
						)
					{
						effectPosX = enemyPosX[y][x] - (EFFECT_SIZE_X - CHIP_SIZE_X) / 2;
						effectPosY = enemyPosY[y][x] - (EFFECT_SIZE_Y - CHIP_SIZE_Y) / 2;
						effectCount = 30;

						enemyFlag[y][x] = false;
						pShotFlag = false;
						scrCount[SCR_PL1] += ENEMY_SCR;
						scrCount[SCR_HIGH] = scrCount[SCR_PL1] > scrCount[SCR_HIGH] ? scrCount[SCR_PL1] : scrCount[SCR_HIGH];
			
						//TRACE("�e�ƓG����������\n");
					}
				}
			}
		}
	}	

	if (pShotFlag)
	{
		if (ufoFlag)
		{
			if (pShotPosX < ufoPosX + CHIP_SIZE_X &&	//�e�̍��@�G�̉E
				ufoPosX < pShotPosX + PSHOT_SIZE_X &&	//�G�̍��@�e�̉E
				pShotPosY < ufoPosY + CHIP_SIZE_Y &&	//�e�̏�@�G�̉�
				ufoPosY < pShotPosY + PSHOT_SIZE_Y		//�G�̏�@�e�̉�
				)
			{
				ufoPosX = 0 - CHIP_SIZE_X;

				ufoFlag = false;
				pShotFlag = false;
				scrCount[SCR_PL1] += UFO_SCR;
			}
		}
	}




	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotFlag[x] && playerFlag)
		{
			if (playerPosX < eShotPosX[x] + ESHOT_SIZE_X &&	//���@�̍��@  �G�̒e�̉E
				eShotPosX[x] < playerPosX + CHIP_SIZE_X &&	//�G�̒e�̍��@���@�̉E
				playerPosY < eShotPosY[x] + ESHOT_SIZE_Y &&	//���@�̏�@  �G�̒e�̉�
				eShotPosY[x] < playerPosY + CHIP_SIZE_Y		//�G�̒e�̏�@���@�̉�
				)
			{
				PlayerDeathProc(false);
				
			}
		}
	}
	if (enemyMoveMode == MOVE_MODE_LR)
	{
		for (int y = ENEMY_COUNT_Y - 1; y >= 0; y--)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				if (enemyFlag[y][x])
				{
					if (enemyPosY[y][x] >= DEF_LINE_Y)
					{
						PlayerDeathProc(true);
					}
				}
			}
		}
	}
}

void PlayerDeathProc(bool backFlag)
{
	if (!playerFlag)
	{
		return;
	}
	frame = 0;
	playerLife--;
	playerFlag = false;
	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		eShotFlag[x] = false;
	}

	if (playerLife <= 0)
	{
		playerLife = 0;
		//�Q�[���I�[�o�[�����Ɉڍs
		scnID = SCN_ID_GAME_OVER;
		
	}

	if (backFlag)
	{
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				enemyPosY[y][x] -= (CHIP_SIZE_Y + (CHIP_SIZE_Y / 4)) * 3;
			}
		}
	}
}

void PlayerReProc(void)
{
	if (playerFlag)
	{
		return;
	}
	playerPosX = (GAME_SCREEN_SIZE_X - CHIP_SIZE_X) / 2;
	playerFlag = true;
}

void TitleDraw(void)
{
	SetDrawScreen(gameScreen);
	ClsDrawScreen();
	DrawBox(0, 0, GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y, 0x303030, true);
	DrawGraph(titleLogoPosX, titleLogoPosY,titleLogoImage,true);

	if ((blinkCount++ / 30) % 2 == 0)	//�O���Z�㉉�Z�ɍ��͂Ȃ�
	{
		int length = GetDrawStringWidth("�o�k�d�`�r�d  '�r�o�`�b�d'�@�j�d�x", 33);
		DrawString((GAME_SCREEN_SIZE_X - length) / 2, (GAME_SCREEN_SIZE_Y - TITLE_LOGO_IMAGE_SIZE_Y) / 4 + TITLE_LOGO_IMAGE_SIZE_Y + 70,
			"�o�k�d�`�r�d  '�r�o�`�b�d'�@�j�d�x",
			0xffffff);
	}

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawTexts();
	DrawGraph(GAME_SCREEN_X, GAME_SCREEN_Y, gameScreen, true);
	_dbgDraw();
	ScreenFlip();
}
void GetKeyState(void)
{
	spaceKeyFlagOld = spaceKeyFlag;
	spaceKeyFlag = CheckHitKey(KEY_INPUT_SPACE);
}
//�`�揈��
void GameDraw(void)
{
	
	SetDrawScreen(gameScreen);
	ClsDrawScreen();
	DrawBox(0, 0, GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y, 0x303030, true);
	//DrawBox(0, 0, GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y, GetColor(100,100,100), true);
	
	DrawLine(0, DEF_LINE_Y,GAME_SCREEN_SIZE_X, DEF_LINE_Y,0xffffff,true);

	
	
	//����񕪂̏������c�ɕK�v�Ȑ������[�v������
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		//����񕪕`�悷��
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyFlag[y][x])
			{
			/*	if (enemyMoveMode == MOVE_MODE_LR)
				{
					if (enemyMoveFlagX == MOVE_RIGHT)
					{
						no = DIR_RIGHT;
					}
					else
					{
						no = DIR_LEFT;
					}
				}
				else
				{
					no = DIR_DOWN;
				}*/
				DrawGraph(enemyPosX[y][x], enemyPosY[y][x],
						enemyImage[y % ENEMY_NOMAL_MAX][enemyDir[y][x]][abs(((enemyAnimCount / 10) % (ANIM_MAX + 1)) - (ANIM_MAX - 1))], true);
				TRACE("%d\n", abs(((enemyAnimCount / 10) % (ANIM_MAX + 1)) - (ANIM_MAX - 1)));
			}
		}
	}

	if (ufoFlag)
	{
		DrawGraph(ufoPosX, ufoPosY, enemyImage[ENEMY_TYPE_UFO][ufoDir][abs(((enemyAnimCount / 10) % (ANIM_MAX + 1)) - (ANIM_MAX - 1))], true);
	}
		

	
	if (pShotFlag)
	{
		DrawGraph(pShotPosX, pShotPosY, pShotImage, true);
	}

	if (effectCount)
	{
		DrawGraph(effectPosX, effectPosY, effectImage, true);
	}

	if (playerFlag)
	{
		DrawGraph(playerPosX, playerPosY, playerImage[playerDir][abs(((playerAnimCount / 10) % (ANIM_MAX + 1)) - (ANIM_MAX - 1))], true);
	}
	//TRACE("%d\n", abs(((playerAnimCount / 10) % (ANIM_MAX + 1)) - (ANIM_MAX - 1)));
	

	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotFlag[x])
		{
			DrawGraph(eShotPosX[x], eShotPosY[x], eShotImage, true);
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	for (int no = 0; no < playerLife - 1; no++)
	{
		DrawGraph(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + no * (CHIP_SIZE_X + 5),
			GAME_SCREEN_SIZE_Y + GAME_SCREEN_Y - CHIP_SIZE_Y * 2, 
			playerImage[DIR_DOWN][0], true);
	}
	DrawTexts();



	DrawGraph(GAME_SCREEN_X, GAME_SCREEN_Y, gameScreen, true);
	_dbgDraw();
	ScreenFlip();
}

void GameOverDraw(void)
{
	SetDrawScreen(gameScreen);
	ClsDrawScreen();
	DrawBox(0, 0, GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y, 0x303030, true);
	DrawGraph(gameOverLogoPosX, gameOverLogoPosY, gameOverLogoImage, true);

	if ((blinkCount++ / 30) % 2 == 0)	//�O���Z�㉉�Z�ɍ��͂Ȃ�
	{
		int length = GetDrawStringWidth("�o�k�d�`�r�d  '�r�o�`�b�d'�@�j�d�x", 33);
		DrawString((GAME_SCREEN_SIZE_X - length) / 2, (GAME_SCREEN_SIZE_Y - TITLE_LOGO_IMAGE_SIZE_Y) / 4 + TITLE_LOGO_IMAGE_SIZE_Y + 70,
			"�o�k�d�`�r�d  '�r�o�`�b�d'�@�j�d�x",
			0xffffff);
	}

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawTexts();
	DrawGraph(GAME_SCREEN_X, GAME_SCREEN_Y, gameScreen, true);
	_dbgDraw();
	ScreenFlip();
}

void DrawTexts(void)
{
	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X , GAME_SCREEN_Y ,"�g�h�f�g",0xffffff);

	int len = GetDrawStringWidth("�r�b�n�q�d ",12);
	DrawString(SCREEN_SIZE_X - len, GAME_SCREEN_Y + CHIP_SIZE_Y / 2, "�r�b�n�q�d",0xffffff);
	
	if (scrCount[SCR_HIGH] >= 100000)
	{
		DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + CHIP_SIZE_X * 2,
			GAME_SCREEN_Y + CHIP_SIZE_Y, 0xffffff, "99999", scrCount[SCR_HIGH]);
	}
	else
	{
		DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + CHIP_SIZE_X * 2,
			GAME_SCREEN_Y + CHIP_SIZE_Y, 0xffffff,"%d", scrCount[SCR_HIGH]);
	}

	//int drawScr = scrCount[SCR_HIGH];
	//if (drawScr >=100000)
	//{
	//	drawScr = 99999;
	//}
	//else
	//{
	//	drawScr = scrCount[SCR_HIGH];
	//}
	//�O�����Z�q
	//�������H�l�P�F�l�Q
	//int drawscr = scrCount[SCR_HIGH] >= 100000 ? 99999 : scrCount[SCR_HIGH];

	//len = GetDrawFormatStringWidth("%d  ", drawscr);
	//DrawFormatString(SCREEN_SIZE_X - len, GAME_SCREEN_Y + 45, 0xffffff, "%d", drawscr);

	int lan = GetDrawStringWidth("�P�t�o�@�@  �@", 16);
	DrawString(SCREEN_SIZE_X - lan, GAME_SCREEN_Y + GAME_SCREEN_Y, "�P�t�o", 0xffffff);
	DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + CHIP_SIZE_X * 2,
		GAME_SCREEN_Y + GAME_SCREEN_Y + CHIP_SIZE_Y / 2, 0xffffff, "%d",scrCount[SCR_PL1]);
	
}


//�������֐�
bool Init(void)
{
	//�ϐ�������
	//�v���C���[
	
	blinkCount = 0;
	scrCount[SCR_PL1] = 0;

	playerPosX = (GAME_SCREEN_SIZE_X - CHIP_SIZE_X) / 2;
	playerPosY = GAME_SCREEN_SIZE_Y - CHIP_SIZE_Y;
	playerAnimCount = 0;
	playerLife = 3;
	playerDir = DIR_UP;
	playerFlag = true;

	pShotPosX = 0;
	pShotPosY = GAME_SCREEN_SIZE_Y;
	
	pShotFlag = false;

	//�G
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		//����񕪕`�悷��
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			enemyPosX[y][x] = x * (CHIP_SIZE_X * 2);
			enemyPosY[y][x] = (y + 1) * (CHIP_SIZE_Y + (CHIP_SIZE_Y / 4));
			enemyDir[y][x] = DIR_DOWN;
			enemyFlag[y][x] = true;

		}
	}
	
	enemyAnimCount = 0;
	enemyMoveFlagX = MOVE_RIGHT;
	enemyMoveMode = MOVE_MODE_LR;
	enemyMoveLineX = 0;
	enemyMoveLineY = 0;
	enemyMoveCount = 0;

	ufoPosX = 0 - CHIP_SIZE_X;
	ufoPosY = 0;
	ufoSpeed = 3;
	ufoDir = DIR_RIGHT;
	ufoFlag = true;

	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		eShotPosX[x] = x * (CHIP_SIZE_X * 2);
		eShotPosY[x] = 0;
		eShotFlag[x] = false;
	}
	

	frame = 0;
	//�����G�t�F�N�g
	effectPosX = 0;
	effectPosY = 0;
	effectCount = 0;

	return true;
}

bool SysInit(void)
{
	SetWindowText("2116020_���c��");

	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	srand((unsigned int)time(NULL));
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	gameScreen = MakeScreen(GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y, true);



	titleLogoImage = LoadGraph("image/TitleLogo2.png");
	TRACE("titleLogoImage:%d\n", titleLogoImage);

	gameOverLogoImage = LoadGraph("image/GameOver.png");
	TRACE("gameOverLogoImage:%d\n", gameOverLogoImage);

	LoadDivGraph("image/Female 05-2.png",
		DIR_MAX* ANIM_MAX,
		ANIM_MAX, DIR_MAX,
		CHIP_SIZE_X, CHIP_SIZE_Y,
		&playerImage[0][0]);	//playerImage[0]);�ł��ʂ�@���Ӎ��͂܂��g��Ȃ�
//�ꎟ���z��̎��͖��O�����ł����@playerImage);
	for (int y = 0; y < DIR_MAX; y++)
	{
		for (int x = 0; x < ANIM_MAX; x++)
		{
			//*(playerImage+3)	//()...�I�y���[�^�[
			//playerImage[0][3]
			TRACE("playerImage:%d\n", playerImage[y][x]);
		}
	}

	pShotImage = LoadGraph("image/pshot.png");
	TRACE("pShotImage:%d\n", pShotImage);

	//const...read only...���������ł��Ȃ�
	//char*...�������錾����Ƃ��Ɏg���@�������̃T�C�Y�͂����Ŋm�肷��
	const char* enemyFileName[ENEMY_TYPE_MAX] = {
		"image/Enemy 15-3.png",
		"image/Enemy 15-2.png",
		"image/Enemy 15-4.png",
		"image/Enemy 15-5.png",
		"image/Enemy 15-6.png",
		"image/Enemy 15-1.png",
		"image/Enemy 09-1.png"
	};

	//�K��񐔕��J��Ԃ�����
	for (int no = 0; no < 7; no++)
	{
		LoadDivGraph(enemyFileName[no],
			DIR_MAX * ANIM_MAX,
			ANIM_MAX, DIR_MAX,
			CHIP_SIZE_X, CHIP_SIZE_Y,
			&enemyImage[no][0][0]);
		for (int y = 0; y < DIR_MAX; y++)
		{
			for (int x = 0; x < ANIM_MAX; x++)
			{
				TRACE("enemyImage:%d\n", enemyImage[0][y][x]);
			}
		}
	}
	eShotImage = LoadGraph("image/eshot.png");
	TRACE("eShotImage:%d\n", eShotImage);

	effectImage = LoadGraph("image/baku.png");
	TRACE("effectImage:%d\n", effectImage);

	scnID = SCN_ID_TITLE;
	
	spaceKeyFlag = 0;
	spaceKeyFlagOld = 0;

	scrCount[SCR_HIGH] = 20;

	titleLogoPosX = (GAME_SCREEN_SIZE_X - TITLE_LOGO_IMAGE_SIZE_X) / 2;
	titleLogoPosY = (GAME_SCREEN_SIZE_Y - TITLE_LOGO_IMAGE_SIZE_Y) / 3;

	gameOverLogoPosX = (GAME_SCREEN_SIZE_X - GAME_OVER_LOGO_IMAGE_SIZE_X) / 2;
	gameOverLogoPosY = (GAME_SCREEN_SIZE_Y - TITLE_LOGO_IMAGE_SIZE_Y) / 3;

	playerSpeed = 2;
	pShotSpeed = PSHOT_SPEED;
	enemySpeed = ENEMY_SPEED;
	eShotSpeed = ESHOT_SPEED;

	return true;
}
