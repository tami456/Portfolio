#include <DxLib.>

#define SCREEN_SIZE_X 640
#define SCREEN_SIZE_Y 480

#define PLAYER_SIZE_X 32
#define PLAYER_SIZE_Y 32

#define ENEMY_SIZE_X 64
#define ENEMY_SIZE_Y 64

// �v���C���[�֘A
int player Image;	//�O���t�B�b�N�n���h��
int player PosX;	//X���W
int player PosY;	//Y���W
int playerSpeed;	//�ړ���(�X�s�[�h�j

//�G�l�~�[�֘A
int enemyImage	//�O���t�B�b�N�n���h��
int enemyPosX;	//X���W
int enemyPosY;	//Y���W
int enemySpeed;	//�ړ���(�X�s�[�h)

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//����������
SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
ChangeWindowMode(true);
if DxLib_Init() == -1)
{
	return -1;
}

//�摜�̓ǂݍ���
playerImage = LoadGraph("image/player.png");

enemyImage = LoadGraph("image/enemy.png");

//�ϐ��̏�����
playerPosX = (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;
playerSpeed = 1;

while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
{
	// playerPosX = playerPosX + 1;
	// playerPosX =+ 1;
	// playerPosX++; //�����Ӗ�
	if (CheckHitkey(KEY_INPUT_NUMPAD4))
	{
		playerPosX -= playerSpeed;
	}
	if (CheckHitKey(KEY_INPUT_NUMPAD6))
	{
		playerPosX -= playerSpeed;
	}
	if (CheckHitKey(KEY_INPUT_NUMPAD2))
	{
		playerPosY += playerSpeed;
	}








	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(playerPosX,  playerPosY, playerImage, true);
	DrawGraph(enemyPosX, enemyPosY, enemyImage, true);
	ScreenFlip();
}

DxLib_End();
return 0
}