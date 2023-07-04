#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		START_LOGO,
		END_LOGO,
		ALERT_MSG,
		LIGHT,
		SKYDOME,
		PLAYER_SHIP,
		LEADER_SHIP,
		BOSS_SHIP,
		DUNGEON,
		SHIP_EXPLOSION,
		SHOT_EXPLOSION,
		ALPHABET,
		SPEECH_BALLOON,
		SHOT_MODEL,
		TURRET_STAND,
		TURRET_GUN,
		ROCK01,
		ROCK02,
		JET,
		BOOST,
		EXPLOSION,
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* mInstance;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> mResourcesMap;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> mLoadedMap;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void);

	// �������[�h
	Resource* _Load(SRC src);

};

