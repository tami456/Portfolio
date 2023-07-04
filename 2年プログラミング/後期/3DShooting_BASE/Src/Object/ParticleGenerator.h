#pragma once
#include <DxLib.h>
#include <vector>
#include "../Common/Vector2.h"
#include "../Common/Quaternion.h"

class Transform;
class Particle;

class ParticleGenerator
{
public:

	
	static constexpr int NUM_CREATE = 30;
	//����~�`���b�V��
	static constexpr int NUM_ANGLE = 10;
	static constexpr float SPLIT_ANGLE = 360.0f / NUM_ANGLE;

	//���X�ɐ���
	static constexpr float GEN_TIME = 0.1f;

	// �R���X�g���N�^
	ParticleGenerator(VECTOR pos,float radius);

	// �f�X�g���N�^
	~ParticleGenerator(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawMeshSquare(void);
	void DrawMeshCircle(void);
	void Release(void);

	void SetPos(const VECTOR& pos);
	void SetRot(const Quaternion& rot);
private:

	//�p�[�e�B�N���摜
	int mImg;

	//���W
	VECTOR mPos;

	//����(�p�x)
	Quaternion mRot;

	
	//���a
	float mRadius;
	
	Transform* mTransform;

	//���_��
	int mCntVertex;

	float mTestRad;

	//3D�`��Ɏg�p���钸�_�f�[�^�̌^(�l�p�p)
	VERTEX3D mVerticesSquare[6];

	//���_�f�[�^(�~�`)
	VERTEX3D mVerticesCircle[NUM_ANGLE + 1];

	//���_�C���f�b�N�X
	WORD mIndexes[NUM_ANGLE * 3];

	//�p�[�e�B�N��
	std::vector<Particle*> mParticles;

	//�����X�ɐ������邽�߂̌o�ߎ���
	float mStepGenerate;

	//�l�p���b�V�������֐�
	void CreateMeshSquare(void);

	//�~�`���b�V�������֐�
	void CreateMeshCircle(void);

	Particle* Generate(Particle* particle);

	//Z�\�[�g
	void ZSort(void);
};

