#pragma once
#include <map>
#include "../Common/Vector2.h"

class InputManager
{

public:

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static InputManager& GetInstance(void);

	void Init(void);
	void Update(void);

	// ������s���L�[��ǉ�
	void Add(int key);

	// ������s���L�[���N���A
	void Clear(void);

	// �L�[�̉�������
	bool IsNew(int key) const;

	// �L�[�̉�������(�������ςȂ���NG)
	bool IsTrgDown(int key) const;

	// �L�[�𗣂������̔���
	bool IsTrgUp(int key) const;

	// �}�E�X���W�̎擾
	Vector2 GetMousePos(void) const;

	// �}�E�X�̃N���b�N��Ԃ��擾(MOUSE_INPUT_LEFT�ARIGHT)
	int GetMouse(void) const;

	// �}�E�X�����N���b�N���ꂽ��
	bool IsClickMouseLeft(void) const;

	// �}�E�X���E�N���b�N���ꂽ��
	bool IsClickMouseRight(void) const;

	// �}�E�X�����N���b�N���ꂽ��(�������ςȂ���NG)
	bool IsTrgMouseLeft(void) const;

	// �}�E�X���E�N���b�N���ꂽ��(�������ςȂ���NG)
	bool IsTrgMouseRight(void) const;

private:

	// �L�[���
	struct Info
	{
		int key;			// �L�[ID
		bool keyOld;		// 1�t���[���O�̉������
		bool keyNew;		// ���t���[���̉������
		bool keyTrgDown;	// ���t���[���Ń{�^���������ꂽ��
		bool keyTrgUp;		// ���t���[���Ń{�^���������ꂽ��
	};

	// �}�E�X
	struct MouseInfo
	{
		int key;			// �L�[ID
		bool keyOld;		// 1�t���[���O�̉������
		bool keyNew;		// ���t���[���̉������
		bool keyTrgDown;	// ���t���[���Ń{�^���������ꂽ��
		bool keyTrgUp;		// ���t���[���Ń{�^���������ꂽ��
	};

	// �V���O���g���p�C���X�^���X
	static InputManager* mInstance;

	// �L�[���
	std::map<int, InputManager::Info> mKeyInfos;

	// �}�E�X���
	std::map<int, InputManager::MouseInfo> mMouseInfos;

	// �}�E�X�J�[�\���̈ʒu
	Vector2 mMousePos;
	
	// �}�E�X�{�^���̓��͏��
	int mMouseInput;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	InputManager(void);
	~InputManager(void);

	// �z��̒�����L�[�����擾����
	const InputManager::Info& Find(int key) const;

	// �z��̒�����}�E�X�����擾����
	const InputManager::MouseInfo& FindMouse(int key) const;

};