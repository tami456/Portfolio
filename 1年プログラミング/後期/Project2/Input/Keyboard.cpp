#include <dxlib.h>
#include "Keyboard.h"
#include "../common/Frame.h"
Keyboard::Keyboard()
{
	//�L�[�{�[�h�̏�����
	memset(keyData_, 0, sizeof(keyData_));
	//�L�[�{�[�h�̐ݒ�
	keyMap_.try_emplace(InputID::Up, KEY_INPUT_UP);			//�L�[���Ȃ��Ƃ��ɁA�L�[�����
	keyMap_.try_emplace(InputID::Down, KEY_INPUT_DOWN);
	keyMap_.try_emplace(InputID::Left, KEY_INPUT_LEFT);
	keyMap_.try_emplace(InputID::Right, KEY_INPUT_RIGHT);
	keyMap_.try_emplace(InputID::Btn1, KEY_INPUT_Z);
	keyMap_.try_emplace(InputID::Btn2, KEY_INPUT_X);
	keyMap_.try_emplace(InputID::Btn3, KEY_INPUT_C);

}

Keyboard::~Keyboard()
{
}

void Keyboard::Update(BaseScene& scene, const Actor* own)
{
	memcpy(inputData_[OldFlame].data(), inputData_[NowFlame].data(), sizeof(inputData_[OldFlame][NowFlame]) * inputData_[OldFlame].size());
	//sizeof��()�̒��̃T�C�Y�����

	GetHitKeyStateAll(keyData_);

	for (auto id : InputID())
	{
		inputData_[0][static_cast<int>(id)] = keyData_[keyMap_[id]];
	}

}
