#include <stdlib.h>		
#include<time.h>		
#include <DxLib.h>
#include "main.h"

#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

// �v���C���[�֘A
int playerImage;	//�O���t�B�b�N�n���h��
int playerPosX;		//X���W
int playerPosY;		//Y���W
int playerSpeed;	//�ړ���(�X�s�[�h�j
bool playerFlag;	//��ԃt���O(true:�����@false:���S)

//�G�l�~�[�֘A
int enemyImage;		//�O���t�B�b�N�n���h��
int enemyPosX;		//X���W
int enemyPosY;		//Y���W
int enemySpeed;		//�ړ���(�X�s�[�h)
bool enemyFlag;		//��ԃt���O (true:���� false:���S)

int insekiImage;
int insekiPosX;
int insekiPosY;
int insekiSpeed;
bool insekiFlag;


//�e�֘A
int shotImage;		//�O���t�B�b�N�n���h��
int shotPosX;		//X���W
int shotPosY;		//Y���W
int shotSpeed;		//�ړ���(�X�s�[�h)
bool shotFlag;		//��ԃt���O true:���˒� false:�񔭎˒�

int bgImage;		//�O���t�B�b�N�n���h��
int bgPosY1;		//�w�i1�p��Y���W
int bgPosY2;		//�w�i�Q�p��Y���W
int bgSpeed;		//�ړ���

int kumoImage;
int kumoPosY1;
int kumoPosY2;
int kumoSpeed;

int score;

