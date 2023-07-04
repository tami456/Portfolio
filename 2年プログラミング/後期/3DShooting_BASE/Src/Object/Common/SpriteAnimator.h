#pragma once
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
class Transform;

class SpriteAnimator
{

public:

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		END
	};

	// �R���X�g���N�^
	SpriteAnimator(ResourceManager::SRC src, float size, float speed);

	// �f�X�g���N�^
	~SpriteAnimator(void);

	// Init�ECreate�̋�ʂ���₱�����̂ŁAInit����

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="pos">�����\�����W</param>
	/// <param name="transform">�w�肳�ꂽ�I�u�W�F�N�g��ǐՂ���</param>
	void Create(VECTOR pos, const Transform* transform);

	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;
	void SetPos(VECTOR pos);

	float GetZLen(void) const;
	void SetZLen(float len);

	// �A�j���[�V�����I��
	bool IsEnd(void) const;

private:

	// �ǐՑΏۃI�u�W�F�N�g
	const Transform* mTrackingTransform;

	// �P�t���[���O�̒ǐՑΏۍ��W
	VECTOR mTrackingPrePos;

	// ���
	STATE mState;

	// �摜�n���h��
	int* mImages;

	// �Đ��C���f�b�N�X
	int mIdxAnim;

	// ���W
	VECTOR mPos;

	// �p�x
	VECTOR mAngles;

	// �傫��
	float mSize;

	// �A�j���[�V�����p�X�V�X�e�b�v
	float mStepAnim;

	// �J���������Z����
	float mZLen;

	// ���A�j���[�V������
	int mCntTotalAnim;

	// �A�j���[�V�����X�s�[�h
	float mSpeedAnim;

	// ��ԑJ��
	void ChangeState(STATE state);

};
