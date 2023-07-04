#pragma once
class Stage
{
public:

	// �O���b�h���̑傫��
	static constexpr float LEN_MASS = 100.0f;

	// �O���b�h���̕`�搔
	static constexpr float LEN_NUM = 50.0f;

	// �R���X�g���N�^
	Stage(void);

	// �f�X�g���N�^
	~Stage(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawGrid(void);
	void Release(void);

	//Game�V�[���ɃX�e�[�W�N���X�錾
	//DrawGrid��XYZ���`��

	int GetModelDungeon(void);
	int GetModelCargoShip(void);
	VECTOR GetPosCargoShip(void);
private:

	//�_���W����
	int mModelDungeon;

	//���
	int mModelCargoShip;

	VECTOR mPosDungeon;
	VECTOR mPosCargoShip;
};

