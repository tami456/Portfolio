#pragma once
#include <DxLib.h>
class Transform;

class Capsule
{

public :

	// �f�o�b�O���̊ȈՃJ�v�Z���\���̐F
	static constexpr int COLOR = 0xffffff;

	// �R���X�g���N�^
	// �J�v�Z��������I�u�W�F�N�g���w�肷��
	Capsule(Transform* parent);

	// �f�X�g���N�^
	~Capsule(void);

	// �`��
	void Draw(void);

	// �eTransform����̑��Έʒu���Z�b�g
	void SetRelativePosTop(const VECTOR& pos);
	void SetRelativePosDown(const VECTOR& pos);

	VECTOR GetPosTop(void) const;
	VECTOR GetPosDown(void) const;
	VECTOR GetRelativePos(const VECTOR& pos) const;

	// ���a
	float GetRadius(void) const;
	void SetRadius(float radius);

	// ����
	float GetHeight(void) const;

	// �J�v�Z���̒��S���W
	VECTOR GetCenter(void) const;

	// �R�s�[
	Capsule Copy(Transform* base);

private :

	Transform* mTransformParent;

	// �eTransform����̑��Έʒu(�㑤)
	VECTOR mRelativePosTop;

	// �eTransform����̑��Έʒu(����)
	VECTOR mRelativePosDown;

	// ���a
	float mRadius;

};

