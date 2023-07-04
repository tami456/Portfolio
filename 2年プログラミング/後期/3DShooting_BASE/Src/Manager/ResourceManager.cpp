#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::mInstance = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (mInstance == nullptr)
	{
		mInstance = new ResourceManager();
	}
	mInstance->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *mInstance;
}

void ResourceManager::Init(void)
{

	Resource res;

	// �X�^�[�g���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "StartLogo.png");
	mResourcesMap.emplace(SRC::START_LOGO, res);

	// �G���h���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "EndLogo.png");
	mResourcesMap.emplace(SRC::END_LOGO, res);

	// �A���[�g
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Alert.png");
	mResourcesMap.emplace(SRC::ALERT_MSG, res);

	// �p�[�e�B�N���G�t�F�N�g�p
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Light.png");
	mResourcesMap.emplace(SRC::LIGHT, res);

	// �X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	mResourcesMap.emplace(SRC::SKYDOME, res);

	// �v���C���[�@
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "PlayerShip/PlayerShip.mv1");
	mResourcesMap.emplace(SRC::PLAYER_SHIP, res);

	// ���[�_�[�@
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "LeaderShip/LeaderShip.mv1");
	mResourcesMap.emplace(SRC::LEADER_SHIP, res);

	// �{�X���
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BossShip/BossShip.mv1");
	mResourcesMap.emplace(SRC::BOSS_SHIP, res);

	// �_���W����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Dungeon.mv1");
	mResourcesMap.emplace(SRC::DUNGEON, res);

	// �@�̂̔����G�t�F�N�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "ShipExplosion.png", 7, 1, 120, 120);
	mResourcesMap.emplace(SRC::SHIP_EXPLOSION, res);

	// �e�̔����G�t�F�N�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "ShotExplosion.png", 7, 1, 120, 120);
	mResourcesMap.emplace(SRC::SHOT_EXPLOSION, res);

	// �����摜
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Alphabet.png", 10, 6, 32, 32);
	mResourcesMap.emplace(SRC::ALPHABET, res);

	// �����o��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeechBalloon.png");
	mResourcesMap.emplace(SRC::SPEECH_BALLOON, res);

	// �e
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Shot/Shot.mv1");
	mResourcesMap.emplace(SRC::SHOT_MODEL, res);

	// �C��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BossShip/Turret.mv1");
	mResourcesMap.emplace(SRC::TURRET_STAND, res);

	// �C�g
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BossShip/Turret_Gun.mv1");
	mResourcesMap.emplace(SRC::TURRET_GUN, res);

	// �w�i��P
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Rock21.mv1");
	mResourcesMap.emplace(SRC::ROCK01, res);

	// �w�i��Q
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Rock23.mv1");
	mResourcesMap.emplace(SRC::ROCK02, res);

	// ���@����
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Jet.efkefc");
	mResourcesMap.emplace(SRC::JET, res);

	// ���@�u�[�X�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Boost.efkefc");
	mResourcesMap.emplace(SRC::BOOST, res);

	// ���@�̔����G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Explosion.efkproj");
	mResourcesMap.emplace(SRC::EXPLOSION, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : mLoadedMap)
	{
		p.second->Release();
		delete p.second;
	}

	mLoadedMap.clear();
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->mHandleId);
	res->mDuplicateModelIds.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	delete mInstance;
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = mLoadedMap.find(src);
	if (lPair != mLoadedMap.end())
	{
		return lPair->second;
	}

	const auto& rPair = mResourcesMap.find(src);
	if (rPair == mResourcesMap.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	mLoadedMap.emplace(src, ret);

	return ret;
}