int zanki;

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	//����������
	SetWindowText("2116020_���c��");

	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
		{
		return -1;
		}

	LoadGraphScreen(0, 0, "image/start.png", TRUE);
	WaitKey();
	
	srand((unsigned int)time(NULL));

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);
	TRACE("DXLIB�̏���������\n");
	//�摜�̓ǂݍ���
	playerImage = LoadGraph("image/player.png");
	TRACE("playerImage = %d\n",playerImage);
	
	enemyImage = LoadGraph("image/enemy.png");
	TRACE("enemyImage = %d\n", enemyImage);

	shotImage = LoadGraph("image/shot.png");
	TRACE("	shotImage = %d\n", shotImage);

	bgImage = LoadGraph("image/haikei.png");
	TRACE("	bgImage = %d\n", bgImage);

	kumoImage = LoadGraph("image/kumo.png");
	TRACE("	kumoImage = %d\n", kumoImage);

	insekiImage = LoadGraph("image/inseki.png");
	TRACE("	insekiImage = %d\n", insekiImage);

	//�ϐ��̏�����
	//�v���C���[
	playerPosX = (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
	playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;
	playerSpeed = 2;
	playerFlag = true;

	//�G
	enemyPosX = (SCREEN_SIZE_X - ENEMY_SIZE_X) / 2;
	enemyPosY = (SCREEN_SIZE_Y - ENEMY_SIZE_Y) / 2;
	enemySpeed = 2;
	enemyFlag = false;

	//覐�
	insekiPosX = (SCREEN_SIZE_X - INSEKI_SIZE_X) / 2;
	insekiPosY = (SCREEN_SIZE_Y - INSEKI_SIZE_Y) / 2;
	insekiSpeed = 1;
	insekiFlag = false;

	//�e
	shotPosX = 0;
	shotPosY = SCREEN_SIZE_Y;
	shotSpeed = -4;
	shotFlag = false;

	//�w�i
	bgPosY1 = 0;
	bgPosY2 = -BG_SIZE_Y;
	bgSpeed = 2;

	kumoPosY1 = 0;
	kumoPosY2 = -KUMO_SIZE_Y;
	kumoSpeed = 4;

	score = 0;

	zanki = 3;


	TRACE("�Q�[�����[�v�J�n\n");

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();
		// playerPosX = playerPosX + 1;
		// playerPosX =+ 1;
		// playerPosX++; //�����Ӗ�
		

		//�w�i�̈ړ�����
		bgPosY1 += bgSpeed;
		bgPosY2 += bgSpeed;
		if (bgPosY1 >= SCREEN_SIZE_Y)
		{
			bgPosY1 = bgPosY2 - BG_SIZE_Y;
		}
		if (bgPosY2 >= SCREEN_SIZE_Y)
		{
			bgPosY2 = bgPosY1 - BG_SIZE_Y;
		}

		kumoPosY1 += kumoSpeed;
		kumoPosY2 += kumoSpeed;
		if (kumoPosY1 >= SCREEN_SIZE_Y)
		{
			kumoPosY1 = kumoPosY2 - KUMO_SIZE_Y;
		}
		if (kumoPosY2 >= SCREEN_SIZE_Y)
		{
			kumoPosY2 = kumoPosY1 - KUMO_SIZE_Y;
		}

		//_dbgDrawBox(0, bgPosY1, 640, bgPosY1 + BG_SIZE_Y, GetColor(255, 0, 0), false);
		//_dbgDrawBox(0, bgPosY2, 640, bgPosY2 + BG_SIZE_Y, GetColor(255, 0, 0), false);
		
		//���X�|�[������
		if (!playerFlag)
		{
			TRACE("�v���C���[�̃��X�|�[������\n");
			playerPosX = (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
			playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;
			playerFlag = true;
		}

		if (!enemyFlag)
		{
			TRACE("�G�̃��X�|�[������\n");
			//rand()��0�`RAND_MAX(32767)�͈̔͂̐��l���擾�ł���
			//���񋁂߂����̂�0�`(SCREEN_SIZE_X - ENEMY_SIZE_X)
			//�����̋N�_�̎��������_���V�[�h�ƌ���
			enemyPosX = rand() % (SCREEN_SIZE_X - ENEMY_SIZE_X);
			enemyPosY = -ENEMY_SIZE_Y;
			enemyFlag = true;
			
		}

		if (!insekiFlag)
		{
			TRACE("覐΂̃��X�|�[������\n");
			insekiPosX = rand() % (SCREEN_SIZE_X - INSEKI_SIZE_X);
			insekiPosY = -INSEKI_SIZE_Y;
			insekiFlag = true;
		}
		
		//�v���C���[�̈ړ�����
		if (playerFlag)
		{
			//�X�N���[���L�[�{�[�h�ł����e���L�[�o���܂���ł���
			if (CheckHitKey(KEY_INPUT_NUMPAD4) || CheckHitKey(KEY_INPUT_LEFT))
				if (playerPosX > 0)
				{
					playerPosX -= playerSpeed;
				}
			if (CheckHitKey(KEY_INPUT_NUMPAD6) || CheckHitKey(KEY_INPUT_RIGHT))
				if (playerPosX < SCREEN_SIZE_X - PLAYER_SIZE_X)
				{
					playerPosX += playerSpeed;
				}
			if (CheckHitKey(KEY_INPUT_NUMPAD8) || CheckHitKey(KEY_INPUT_UP))
				if (playerPosY > 0)
				{
					playerPosY -= playerSpeed;
				}
			if (CheckHitKey(KEY_INPUT_NUMPAD2) || CheckHitKey(KEY_INPUT_DOWN))
				if (playerPosY < SCREEN_SIZE_Y - PLAYER_SIZE_Y)
				{
					playerPosY += playerSpeed;
				}
		}

		_dbgDrawBox(playerPosX + 31, playerPosY + 31, playerPosX, playerPosY, 0xfff, false);
		
		if (shotFlag)
		{
			shotPosY += shotSpeed;
			if (shotPosY < 0)
			{
				shotFlag = false;
			}
		}
		
		
			

		

		//Z�L�[����������A��������
		if (playerFlag)
		{
			if (CheckHitKey(KEY_INPUT_Z)) //CheckHitKey���ɓ��͂���Ə������e�����Ȃ�
			{
				if (!shotFlag)
				{
					//PlaySoundFile("GB-mp3/GB-Shooter01-01(Shoot).mp3", DX_PLAYTYPE_NORMAL);
					//���@�̍��W��e�̍��W�ɑ������
					shotPosX = playerPosX;
					shotPosY = playerPosY;
					//��Ԃ𔭎˒��ɂ���
					shotFlag = true;

					
				}
			}
		}


		

		//�G�l�~�[�̈ړ�����
		
		if (enemyFlag)
		{
			enemyPosY += enemySpeed;
			if (enemyPosY >= SCREEN_SIZE_Y)
			{
				enemyFlag = false;
			}
		}
		_dbgDrawBox(enemyPosX + 63, enemyPosY + 63, enemyPosX, enemyPosY, 0xffff, false);		
		//_dbgDrawFormatString(0, 0, 0xffffff, "�G���W:%d,%d", enemyPosX, enemyPosY);
		//_dbgDrawFormatString(0, 16, 0xffffff, "PL���W:%d,%d", playerPosX, playerPosY);
		
		//覐΂̈ړ�����
		if (insekiFlag)
		{
			insekiPosY += insekiSpeed;
			if (insekiPosY >= SCREEN_SIZE_Y)
			{
				insekiFlag = false;
			}
		}
		_dbgDrawBox(insekiPosX + 127, insekiPosY + 63, insekiPosX, insekiPosY, 0xff00ff, false);
		//�����蔻��
		//���@�ƓG
		if (playerFlag && enemyFlag)
		{
			if (playerPosX  < enemyPosX + ENEMY_SIZE_X - 20 && //���@�̍� < �G�̉E
				enemyPosX  < playerPosX + PLAYER_SIZE_X -20 && //�G�̍� < ���@�̉E
				playerPosY < enemyPosY + ENEMY_SIZE_Y -20 && //���@�̏� < �G�̉�
				enemyPosY < playerPosY + PLAYER_SIZE_Y -20   //�G�̏� < ���@�̉�
				)
			{
				//�@���X�|�[������
				playerFlag = false;
				enemyFlag = false;
				
				
				if (zanki >= 0)
				{
					zanki -= 1;
					if(zanki == -1)
					{
						WaitKey();
						zanki += 4;
						score = 0;
					}
				}
				//TRACE("��������\n");
				//_dbgDrawFormatString(0, 48, 0xffffff,"��������\n");
			}
		}
		//���@��覐�
		if (playerFlag && insekiFlag)
		{
			if (playerPosX < insekiPosX + INSEKI_SIZE_X&& //���@�̍� < 覐΂̉E
				insekiPosX < playerPosX + PLAYER_SIZE_X  && //覐΂̍� < ���@�̉E
				playerPosY < insekiPosY + INSEKI_SIZE_Y -5 && //���@�̏� < �G�̉�
				insekiPosY < playerPosY + PLAYER_SIZE_Y -5     //覐΂̏� < ���@�̉�
				)
			{
				//�@���X�|�[������
				playerFlag = false;
				insekiFlag = false;

				if (zanki >= 0)
				{
					zanki -= 1;
					if (zanki == -1)
					{
						WaitKey();
						zanki += 4;
						score = 0;
					}
				}
				//TRACE("��������\n");
				//_dbgDrawFormatString(0, 48, 0xffffff,"��������\n");
			}
		}
		
		if (shotFlag && enemyFlag)
		{
			//���@�̒e�ƓG�̓����蔻��
			if (shotPosX < enemyPosX + ENEMY_SIZE_X -20 &&
				enemyPosX < shotPosX + SHOT_SIZE_X -10&&
				shotPosY < enemyPosY + ENEMY_SIZE_Y -20 &&
				enemyPosY < shotPosY + SHOT_SIZE_Y -10
				)
			{
				enemyFlag = false;
				score += 100;

				shotFlag = false;
				shotPosX = SCREEN_SIZE_X;
				shotPosY = SCREEN_SIZE_Y;
			}
		}
		if (shotFlag && insekiFlag)
		{
			//���@�̒e��覐΂̓����蔻��
			if (shotPosX < insekiPosX + INSEKI_SIZE_X &&
				insekiPosX < shotPosX + SHOT_SIZE_X &&
				shotPosY < insekiPosY + INSEKI_SIZE_Y -5&&
				insekiPosY < shotPosY + SHOT_SIZE_Y -5
				)
			{
				insekiFlag = false;
				score += 50;

				shotFlag = false;
				shotPosX = SCREEN_SIZE_X;
				shotPosY = SCREEN_SIZE_Y;
			}
		}



		//�`�揈��
		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		DrawGraph(0, bgPosY1, bgImage, true);
		DrawGraph(0, bgPosY2, bgImage, true);
		if (shotFlag) //!shotFlag�񔭎˒�
		{
			DrawGraph(shotPosX, shotPosY, shotImage, true);
		}
		if (playerFlag)
		{
			DrawGraph(playerPosX, playerPosY, playerImage, true);
		}
		if (enemyFlag)
		{
			DrawGraph(enemyPosX, enemyPosY, enemyImage, true);
		}
		if (insekiFlag)
		{
			DrawGraph(insekiPosX, insekiPosY, insekiImage, true);
		}
		DrawGraph(0, kumoPosY1, kumoImage, true);
		DrawGraph(0, kumoPosY2, kumoImage, true);
		DrawFormatString(0, 0, 0xffffff, "�X�R�A : %d", score);
		if (zanki == -1)
		{
			DrawFormatString(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2, 0xffffff, "�X�R�A : %d", score);
		}
		DrawFormatString(560, 464, 0xffffff, "�c�@ : %d", zanki);
		_dbgDraw();
		ScreenFlip();
		
	}

	DxLib_End();
	return 0;
}














