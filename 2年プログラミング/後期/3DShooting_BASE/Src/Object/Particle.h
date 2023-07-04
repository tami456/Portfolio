#pragma once
#include <DxLib.h>

class Particle
{
public:
	// �R���X�g���N�^
	Particle(void);

	// �f�X�g���N�^
	~Particle(void);

	void Generate(int img,VECTOR pos,float size,
		VECTOR dir,float speed, float lifeTime);
	void Update(void);
	void Draw(void);
	void Release(void);

	//��������
	bool isAlive(void) const;

	VECTOR GetPos(void) const;
	void SetPos(const VECTOR& pos);

	float GetZLen(void) const;
	void SetZLen(const float& zlen);

private:

	//�摜�̃n���h��ID
	int mImg;

	//���W
	VECTOR mPos;

	//�傫��
	float mSize;

	//�ړ�����
	VECTOR mDir;

	//�ړ����x
	float mSpeed;

	//��������
	float mLife;

	//Z����
	float mZLen;
};

