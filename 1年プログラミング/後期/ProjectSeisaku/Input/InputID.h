#pragma once
#include <type_traits>

enum class InputID
{
	Up,
	Down,
	Left,
	Right,
	Btn1,
	Btn2,
	Btn3,
	Max
};

//enum�N���X�̃I�y���[�^�[�̃I�[�o�[���[�h

static InputID begin(InputID)
{
	return InputID::Up;
}
static InputID end(InputID)
{
	return InputID::Max;
}

static InputID operator* (InputID& id)
{
	return id;		//���̂�Ԃ�
}

static InputID operator++ (InputID& id)	//�n�����I�u�W�F�N�g��ς������̂ŁAconst�͂���Ȃ�
{
	id = static_cast<InputID>(std::underlying_type<InputID>::type(id) + 1);	//�N���X�̊���̌^(���̏ꍇ��id)���擾����
	return id;		//���l��++���ĕԂ�
}