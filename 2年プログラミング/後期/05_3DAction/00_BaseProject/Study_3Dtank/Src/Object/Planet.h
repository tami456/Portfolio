#pragma once
#include "../Common//Quaternion.h"
#include "Stage.h"
class GravityManager;
class WarpStar;

class Planet
{

public:

	// �d�͂̋���
	static constexpr float DEFAULT_GRAVITY_POW = 25.0f;

	// �d�͂�������͈�
	static constexpr float DEFAULT_GRAVITY_RADIUS = 5000.0f;

	// �Q�[���I�[�o�[�͈�
	static constexpr float DEFAULT_DEAD_LENGTH = 1000.0f;

	enum class TYPE
	{
		GROUND,
		SPHERE,
		TRANS_ROT,
		TRANS_CAMERA_FIXED,
		ROAD,
	};

	Planet(Stage::NAME name, TYPE type, Transform transform);
	~Planet(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	
	// �f�����
	TYPE GetType(void) const;

	// �f����
	Stage::NAME GetName(void) const;

	// �ʒu�̐ݒ�
	void SetPosition(const VECTOR& pos);

	// ��]�̐ݒ�
	void SetRotation(const Quaternion& rot);

	// �d�͂̋����̎擾�E�ݒ�
	float GetGravityPow(void) const;
	void SetGravityPow(float pow);
	
	// �d�͔͈͂̎擾�E�ݒ�
	float GetGravityRadius(void) const;
	void SetGravityRadius(float radius);

	Transform* GetTransform(void);

	// �d�͔͈͓�
	bool InRangeGravity(const VECTOR& pos) const;

	// �Q�[���I�[�o�[�͈͓�
	bool InRangeDead(const VECTOR& pos) const;

	// �Q�[���I�[�o�[�͈�
	void SetDeadLength(float len);

private:

	GravityManager* mGravityManager;

	Transform mTransform;

	// �f�����
	TYPE mType;

	// �f����
	Stage::NAME mName;

	// �d�͂̋���
	float mGravityPow;

	// �d�͔͈�
	float mGravityRadius;

	// �Q�[���[�I�[�o�[����
	float mDeadLength;

};
