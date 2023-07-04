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

	// �^�C�g���摜
	res = Resource(Resource::TYPE::IMG, "Data/Image/Title.png");
	mResourcesMap.emplace(SRC::TITLE, res);

	// PushSpace
	res = Resource(Resource::TYPE::IMG, "Data/Image/PushSpace.png");
	mResourcesMap.emplace(SRC::PUSH_SPACE, res);

	//�Q�[���I�[�o�[�w�i
	res = Resource(Resource::TYPE::IMG, "Data/Image/GameOverBG.png");
	mResourcesMap.emplace(SRC::GAMEOVERBG, res);

	//�Q�[���I�[�o�[���S
	res = Resource(Resource::TYPE::IMG, "Data/Image/GameOver.png");
	mResourcesMap.emplace(SRC::GAMEOVER, res);

	// �����o��
	res = Resource(Resource::TYPE::IMG, "Data/Image/SpeechBalloon.png");
	mResourcesMap.emplace(SRC::SPEECH_BALLOON, res);
	
	// �v���C���[
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Player/Player.mv1");
	mResourcesMap.emplace(SRC::PLAYER, res);

	// �e
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Shot/Shot.mv1");
	mResourcesMap.emplace(SRC::SHOT, res);

	// ����
	res = Resource(Resource::TYPE::MODEL, "Data/Effect/Explosion/Explosion.efkproj");
	mResourcesMap.emplace(SRC::EXPLOSION, res);

	// �G�l�~�[
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Enemy/Enemy.mv1");
	mResourcesMap.emplace(SRC::ENEMY, res);

	// �}�X�R�b�g
	res = Resource(Resource::TYPE::MODEL, "Data/Image/Mascot/MascotIdle.png");
	mResourcesMap.emplace(SRC::MASCOT, res);

	// �}�X�R�b�g
	res = Resource(Resource::TYPE::MODEL, "Data/Image/Mascot/MascotNice.png");
	mResourcesMap.emplace(SRC::MASCOTNICE, res);

	// �X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/SkyDome/SkyDome.mv1");
	mResourcesMap.emplace(SRC::SKY_DOME, res);

	// �ŏ��̘f��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/MainPlanet.mv1");
	mResourcesMap.emplace(SRC::MAIN_PLANET, res);

	// ���Ƃ����̘f��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/FallPlanet.mv1");
	mResourcesMap.emplace(SRC::FALL_PLANET, res);

	// ���R�Șf��01
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/FlatPlanet01.mv1");
	mResourcesMap.emplace(SRC::FLAT_PLANET_01, res);

	// ���R�Șf��02
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/FlatPlanet02.mv1");
	mResourcesMap.emplace(SRC::FLAT_PLANET_02, res);

	// �Ō�̘f��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/LastPlanet.mv1");
	mResourcesMap.emplace(SRC::LAST_PLANET, res);

	// ���ʂȘf��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/RoadPlanet.mv1");
	mResourcesMap.emplace(SRC::SPECIAL_PLANET, res);

	// ����
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/Smoke/Smoke.efkefc");
	mResourcesMap.emplace(SRC::FOOT_SMOKE, res);

	// ���[�v�X�^�[���f��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Star/star.mv1");
	mResourcesMap.emplace(SRC::WARP_STAR, res);

	// ���[�v�X�^�[�p��]�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/StarDust/StarDust.efkefc");
	mResourcesMap.emplace(SRC::WARP_STAR_ROT_EFF, res);

	// ���[�v�̋O�Ր�
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/Warp/WarpOrbit.efkefc");
	mResourcesMap.emplace(SRC::WARP_ORBIT, res);

	// �u���b�N�z�[��
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/BlackHole/BlackHole.efkefc");
	mResourcesMap.emplace(SRC::BLACK_HOLE, res);

	// �S�[��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/GoalStar/GoalStar.mv1");
	mResourcesMap.emplace(SRC::GOAL_STAR, res);

	// Clear
	res = Resource(Resource::TYPE::IMG, "Data/Image/Congratulations.png");
	mResourcesMap.emplace(SRC::CLEAR, res);

	// �^���N
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Tank/Body.mv1");
	mResourcesMap.emplace(SRC::TANK_BODY, res);
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Tank/Wheel.mv1");
	mResourcesMap.emplace(SRC::TANK_WHEEL, res);
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Tank/Barrel.mv1");
	mResourcesMap.emplace(SRC::TANK_BARREL, res);

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
