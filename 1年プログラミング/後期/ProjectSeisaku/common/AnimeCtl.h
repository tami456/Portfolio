#pragma once
#include <string>
#include <map>
#include "../Actor/State.h"
#include "../Actor/ActorType.h"
#include "../Actor/Dir.h"
#include "../tmx/rapidxml.hpp"
#include "../tmx/rapidxml_utils.hpp"
#include "Vector2D.h"
#include "ImageCtl.h"

using AnimeData = std::pair<int, int>;		//first:�O���t�B�b�N�n���h���@second:�ێ��t���[��
using SequenceTable = std::vector<AnimeData>;

struct SequenceData
{
	int loopFlag_;
	SequenceTable table_;
};

class AnimeCtl
{
public:
	//���ɍ��܂ꂽ�f�[�^���Đ�����N���X
	AnimeCtl();
	~AnimeCtl();
	void Draw(void);
	void reConnect(Vector2D* pos);
	State SetState(State state);
	State GetState(void);
	Vector2D GetFootPos(void) { return *pos_; }
	bool IsAnimeEnd(void);

	Dir dir_[2];		//0:���݂̃t���[���̕����@1:�P�t���[���O�̕���
private:
	State state_;
	int loopCount_;		//���݂̃��[�v��
	int animePoint_;	//���݂�table_�̍Đ��ʒu
	int animeCount_;	//���݂̃e�[�u���̍Đ��ʒu�ł̌o�߃t���[��
	bool isAnimeEnd;	//�A�j���[�V�����̏I���t���O
	ImageVec imageVec_;
	std::string source_;
	Vector2D* pos_;
	std::map<State, SequenceData> sequence_;
	friend struct AnimeLoader;		//���O�̂��͉̂��̂��� �A�N�Z�X������˔j���ăA�N�Z�X�ł���@���p���Ȃ�
};

struct AnimeLoader
{
	AnimeCtl execute(ActorType type);
};
