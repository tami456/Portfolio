#pragma once
#include <string>
#include <vector>

class Resource
{

public:
	
	// ���\�[�X�^�C�v
	enum class TYPE
	{
		NONE,
		IMG,
		IMGS,
		MODEL,
		EFFEKSEER
	};

	// �R���X�g���N�^
	Resource(void);
	// �R���X�g���N�^
	Resource(TYPE type, const std::string& path);
	// �R���X�g���N�^(IMGS�p)
	Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

	// �f�X�g���N�^
	~Resource(void);

	// �ǂݍ���
	void Load(void);

	// ���
	void Release(void);

	// �����摜�n���h����ʔz��ɃR�s�[
	void CopyHandle(int* imgs);

	// ���\�[�X�^�C�v
	TYPE mType;

	// ���\�[�X�̓ǂݍ��ݐ�
	std::string mPath;

	// �摜�ƃ��f���̃n���h��ID
	int mHandleId;

	// IMGS::LoadDivGraph�p
	int* mHandleIds;
	int mNumX;
	int mNumY;
	int mSizeX;
	int mSizeY;

	// ���f�������p
	std::vector<int> mDuplicateModelIds;

};

