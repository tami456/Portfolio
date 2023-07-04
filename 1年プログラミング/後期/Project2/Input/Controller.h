#pragma once
#include <map>
#include <array>
#include "InputID.h"

class BaseScene;
class Actor;

using InputData = std::array<int, static_cast<int>(InputID::Max)>;	//�����l��Max�̐����������
using InputDatas = std::array<InputData, 2>;


class Controller		//�C���^�[�t�F�[�X��p�ӂ���N���X
{
public:
	Controller();
	~Controller();
	virtual void Update(BaseScene& scene,const Actor* own = nullptr) = 0;
	const InputDatas& GetInputData(void) { return inputData_; }
protected:
	std::map<InputID, int> keyMap_;		//keyData_�̒��ɉ������邩���i�[
	InputDatas inputData_;	//�ꎟ���ځF���݂̒l�@�񎟌��ځF�P�t���[���O�̒l
};

