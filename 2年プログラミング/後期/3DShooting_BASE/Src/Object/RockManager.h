#pragma once

#include <vector>
#include <map>
#include "../Common/IntVector3.h"

class Transform;
class Rock;

class RockManager
{
public:
	//1�}�b�v������̃T�C�Y
	static constexpr int MAP_SIZE = 2000;

	//1�}�b�v������ɐ��������̐�
	static constexpr int NUM_CREATE = 30;



	// �R���X�g���N�^
	RockManager(const Transform* target);

	// �f�X�g���N�^
	~RockManager(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	//�␶���̒��S
	const Transform* mTarget;

	//�}�b�v�Ǘ��̓��I�z��
	std::map<IntVector3, std::vector<Rock*>> mMapRocks;

	//��������_������
	Rock* CreateRandom(IntVector3 mapPos);
};

