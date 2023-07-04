#pragma once

//�V�[��ID
enum SCN_ID {
	SCN_ID_TITLE,
	SCN_ID_GAME,
	SCN_ID_GAME_OVER,
	SCN_ID_MAX
};



//�L�����̌���
enum DIR_ID {
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_MAX
};

//�G�̍��E�̈ړ�����
enum MOVE_LR {
	MOVE_LEFT,		//���ړ�
	MOVE_RIGHT,		//�E�ړ�
	MOVE_MAX
};


enum MOVE_MODE{
	MOVE_MODE_LR,	//���E�ړ����[�h
	MOVE_MODE_DOWN,	//���ړ����[�h
	MOVE_MODE_MAX	//
};



enum ENEMY_TYPE {
	ENEMY_TYPE_1,
	ENEMY_TYPE_2,
	ENEMY_TYPE_3,
	ENEMY_TYPE_4,
	ENEMY_TYPE_5,
	ENEMY_TYPE_6,
	ENEMY_TYPE_UFO,
	ENEMY_TYPE_MAX
};

//�X�R�A
enum {
	SCR_PL1,		//�v���C���[
	SCR_HIGH,		//�n�C�X�R�A
	SCR_MAX	
};

#define TITLE_LOGO_IMAGE_SIZE_X 423		//�^�C�g���̃��S�T�C�Y�C���[�WX
#define TITLE_LOGO_IMAGE_SIZE_Y 198		//�^�C�g���̃��S�T�C�Y�C���[�WY

#define GAME_OVER_LOGO_IMAGE_SIZE_X 452	//�Q�[���I�[�o�[�̃��S�T�C�Y�C���[�WX

#define ENEMY_NOMAL_MAX ENEMY_TYPE_UFO

#define SCREEN_SIZE_X 800	//��ʂ̉��T�C�Y
#define SCREEN_SIZE_Y 600	//��ʂ̏c�T�C�Y

#define GAME_SCREEN_X 40	//�Q�[���G���A��ʂ̕\���ʒu
#define GAME_SCREEN_Y 75	//�Q�[���G���A��ʂ̕\���ʒu

#define GAME_SCREEN_SIZE_X 640	//�Q�[���G���A��ʂ̉��T�C�Y
#define GAME_SCREEN_SIZE_Y 480	//�Q�[���G���A��ʂ̏c�T�C�Y


#define ANIM_MAX 3			//�A�j���[�V�����p�^�[����

#define CHIP_SIZE_X 32		//�L�����̃T�C�YX
#define CHIP_SIZE_Y 32		//�L�����̃T�C�YY

#define EFFECT_SIZE_X 40	//�����G�t�F�N�g�T�C�YX
#define EFFECT_SIZE_Y 40	//�����G�t�F�N�g�T�C�YY



#define ENEMY_COUNT_X 8		//�G�̉����т̐�
#define ENEMY_COUNT_Y 7		//�G�̏c���т̐�
#define ENEMY_SPEED 8




#define PSHOT_SIZE_X 5			//���@�̒e�̃T�C�YX
#define PSHOT_SIZE_Y 25			//���@�̒e�̃T�C�YY
#define PSHOT_SPEED (-5)		//���@�̒e�̑��x

#define ESHOT_SIZE_X 16			//�G�̒e�̃T�C�YX
#define ESHOT_SIZE_Y 25			//�G�̒e�̃T�C�YY
#define ESHOT_SPEED 4			//�G�̒e�̑��x

#define ENEMY_SCR 10			//�ʏ�̓G��|�������̓_��
#define DEF_LINE_Y (GAME_SCREEN_SIZE_Y - CHIP_SIZE_Y * 2)	//�h�q���C����Y���W

#define UFO_SCR 50

void TitleScene(void);
void TitleDraw(void);

void GameScene(void);
void GameDraw(void);

void GameOverScene(void);
void GameOverDraw(void);

void EnemyCtr(void);
void PlayerCtr(void);
void ShotCtr(void);
void CheckHitObj(void);
void PlayerDeathProc(bool backFlag);
void PlayerReProc(void);
void DrawTexts(void);
bool Init(void);
bool SysInit(void);
void GetKeyState(void);			//�L�[���擾�֐�

