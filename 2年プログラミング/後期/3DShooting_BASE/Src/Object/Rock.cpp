#include <DxLib.h>
#include "Rock.h"

Rock::Rock(void)
{
}

Rock::~Rock(void)
{
}

void Rock::Init(int modelId, VECTOR pos, VECTOR angle, VECTOR scale)
{
	mTransform.SetModel(modelId);
	mTransform.scl = scale;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(angle);
	mTransform.pos = pos;
	mTransform.Update();
}

void Rock::Update(void)
{
}

void Rock::Draw(void)
{
	//�J�����N���b�v�O�ɂȂ�����`�悵�Ȃ�
	if (!CheckCameraViewClip(mTransform.pos))
	{
		MV1DrawModel(mTransform.modelId);
	}
	
}

void Rock::Release(void)
{
	
}
