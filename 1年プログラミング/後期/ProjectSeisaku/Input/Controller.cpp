#include<dxlib.h>
#include "Controller.h"
#include "../common/Frame.h"

Controller::Controller()
{
	//[0][0]�͗v�f�̐擪�@�擪�̌^�̃T�C�Y��n���΁A���̂Ƃ��̐������T�C�Y���Ƃ��Ă��Ă����
	//�����āA���̗v�f���Ƃ����Ă�����B
	memset(inputData_[NowFlame].data(), 0xff, inputData_[NowFlame].size() * sizeof(inputData_[NowFlame][0]));
	memset(inputData_[OldFlame].data(), 0xff, inputData_[OldFlame].size() * sizeof(inputData_[OldFlame][0]));

}
Controller::~Controller()
{

}

