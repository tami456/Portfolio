#pragma once
#include <array>
#include <map>
#include <vector>
#include <list>
//���Ԍn
#include <chrono>
#include "InputID.h"
#include "../Obj/DirLR.h"

enum class Trg
{
	Now,
	Old,
	Max
};

using TrgBool = std::array<bool,static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID,TrgBool>;
using ChronoSysClock = std::chrono::system_clock::time_point;

//PlayerCmd�̉����猩�Ă���
	//std::vector�c�L�[,���Ԃ��i�[�Astd::list�cvector�����X�g�ŊǗ�����Astd::array�c����(2���)
	//std::map�c�Z�̐���
using CommandMap = std::map<std::string, std::array<std::list<std::vector<int>>, 2>>;
//�t�C�e���[�^�[ �c ++�����Ƃ��Ɍォ��O�ɍs���Ƃ��̂��Ɓc begin �c end - 1�̂���
//�t�C�e���[�^��begin �c rbeign end �c rend
//rbegin��++����ƐÃC�e���[�^�[����t�Ɍ�����--��������
//�ÃC�e���[�^�[�̎� �c begin�͍ŏ��̈ʒu
//�t��Âɓ��ꂽ�肻�̋t�ɓ����̂�����
//���݂���ꍇ�͂ǂ������œ��ꂵ�Ă��邵���Ȃ�

class Controller
{
public:

//�����O�o�b�t�@�c�R�}���h�̎�t�o�b�t�@���~��ɂȂ��Ă���̂�1������Ə㏑������Ă���
//�����c���萻�̑o�������X�g�����
//1�v�f�Ȃ̂łǂ�ǂ�new���Ă���
	struct RingBuf
	{
		//�{�^�����̕ۑ����ł���
		int value = 0;
		//�L�[�������Ă���̌o�ߎ���
		double time = 0.0;
		//�O
		RingBuf* before = nullptr;
		//��
		RingBuf* next = nullptr;

		static RingBuf* Create(int no);
		static void Destroy(RingBuf* buf);
	
	};

	Controller(){};
	virtual ~Controller(){};
	virtual void Update(void) = 0;
	const CntData& GetCntData(void)
	{
		return cntData_;
	}

	std::string CheckCommand(DIR_LR dir,CommandMap& commandMap);	//�L�����̌����ƃR�}���h�̃��X�g�����炤

private:
	ChronoSysClock startTime_;

protected:
	void WriteCmd(void);
	RingBuf* ringBuf_ = nullptr;
	RingBuf* startBuf_ = nullptr;
	CntData cntData_;

};



//�R�}���h�̃}�b�`���O��Ɓc��납��T������������