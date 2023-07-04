#pragma once
#include "Controller.h"

class KeyInput :
    public Controller
{
public:
    KeyInput();
    ~KeyInput();

private:
   // CntType GetType(void) override { return CntType::Key; };
    void Update(void)override;
    std::array<char, 256> keyData_;
    //�\�����i�[���邽�߂̂���
    std::map<InputID, int>inputTbl_;
};

