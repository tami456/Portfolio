#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
class ResourceManager;
class AnimationController;

class Mascot
{
public:


	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		IDLE,
		WARP_RESERVE,
		WARP_MOVE,
		ENEMYDEAD,
		VICTORY,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		ENEMYDEAD
	};

	int ImgMascotIdle;
	int ImgMascotNice;

	Mascot();
	~Mascot();

	void Init(void);

	void Update(void);
	void UpdateIdle(void);
	void UpdateEnemyDead(void);

	void Draw(void);

	void Release(void);

private:
	ResourceManager& mResourceManager;

	Transform mTransform;

	// �A�j���[�V����
	AnimationController* mAnimationController;

	// ���
	STATE mState;

	void ChangeState(STATE state);
};