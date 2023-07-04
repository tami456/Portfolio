#pragma once
#include "../COMMON/Vector2.h"
#include "Common.h"

#define BLOCK_PTN_MAX (7)
#define BLOCK_COUNT_INV (3)
#define BLOCK_MAX_SIZE_CNT ((BLOCK_PTN_MAX - 1) * BLOCK_COUNT_INV)


class MapCtl
{
public:
	MapCtl();
	~MapCtl();
	void Update(void);
	void Draw(void);
	bool Init(void);
	bool Release(void);
	

	//�w�肵�����W�Ƀu���b�N��ݒu����
	int SetBlock(Vector2 pos, int cnt);
	int SetBlock(Vector2 pos);

	//�w�肵�����W�̃u���b�N�̏�Ԃ��擾����
	int GetBlock(Vector2 pos);

	//�u���b�N�̃T�C�Y��Ԃ�
	int GetBlockSize(void);

	//�ő�T�C�Y�ɂȂ��Ă���u���b�N�ɐڐG���Ă��邩���擾����
	bool isOnBlock(Vector2 pos);
private:
	int blockImage_[BLOCK_PTN_MAX];

	int mapData_[MAP_CHIP_COUNT_Y][MAP_CHIP_COUNT_X];


};

