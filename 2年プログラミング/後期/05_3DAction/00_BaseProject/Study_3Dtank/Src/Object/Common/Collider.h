#pragma once
class Collider
{
public :

	// �Փˎ��
	enum class TYPE
	{
		STAGE,
	};

	// �R���X�g���N�^
	Collider(TYPE type, int modelId);

	// �f�X�g���N�^
	~Collider(void);

	// �Փˎ��
	TYPE mType;

	// ���f���̃n���h��ID
	int mModelId;

};

