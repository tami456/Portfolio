#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"
class SpaceDome
{
public:

	//���
	enum class STATE {
		NONE,
		STAY,
		FOLLOW
	};

	// �R���X�g���N�^
	SpaceDome(const Transform* target);

	// �f�X�g���N�^
	~SpaceDome(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:

	//���f����3����
	Transform mTransform;

	//�Ǐ]����
	const Transform* mTarget;

	STATE mState;

	void ChangeState(STATE state);

};

